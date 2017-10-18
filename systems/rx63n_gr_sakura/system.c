#include "r_sys_time_rx_if.h"
#include "freertps/freertps.h"
#include "freertps/system.h"
#include "freertps/psm.h"

struct rtps_psm g_rtps_psm;

void freertps_system_init(void)
{
  sys_time_err_t systime_ercd;

  /* Start system timer module */
  systime_ercd = R_SYS_TIME_Open();
  if (systime_ercd != SYS_TIME_SUCCESS)
  {
    while ((bool)1);
  }

  /* Initialize PSM part */
  g_rtps_psm = g_rtps_psm_udp;
  g_rtps_psm.init();
  g_freertps_init_complete = true;
}

bool freertps_system_ok(void)
{
  return true;
}
