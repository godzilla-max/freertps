#ifndef FREERTPS_LOCATOR_H
#define FREERTPS_LOCATOR_H

#include <stdint.h>

#define FRUDP_LOCATOR_KIND_INVALID -1
#define FRUDP_LOCATOR_KIND_RESERVED 0
#define FRUDP_LOCATOR_KIND_UDPV4    1
#define FRUDP_LOCATOR_KIND_UDPV6    2

#ifdef __GNUC__
typedef struct
{
  int32_t kind;
  uint32_t port;
  union
  {
    uint8_t raw[16];
    struct
    {
      uint8_t zeros[12];
      uint32_t addr;
    } udp4;
  } addr; 
} __attribute__((packed)) frudp_locator_t;
#elif __RX == 1
#pragma pack
typedef struct
{
  int32_t kind;
  uint32_t port;
  union
  {
    uint8_t raw[16];
    struct
    {
      uint8_t zeros[12];
      uint32_t addr;
    } udp4;
  } addr; 
} frudp_locator_t;
#pragma unpack
#endif /* __RX == 1 */

#endif
