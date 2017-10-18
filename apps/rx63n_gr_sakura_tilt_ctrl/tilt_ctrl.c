/*******************************************************************************
Overview:

This node subscribes sensor_msgs::msg::Joy containing only x-axis and y-axis values 
and sends the x-axis and y-axis servo motors position to tilt controller via serial
interface(UART). The range of the servo motors position is 1830 to 2266.

Servo motors position to move target angle(x,y)
x
|
|(1830,2266)    (2048,2266)    (2266,2266)
|(1830,2048)    (2048,2048)    (2266,2048)
|(1830,1830)    (2048,1830)    (2266,1830)
|___________________________________________ y

And the range of the x-axis and y-axis servo motors angle from the center is -20
to +20. This value corresponds to above servo motors position.

Servo motors angle from the center(x,y)
x
|(-20,+20)      (0,+20)       (+20,+20)
|(-20,0)         (0,0)         (+20,0)
|(-20,-20)      (0,-20)       (+20,-20)
|___________________________________________ y

Joystick values shall be converted to servo motors target angle. For getting the
x-axis target angle, this formula shall be used.
x-axis target angle = 2048 + (x-axis value - 512) / (512 / 218)
And also, for getting the y-axis target angle, this formula shall be used. To adjust
axis movement, Y direction shall be set in reverse.
y-axis target angle = 4096 - 2048 + (y-axis value - 512) / (512 / 218)


UART command:

To start servo output, this node sends the next command.
Byte     Value
---------------
Byte1    0xFF
Byte2    0xFF 
Byte3    0x00
Byte4    0x03
Byte5    0x53 
Byte6    0x41

To stop servo output, this node sends the next command.
Byte     Value
---------------
Byte1    0xFF
Byte2    0xFF 
Byte3    0x00
Byte4    0x03
Byte5    0x53 
Byte6    0x4F

To move to target angle, this node sends the next command.
Byte     Value
---------------
Byte1    0xFF
Byte2    0xFF 
Byte3    Servo ID(0x01 or 0x02)
Byte4    0x1E
Byte5    Target angle(unsigned short/lower 1 byte) 
Byte6    Target angle(unsigned short/upper 1 byte)
*******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <machine.h>
#include "../../uart/include/sci.h"
#include "../../uart/include/init_uart.h"
#include "freertps/freertps.h"

/* Tilt minimum value */
#define TILT_VALUE_MIN          (1830)
/* Tilt maximum value */
#define TILT_VALUE_MAX          (2266)
/* Delay of the sending data */
#define SEND_DATA_DELAY         (150000)
/* Offset 20(= joy_x value is the element 5 * 4 byte) */
#define OFFSET_JOY_X            (20)
/* Offset 24(= joy_y value is the element 6 * 4 byte) */
#define OFFSET_JOY_Y            (24)
/* Servo(x-axis) id*/
#define SERVO_X_AXIS            (1)
/* Servo(y-axis) id*/
#define SERVO_Y_AXIS            (2)

static uint8_t tx_buf[6] = {0};
static float joy_x = 0.0F;
static float joy_y = 0.0F;

void chatter_cb(const void *msg)
{
  uint8_t *buf = (uint8_t *)msg;

  memcpy((void *)&joy_x, (void *)&buf[OFFSET_JOY_X], sizeof(float));
  memcpy((void *)&joy_y, (void *)&buf[OFFSET_JOY_Y], sizeof(float));
  printf("receiving: [joy_x:%f] / [joy_y:%f]\n", joy_x, joy_y);
}

static void send_servo_output(void)
{ 
  /* Enable transmission */
  SCI_B_TX_BUSY = 1;

  /* Disable maskable interrupts */
  clrpsw_i();

  /* Wait until the transmission is completed */
  while(0 != SCI_B_TX_BUSY) {
    SCI_Transmit(SCI_CH0, tx_buf, sizeof(tx_buf));
  }

  /* Enable maskable interrupts */
  setpsw_i();

  for (volatile uint32_t i = 0; SEND_DATA_DELAY > i; i++)
  {
    /* Wait at least 20msec */
  }
}

static void send_start_command(void)
{
  tx_buf[0] = 0xFF;
  tx_buf[1] = 0xFF;
  tx_buf[2] = 0x00;
  tx_buf[3] = 0x03;
  tx_buf[4] = 0x53;
  tx_buf[5] = 0x41;
  send_servo_output();
}

static void send_stop_command(void)
{
  tx_buf[0] = 0xFF;
  tx_buf[1] = 0xFF;
  tx_buf[2] = 0x00;
  tx_buf[3] = 0x03;
  tx_buf[4] = 0x53;
  tx_buf[5] = 0x4F;
  send_servo_output();
}

static void send_servo_position(void)
{
  static int servo_axis = SERVO_X_AXIS;
  static unsigned short angle_x = 0, angle_y = 0;

  if (SERVO_X_AXIS == servo_axis) {
    angle_x = (unsigned short)(2048.0F + ((joy_x - 512.0F) / (512.0F / 218.0F)));

    /* Guards joystick value */
    if(TILT_VALUE_MIN > angle_x) {
      angle_x = TILT_VALUE_MIN;
    } else if (TILT_VALUE_MAX < angle_x) {
      angle_x = TILT_VALUE_MAX;
    }
    tx_buf[2] = servo_axis;
    tx_buf[4] = (uint8_t)(angle_x & 0xFF);
    tx_buf[5] = (uint8_t)((angle_x & 0xFF00) >> 8);
    servo_axis = SERVO_Y_AXIS;
  } else if (SERVO_Y_AXIS == servo_axis) {
    angle_y = 4096 - (unsigned short)(2048.0F + ((joy_y - 512.0F) / (512.0F / 218.0F)));

    /* Guards joystick value */
    if(TILT_VALUE_MIN > angle_y) {
      angle_y = TILT_VALUE_MIN;
    } else if (TILT_VALUE_MAX < angle_y) {
      angle_y = TILT_VALUE_MAX;   
    }
    tx_buf[2] = servo_axis;
    tx_buf[4] = (uint8_t)(angle_y & 0xFF);
    tx_buf[5] = (uint8_t)((angle_y & 0xFF00) >> 8);
    servo_axis = SERVO_X_AXIS;
  }
  printf("sending: [angle_x:%d][angle_y:%d]\r\n", angle_x, angle_y);
  tx_buf[0] = 0xFF;
  tx_buf[1] = 0xFF;
  tx_buf[3] = 0x1E;
  send_servo_output();
}

int main(int argc, char **argv)
{
  printf("Tilt controller\r\n");
  init_uart();
  freertps_system_init();
  freertps_create_sub("joystick", 
                      "sensor_msgs::msg::dds_::Joy_",
                      chatter_cb);
  /* We're alive now; announce ourselves to the world */
  frudp_disco_start();

  send_start_command();
  while (freertps_system_ok())
  {
    frudp_listen(0);
    frudp_disco_tick();
    send_servo_position();
  }
  frudp_fini();
  return 0;
}