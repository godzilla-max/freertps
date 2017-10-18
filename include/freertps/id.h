#ifndef FRUDP_ID_H
#define FRUDP_ID_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __GNUC__
typedef union
{
  struct
  {
    uint8_t key[3];
    uint8_t kind;
  } s;
  uint32_t u;
} __attribute__((packed)) frudp_eid_t; // entity ID
#elif __RX == 1
#pragma pack
typedef union
{
  struct
  {
    uint8_t key[3];
    uint8_t kind;
  } s;
  uint32_t u;
} frudp_eid_t; // entity ID
#pragma unpack
#endif /* __RX == 1 */

#define FRUDP_ENTITY_KIND_USER_WRITER_WITH_KEY 0x02
#define FRUDP_ENTITY_KIND_USER_WRITER_NO_KEY   0x03
#define FRUDP_ENTITY_KIND_USER_READER_NO_KEY   0x04
#define FRUDP_ENTITY_KIND_USER_READER_WITH_KEY 0x07

extern const frudp_eid_t g_frudp_eid_unknown;

#define FRUDP_GUID_PREFIX_LEN 12
typedef struct
{
  uint8_t prefix[FRUDP_GUID_PREFIX_LEN];
} frudp_guid_prefix_t;

#ifdef __GNUC__
typedef struct
{
  frudp_guid_prefix_t prefix;
  frudp_eid_t eid;
} __attribute__((packed)) frudp_guid_t;
#elif __RX == 1
#pragma pack
typedef struct
{
  frudp_guid_prefix_t prefix;
  frudp_eid_t eid;
} frudp_guid_t;
#pragma unpack
#endif /* __RX == 1 */
extern const frudp_guid_t g_frudp_guid_unknown;

bool frudp_guid_prefix_identical(frudp_guid_prefix_t * const a,
                                 frudp_guid_prefix_t * const b);

bool frudp_guid_identical(const frudp_guid_t * const a,
                          const frudp_guid_t * const b);

void frudp_stuff_guid(frudp_guid_t *guid,
                      const frudp_guid_prefix_t *prefix,
                      const frudp_eid_t *id);

/////////////////////////////////////////////////////////////////////////
// VENDOR ID STUFF
// for now let's pretend that our vendor ID is 11311 in hex
#define FREERTPS_VENDOR_ID 0x2C2F
typedef uint16_t frudp_vid_t;
const char *frudp_vendor(const frudp_vid_t vid);

void frudp_print_guid_prefix(const frudp_guid_prefix_t *guid_prefix);
void frudp_print_guid(const frudp_guid_t *guid);

frudp_eid_t frudp_create_user_id(const uint8_t entity_kind);

#endif
