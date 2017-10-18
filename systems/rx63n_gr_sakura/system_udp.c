#include "freertps/freertps.h"
#include "freertps/timer.h"
#include "freertps/udp.h"
#include "freertps/disco.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "r_t4_itcpip.h"

extern UB _myethaddr[][6];
extern T_UDP_CCEP udp_ccep[];
/*
 Each endpoint is assigned as below (see config_tcpudp.c). 
 Multicast builtin port => 1
 Unicast builtin port   => 2
 Multicast user port    => 3
 Unicast user port      => 4
*/
#define TCPUDP_CEPID_IDX_MCAST_BULTIN_PORT (1)
#define TCPUDP_CEPID_IDX_UCAST_BULTIN_PORT (2)
#define TCPUDP_CEPID_IDX_MCAST_USER_PORT   (3)
#define TCPUDP_CEPID_IDX_UCAST_USER_PORT   (4)
#define T4_WORK_SIZE (2048) /* This value shall be reviewed in future. */
static UW tcpudp_work[T4_WORK_SIZE / 4 + 1];

typedef uint32_t in_addr_t;
typedef uint16_t in_port_t;

typedef struct
{
  uint32_t head;
  uint32_t tail;
} frudp_rx_queue_t;
#pragma pack
typedef struct
{
  uint32_t src_addr;
  uint16_t src_port;
  uint16_t rx_len;
} frudp_rx_queue_header;
#pragma unpack

/*
 The format of data stream which be stored in rx_queue_buf is as below.
*1+-------------+----------------------------+---------------------- - - - -
  | Message     | Source address of received | Payload of received UDP frame
  | Length *2   | UDP frame                  |
  | (uint16_t)  | (T_IPV4EP)                 | (uint8_t[])
  +-------------+----------------------------+---------------------- - - - -
 *1 The data stream is aligned on 2-byte boundaries.
 *2 The length includes its own 2 bytes.
*/
#define SIZE_MAX_UDP_PAYLOAD (1472) /* Table2.2 of M3S-T4-Tiny User manual. */
#define SIZE_RX_QUEUE_LENGTH (sizeof(uint16_t))
#define SIZE_RX_QUEUE_ADDR (sizeof(T_IPV4EP))
#define SIZE_CB_RX_BUF (SIZE_MAX_UDP_PAYLOAD + SIZE_RX_QUEUE_LENGTH + SIZE_RX_QUEUE_ADDR)
#define SIZE_LISTEN_RX_BUF (SIZE_MAX_UDP_PAYLOAD + SIZE_RX_QUEUE_ADDR)
static UB cb_rx_buf[SIZE_CB_RX_BUF];
static UB listen_rx_buf[SIZE_LISTEN_RX_BUF];

#define MIN(a,b) (((a)<(b))?(a):(b))
#define RX_QUEUE_SIZE_EXP (12)
#define RX_QUEUE_SIZE (2 << RX_QUEUE_SIZE_EXP)
#define RX_QUEUE_SIZE_MASK (RX_QUEUE_SIZE - 1)

static uint8_t rx_queue_buf[RX_QUEUE_SIZE];
volatile static frudp_rx_queue_t rx_queue = {0, 0};

static uint32_t rx_queue_write(uint8_t *buf)
{
  uint32_t offset_head = rx_queue.head & RX_QUEUE_SIZE_MASK;
  uint32_t size_written = rx_queue.head - rx_queue.tail;
  uint32_t len, len_1st, len_2nd, len_aligned;

  /* Check if there is enough space or not. */
  len = (uint32_t)(*((uint16_t *)buf));
  len_aligned = len + (len & 0x01); /* to align on 2-byte boundary */
  if ((RX_QUEUE_SIZE - size_written) < len_aligned)
  {
    return 0;
  }

  /* Queue write */
  len_1st = MIN(len, RX_QUEUE_SIZE - offset_head);
  len_2nd = len - len_1st;
  memcpy(rx_queue_buf + offset_head, buf, len_1st);
  memcpy(rx_queue_buf, buf + len_1st, len_2nd);
  rx_queue.head += len_aligned;

  return len;
}

static uint32_t rx_queue_read(uint8_t *buf)
{
  uint32_t offset_tail = rx_queue.tail & RX_QUEUE_SIZE_MASK;
  uint32_t len, len_1st, len_2nd, len_aligned;

  /* Return immediately if no message is queued. */
  if (rx_queue.head == rx_queue.tail)
  {
    return 0;
  }

  /* Fetch message length. */
  len = *((uint16_t *)(rx_queue_buf /* always aligned on 2-byte boundary */
                       + (rx_queue.tail & RX_QUEUE_SIZE_MASK)));
  len_aligned = len + (len & 0x01); /* to align on 2-byte boundary */

  /* Queue read */
  len_1st = MIN(len, RX_QUEUE_SIZE - offset_tail);
  len_2nd = len - len_1st;
  memcpy(buf, rx_queue_buf + offset_tail, len_1st);
  memcpy(buf + len_1st, rx_queue_buf, len_2nd);
  rx_queue.tail += len_aligned;

  return len;
}

ER t4_udp_callback(ID cepid, FN fncd , VP p_parblk)
{
  T_IPV4EP t4_dst_addr;
  uint16_t data_len;
  ER ercd;

  switch (fncd)
  {
    case TEV_UDP_RCV_DAT:
    case TFN_UDP_RCV_DAT:
      ercd = udp_rcv_dat(cepid, &t4_dst_addr,
                         &cb_rx_buf[SIZE_RX_QUEUE_LENGTH + SIZE_RX_QUEUE_ADDR],
                         SIZE_MAX_UDP_PAYLOAD, TMO_POL);
      if (0 < ercd)
      {
        /* Write received message to RX queue. */
        data_len = (uint16_t)(ercd + SIZE_RX_QUEUE_LENGTH + SIZE_RX_QUEUE_ADDR);
        memcpy((void *)cb_rx_buf, (void *)&data_len, SIZE_RX_QUEUE_LENGTH); /* Payload length */
        memcpy((void *)(cb_rx_buf + SIZE_RX_QUEUE_LENGTH),
               (void *)&t4_dst_addr, SIZE_RX_QUEUE_ADDR); /* Destination address */
        (void)rx_queue_write((void *)cb_rx_buf);
      }
      break;
    case TFN_UDP_SND_DAT:
    default:
      break;
  }

  return 0;
}

extern bool tcpudp_is_link_mode_up(void);
bool frudp_init(void)
{
  const uint32_t pid = 0; /* FIXME: There is no I/F to set Participant ID. */
  ER ercd;
  W size;

  FREERTPS_INFO("udp init()\n");

  /* Start LAN controller */
  ercd = lan_open();
  if (ercd != E_OK)
  {
    while ((bool)1);
  }

  /* Initialize TCP/IP */
  size = tcpudp_get_ramsize();
  if (size > (sizeof(tcpudp_work)))
  {
    while ((bool)1);
  }
  ercd = tcpudp_open(tcpudp_work);
  if (ercd != E_OK)
  {
    while ((bool)1);
  }

  /* Wait for link up */
  while (false == tcpudp_is_link_mode_up())
  {
    ;
  }

  /* Set RTPS parameters */
  memcpy(&g_frudp_config.unicast_addr,
         &tcpudp_env[0].ipaddr[0], sizeof(uint32_t));
  g_frudp_config.guid_prefix.prefix[0] = FREERTPS_VENDOR_ID >> 8;
  g_frudp_config.guid_prefix.prefix[1] = FREERTPS_VENDOR_ID & 0xff;
  memcpy(&g_frudp_config.guid_prefix.prefix[2], &_myethaddr[0], 6);
  memcpy(&g_frudp_config.guid_prefix.prefix[8], &pid, 4);

  /* Initialize system independent parts */
  frudp_generic_init();

  return true;
}

bool frudp_init_participant_id(void)
{
  const uint16_t port = frudp_ucast_builtin_port();

  FREERTPS_INFO("frudp_init_participant_id()\n");

  g_frudp_config.participant_id = 0; /* FIXME: There is no I/F to set Participant ID. */

  if (frudp_add_ucast_rx(port))
  {
    return true;
  }
  else
  {
    return false;
  }
}

void frudp_fini(void)
{
  FREERTPS_INFO("udp fini\n");
  frudp_disco_fini();
}

bool frudp_add_ucast_rx(const uint16_t port)
{
  FREERTPS_INFO("add ucast rx port %d\n", port);
  /* M3S-T4-Tiny needs no initialization for each socket or endpoint. */
  return true;
}

bool frudp_add_mcast_rx(in_addr_t group, uint16_t port)
{
  static bool already_join_mcast = false;
  int i;
  bool found = false;
  ER retval;
  UW ngroup = (UW)freertps_htonl(group);

  /* IGMP join operation is required only once for M3S-T4-Tiny. */
  if (true == already_join_mcast)
  {
    return true;
  }

  /* Check port number */
  for (i = 0; i < TCPUDP_CEPID_IDX_UCAST_USER_PORT; i++)
  {
    if (udp_ccep[i].myaddr.portno == port)
    {
      found = true;
      break;
    }
  }

  /* Send IGMP join message */
  if (true == found) {
    retval = igmp_join_group(&group, 0); /* RJ45 number is fixed to zero. */
    if (0 != retval)
    {
      while ((bool)1){};
    }
    already_join_mcast = true;
  }

  return found;
}

bool frudp_listen(const uint32_t max_usec)
{
  double t_start = fr_time_now_double();
  double t_now = t_start;
  uint16_t len;

  do 
  {
    /* Regardless of what time interval is set, the listen procedure must
     be executed at least once per call.*/
    T_IPV4EP *t4_dst_addr = (T_IPV4EP*)(listen_rx_buf + (SIZE_RX_QUEUE_LENGTH));

    len = rx_queue_read(listen_rx_buf);
    if (0 != len)
    {
      frudp_rx(freertps_ntohl(t4_dst_addr->ipaddr),
               t4_dst_addr->portno,
               0x0, 0x0, /* Currently, these parameters are not used. */
               listen_rx_buf + (SIZE_RX_QUEUE_LENGTH + SIZE_RX_QUEUE_ADDR),
               len - SIZE_RX_QUEUE_LENGTH - SIZE_RX_QUEUE_ADDR);
    }

    t_now = fr_time_now_double();
  } while (0 != len || t_now - t_start < 1.0e-6 * max_usec);

  return true;
}

bool frudp_tx(const in_addr_t dst_addr,
              const in_port_t dst_port,
              const uint8_t *tx_data,
              const uint16_t tx_len)
{
  T_IPV4EP t4_dst_addr;
  ER retval;

  t4_dst_addr.ipaddr = (UW)freertps_htonl(dst_addr);
  t4_dst_addr.portno = dst_port;
  /* Send data using Unicast user port */
  retval = udp_snd_dat(TCPUDP_CEPID_IDX_UCAST_USER_PORT,
                       &t4_dst_addr, (VP *)tx_data, tx_len, TMO_FEVR);
  if (tx_len == retval)
  {
    return true;
  }
  else
  {
    return false;
  }
}
