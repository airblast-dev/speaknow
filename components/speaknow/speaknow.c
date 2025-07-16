#include "speaknow.h"
#include "esp_log.h"
#include "stdlib.h"
#include <inttypes.h>
#include <stdalign.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

struct SnMessage *sn_new_message(uint32_t capacity) {
  ESP_LOGI(__func__, "creating new message");
  const static struct SnMessage EMPTY_MESSAGE = {
      .capacity = 0, .magic = SN_MAGIC_BYTES, .version = SnV1};
  uint32_t struct_capacity = sizeof(struct SnMessage) + capacity;
  if (struct_capacity < capacity || struct_capacity < sizeof(struct SnMessage)) {
    ESP_LOGE(__func__, "requested message capacity exceeds max 32 bit integer");
    return NULL;
  }
  if (struct_capacity > SN_MAX_MESSAGE_LEN) {
    ESP_LOGW(__func__, "requested capacity exceeds");
    return NULL;
  }
  void *mptr = aligned_alloc(alignof(uint32_t), struct_capacity);
  if (mptr == NULL) {
    ESP_LOGE(__func__,
             "failed to allocate space for new message with %" PRIu32
             " capacity",
             capacity);
    return NULL;
  }

  memcpy(mptr, &EMPTY_MESSAGE, sizeof(struct SnMessage));
  struct SnMessage *snm_ptr = mptr;
  snm_ptr->capacity = capacity;
  return snm_ptr;
}

uint8_t *sn_message_copy_point(const struct SnMessage *snm) {
  return (uint8_t *)&snm->data_len;
}
