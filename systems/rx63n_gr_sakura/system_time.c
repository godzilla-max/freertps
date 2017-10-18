#include "r_sys_time_rx_if.h"
#include "freertps/time.h"

fr_time_t fr_time_now(void)
{
  SYS_TIME sys_time;
  fr_time_t now;

  /* FIXME   For now, the resolution is limited to 1 second. */
  R_SYS_TIME_GetCurrentTime(&sys_time);
  now.seconds = sys_time.unix_time;
  now.fraction = 0;

  return now;
}
