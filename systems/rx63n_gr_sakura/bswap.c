#include "freertps/bswap.h"

static uint32_t _bswap32(uint32_t u)
{
  return (((((unsigned long)(u) & 0x000000FF)) << 24) |
          ((((unsigned long)(u) & 0x0000FF00)) << 8) |
          ((((unsigned long)(u) & 0x00FF0000)) >> 8) |
          ((((unsigned long)(u) & 0xFF000000)) >> 24));
}

static uint16_t _bswap16(uint16_t u)
{
  return (((((unsigned short)(u) & 0x00FF)) << 8) |
          (((unsigned short)(u) & 0xFF00) >> 8));
}

uint32_t freertps_htonl(uint32_t u)
{
  return _bswap32(u);
}

uint16_t freertps_htons(uint16_t u)
{
  return _bswap16(u);
}

uint32_t freertps_ntohl(uint32_t u)
{
  return _bswap32(u);
}

uint16_t freertps_ntohs(uint16_t u)
{
  return _bswap16(u);
}
