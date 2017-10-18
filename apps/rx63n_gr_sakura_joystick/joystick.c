/*******************************************************************************
Overview:

This node publishes sensor_msgs::msg::Joy containing only x-axis and y-axis values.
The range of the x-axis and y-axis values are 0 to 1024 and the center value is 512.
In order to avoid unsteadibility, the value near the center value (within 5%) shall
be set to 512. 

Joystick value(Full scale 1024)(x,y)
x
|(0,1024)       (512,1024)      (1024,1024)
|                v center value
|(0,512)        (512,512)       (1024,512)
|
| (0,0)          (512,0)         (1024,0)
|___________________________________________ y

This node uses a 12-bit AD converter. For a 12-bit AD converter the range of the
AD value is 0 to 4096. So the AD value which is converted by AD converter shall be
divided by 4 to scale 0-1024(publish values).
*******************************************************************************/

#include <stdio.h>
#include <string.h>
#include "freertps/freertps.h"
#include "mcu/rx63n/register_access/iodefine.h"

/* Joystick reference center value */
#define JOY_REFERENCE_CENTER_VALUE  (2048.0F)
/* Joystick center high threshold. 2048 * 5% = 102.4 */
#define JOY_CENTER_HIGH_THRESHOLD   (2048.0F + 102.0F)
/* Joystick center low threshold. 2048 * 5% = 102.4 */
#define JOY_CENTER_LOW_THRESHOLD    (2048.0F - 102.0F)

static uint32_t msg[32] = {0};
static float joy_x = 0.0F;
static float joy_y = 0.0F;

static void adc_init(void)
{
  SYSTEM.PRCR.WORD = 0xA503;

  /* 12bitADC module stop */
  MSTP_S12AD = 0;

  /* Select the input function(b0:b1 = 00) */
  PORT4.PDR.BYTE &= 0xFC;
  PORT4.PMR.BYTE &= 0xFC;
  MPC.PWPR.BYTE = 0x40;

  /* Select the analog function(b7 = 1) */
  MPC.P40PFS.BYTE |= 0x80;
  MPC.P41PFS.BYTE |= 0x80;
  MPC.PWPR.BYTE = 0x80;

  /* Select the PCLK Clock(b2:b3 = 11) */
  S12AD.ADCSR.BYTE |= 0x0C;

  /* Select AN000 & AN001 converters(b0:b1 = 11) */
  S12AD.ADANS0.WORD |= 0x0003;
  SYSTEM.PRCR.WORD = 0xA500;

  /* Start ADC */
  S12AD.ADCSR.BIT.ADST = 1;
}

static void set_joy_data(void)
{
  /* Wait until ADC is completed */
  while(1 == S12AD.ADCSR.BIT.ADST) {
    /* DO NOTHING */
  }

  joy_x = (float)S12AD.ADDR0;
  joy_y = (float)S12AD.ADDR1;

  /* Joystick values set reference center value */
  if((JOY_CENTER_LOW_THRESHOLD <= joy_x) &&
     (JOY_CENTER_HIGH_THRESHOLD >= joy_x)) {
    joy_x = JOY_REFERENCE_CENTER_VALUE;
  }
  if((JOY_CENTER_LOW_THRESHOLD <= joy_y) &&
     (JOY_CENTER_HIGH_THRESHOLD >= joy_y)) {
    joy_y = JOY_REFERENCE_CENTER_VALUE;
  }

  /* Divided by 4 to convert 0-4096(12-bit ADC values) to 0-1024(publish values). */
  joy_x = joy_x / 4.0F;
  joy_y = joy_y / 4.0F;

  /* Set serialized data(sensor_msgs::msg::Joy)

  Raw message difinition:
  Header header 
  float32[] axes  #the axes measurements from a joystick
  int32[] buttons #the buttons measurements from a joystick
  
  Serialized data:
  WORD     Value
  ---------------
  WORD1    0(header/stamp.sec)
  WORD2    0(header/stamp.nanosec)
  WORD3    0x02(header/frame_id)
  WORD4    0x30(header/frame_id)
  WORD5    2(number of axes)
  WORD6    joy_x(axis)
  WORD7    joy_y(axis)
  WORD8    0(buttons) */
  msg[2] = 0x02;
  msg[3] = 0x30;
  msg[4] = 2;
  memcpy((void *)&msg[5], (void *)&joy_x, sizeof(joy_x));
  memcpy((void *)&msg[6], (void *)&joy_y, sizeof(joy_y)); 
  printf("sending: [joy_x:%f][joy_y:%f]\r\n", msg[5], msg[6]);

  /* Start ADC */
  S12AD.ADCSR.BIT.ADST = 1;
}

int main(int argc, char **argv)
{
  printf("Joystick driver\r\n");
  adc_init();
  freertps_system_init();
  frudp_pub_t *pub = freertps_create_pub("joystick",
                                         "sensor_msgs::msg::dds_::Joy_");
  frudp_disco_start();
  while (freertps_system_ok())
  {
    frudp_listen(0);
    frudp_disco_tick();
    set_joy_data();
    freertps_publish(pub, (const uint8_t *)msg, sizeof(msg));
  }
  frudp_fini();
  return 0;
}