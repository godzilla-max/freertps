/*******************************************************************************
* File Name     : config_tcpudp.c
* Version       : 1.00
* Device(s)     : Renesas microcomputer
* Tool-Chain    : Renesas compilers
* OS            :
* H/W Platform  :
* Description   : T4 configuration file
* Limitations   :
******************************************************************************/
/******************************************************************************
* History       : DD.MM.YYYY Version Description
*               : 10.08.2017 1.00    First release.
******************************************************************************/

#include "r_t4_itcpip.h"
#include "freertps/freertps.h"
#include "freertps/ports.h"
/* FIXME: There is no I/F to set domain ID and Participant ID. */
#define FREERTPS_PROTO_DOMAIN_ID (0)
#define FREERTPS_PROTO_PARTICIPANT_ID (0)

/***  These configuration below may be able to ***/
/***  removable without TCP/IP connection. ***/

/****************************************************************************/
/**********************     TCP-related definition     **********************/
/****************************************************************************/
/* Number of LAN port, Number of Serial port */
const UB _t4_channel_num = 1;
const UB _t4_dhcp_enable = 0;
const UH _t4_dhcp_ip_reply_arp_delay = 300;       /* unit:10ms */

/***  Definition of TCP reception point (only port number needs to be set) ***/
const T_TCP_CREP tcp_crep[1] =
{
    /* { attribute of reception point, {local IP address, local port number}} */
    { 0, { 0, 1024 }},

};

/* Total number of TCP reception points */
const H __tcprepn = sizeof(tcp_crep) / sizeof(T_TCP_CREP);

/***  Definition of TCP communication end point
      (only receive window size needs to be set) ***/
const T_TCP_CCEP tcp_ccep[1] =
{
    /* { attribute of TCP communication end point,
         top address of transmit window buffer, size of transmit window buffer,
         top address of receive window buffer, size of receive window buffer,
         address of callback routine }
    */
    { 0, 0, 0, 0, 1460, 0 },

};

/* Total number of TCP communication end points */
const H __tcpcepn = sizeof(tcp_ccep) / sizeof(T_TCP_CCEP);

/***  TCP MSS  ***/
const UH _tcp_mss[] =
{
    /* MAX:1460 bytes */
    1460,
};

/***  2MSL wait time (unit:10ms)  ***/
const UH    _tcp_2msl[] =
{
    (1 * 60 * (1000 / 10)),      /* 1 min */
};
/***  Maximum value of retransmission timeout period (unit:10ms)  ***/
const UH    _tcp_rt_tmo_rst[] =
{
    (10 * 60 * (1000 / 10)),     /* 10 min */
};
/***  Transmit for delay ack (ON=1/OFF=0) ***/
const UB   _tcp_dack[] = { 1 };

/***  These configuration above may be able to ***/
/***  removable without TCP/IP connection. ***/


/****************************************************************************/
/**********************     UDP-related definition     **********************/
/****************************************************************************/
/***  Definition of UDP communication end point  ***/

extern ER    t4_udp_callback(ID cepid, FN fncd , VP p_parblk);

const T_UDP_CCEP udp_ccep[4] =
{
    /* Multicast builtin port */
    { 0, { 0, FRUDP_PORT_PB + FRUDP_PORT_DG * FREERTPS_PROTO_DOMAIN_ID +
           FRUDP_PORT_D0}, t4_udp_callback},
    /* Unicast builtin port */
    { 0, { 0, FRUDP_PORT_PB + FRUDP_PORT_DG * FREERTPS_PROTO_DOMAIN_ID +
           FRUDP_PORT_D1 + FRUDP_PORT_PG * FREERTPS_PROTO_PARTICIPANT_ID}, t4_udp_callback},
    /* Multicast user port */
    { 0, { 0, FRUDP_PORT_PB + FRUDP_PORT_DG * FREERTPS_PROTO_DOMAIN_ID +
           FRUDP_PORT_D2}, t4_udp_callback},
    /* Unicast user port */
    { 0, { 0, FRUDP_PORT_PB + FRUDP_PORT_DG * FREERTPS_PROTO_DOMAIN_ID +
           FRUDP_PORT_D3 + FRUDP_PORT_PG * FREERTPS_PROTO_PARTICIPANT_ID}, t4_udp_callback}
};
/* Total number of UDP communication end points */
const H __udpcepn = (sizeof(udp_ccep) / sizeof(T_UDP_CCEP));

/***  TTL for multicast transmission  ***/
const UB __multi_TTL[] = { 1 };

/*** Behavior of UDP zero checksum ***/
const UB _udp_enable_zerochecksum[] = { 0 }; /* 0 = disable, other = enable */

/****************************************************************************/
/**********************  SYSTEM-callback definition   ***********************/
/****************************************************************************/

volatile bool tcpudp_link_mode = false;
ER user_cb(UB channel, UW eventid, VP param)
{
    uint8_t*    ev_tbl[] =
    {
        "ETHER_EV_LINK_OFF",
        "ETHER_EV_LINK_ON",
        "ETHER_EV_COLLISION_IP",
        "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
        "DHCP_EV_LEASE_IP",
        "DHCP_EV_LEASE_OVER",
        "DHCP_EV_INIT",
        "DHCP_EV_INIT_REBOOT",
        "DHCP_EV_APIPA",
        "DHCP_EV_NAK",
        "DHCP_EV_FATAL_ERROR",
        "DHCP_EV_PLEASE_RESET"
    };
    if (ETHER_EV_LINK_ON == eventid)
    {
	    tcpudp_link_mode = true;
    }
    FREERTPS_INFO("^^>>>user_cb<<< ch:%d,eventID = %s\n", channel, ev_tbl[eventid]);

    return 0;
}
const callback_from_system_t g_fp_user = user_cb;

/* The function below is used in system_udp.c. */
bool tcpudp_is_link_mode_up(void)
{
  return tcpudp_link_mode;
}


/****************************************************************************/
/**********************     IP-related definition     ***********************/
/****************************************************************************/
const UH _ip_tblcnt[] = { 3 };

#define MY_IP_ADDR0     192,168,1,101          /* Local IP address for talker */
#define GATEWAY_ADDR0   0,0,0,0                /* Gateway address (invalid if all 0s) */
#define SUBNET_MASK0    255,255,255,0          /* Subnet mask  */

TCPUDP_ENV tcpudp_env[] =
{
    {{MY_IP_ADDR0}, {SUBNET_MASK0}, {GATEWAY_ADDR0}},
};

/****************************************************************************/
/**********************     Driver-related definition     *******************/
/****************************************************************************/
/*--------------------------------------------------------------------------*/
/*    Set of Ethernet-related                                               */
/*--------------------------------------------------------------------------*/
/* Local MAC address (Set all 0s when unspecified) */

#define MY_MAC_ADDR0    0x74,0x90,0x50,0x00,0x79,0x04 /* Local IP address for talker */

UB _myethaddr[][6] =
{
    {MY_MAC_ADDR0},
};

/*--------------------------------------------------------------------------*/
/*    Set of PPP-related                                                    */
/*--------------------------------------------------------------------------*/
/* Authentication-related setting */
const UH ppp_auth  = AUTH_PAP;               /* PAP,No authentication enabled */
UB user_name[6]    = "abcde";                /* user name */
UB user_passwd[6]  = "abc00";                /* password  */

/* Dial up-related setting */
const UB peer_dial[]      = "0,123";                /* Destination telephone number */
const UB at_commands[]   = "ATW2S0=2&C0&D0&S0M0S10=255X3";    /* Modem initialization command */


/* Copyright 2014, RENESAS ELECTRONICS CORPORATION */

