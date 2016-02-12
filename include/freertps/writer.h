#ifndef FREERTPS_WRITER_H
#define FREERTPS_WRITER_H

#include <stdbool.h>
#include "freertps/endpoint.h"
#include "freertps/history_cache.h"
#include "freertps/sequence_number.h"
#include "freertps/container.h"
#include "freertps/reader_locator.h"
#include "freertps/reader_proxy.h"
#include "freertps/duration.h"
#include "freertps/time.h"

typedef struct fr_writer
{
  struct fr_endpoint endpoint;
  bool push_mode;
  struct fr_duration heartbeat_period;
  struct fr_duration nack_response_delay;
  struct fr_duration nack_suppression_duration;
  struct fr_duration resend_data_period;
  fr_sequence_number_t last_change_sequence_number;
  struct fr_container *matched_readers; // for stateful writing
  struct fr_container *reader_locators; // for stateless writing
  struct fr_history_cache writer_cache; 
  //////////////////////////////////////////////////////////////
  char *topic_name;
  char *type_name;
} fr_writer_t;

#define FR_WRITER_TYPE_BEST_EFFORT 0
#define FR_WRITER_TYPE_RELIABLE    1
fr_writer_t *fr_writer_create(const char *topic_name, const char *type_name,
    const uint32_t type);
void fr_writer_destroy(fr_writer_t *w);
//void fr_writer_add_reader_proxy(fr_reader_proxy_t *rp);
bool fr_writer_new_change(struct fr_writer *w, void *data, size_t data_len,
    void *handle, uint32_t handle_len);
fr_rc_t fr_writer_add_reader_locator(struct fr_writer *w,
    struct fr_reader_locator *reader_loc);
void fr_writer_unsent_changes_reset(struct fr_writer *w);
void fr_writer_send_changes(struct fr_writer *w);

#endif
