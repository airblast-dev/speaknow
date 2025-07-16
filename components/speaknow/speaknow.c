#include "speaknow.h"
#include "esp_log.h"
#include "stdlib.h"
#include <inttypes.h>
#include <stdalign.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#define ESP_NOW_MAX_DATA_LEN_V2 1470
__attribute__((assume_aligned(alignof(void *)))) struct SnMessage *
sn_new_message(uint32_t capacity) {
  ESP_LOGI(__func__, "creating new message with %" PRIu32 " capacity",
           capacity);
  const static struct SnMessage EMPTY_MESSAGE = {
      .capacity = 0, .magic = SN_MAGIC_BYTES, .version = SnV1};
  uint32_t struct_capacity = sizeof(struct SnMessage) + capacity;
  if (struct_capacity < capacity ||
      struct_capacity < sizeof(struct SnMessage)) {
    ESP_LOGE(__func__, "requested message capacity exceeds max 32 bit integer");
    return NULL;
  }
  if (struct_capacity > ESP_NOW_MAX_DATA_LEN_V2) {
    ESP_LOGW(__func__, "requested capacity exceeds ESP NOW data length limit");
    return NULL;
  }
  void *mptr = aligned_alloc(alignof(void *), struct_capacity);
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

inline uint8_t *sn_message_copy_point(const struct SnMessage *snm) {
  return (uint8_t *)&snm->data_len;
}

uint8_t sn_new_message_builder(uint32_t capacity,
                               struct SnMessageBuilder *restrict builder) {
  static const struct SnMessageBuilder EMPTY_MESSAGE_BUILDER = {
      .message_count = 1, .total_data_len = 0, .messages = NULL};
  memcpy(builder, &EMPTY_MESSAGE_BUILDER, sizeof(struct SnMessageBuilder));

  if (SN_MAX_MESSAGE_LEN <= capacity) {
    builder->messages = sn_new_message(SN_MAX_MESSAGE_LEN);
    capacity -= SN_MAX_MESSAGE_LEN;
  } else {
    builder->messages = sn_new_message(capacity);
    capacity = 0;
  }

  struct SnMessage *cur = builder->messages;
  while (capacity >= SN_MAX_MESSAGE_LEN) {
    capacity -= SN_MAX_MESSAGE_LEN;
    ESP_LOGI(__func__, "allocating next message for builder");
    cur->next = sn_new_message(SN_MAX_MESSAGE_LEN);
    if (cur->next == NULL) {
      ESP_LOGE(__func__, "allocating next message for builder failed");
      goto error;
    }
    cur = cur->next;
  }

  if (capacity > 0) {
    cur->next = sn_new_message(capacity);
    if (cur->next == NULL) {
      ESP_LOGE(__func__, "allocating leftover bytes failed (%" PRIu32 ")",
               capacity);
      goto error;
    }
  }

  return 0;

error:
  cur = builder->messages;
  struct SnMessage *next;
  while (cur != NULL) {
    next = cur->next;
    free(cur);
    cur = next;
  }
  return -1;
}
