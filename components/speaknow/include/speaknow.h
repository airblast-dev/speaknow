#ifndef RIGHTNOW2_COMPONENTS_RIGHTNOW2_SPEAKNOW_H
#define RIGHTNOW2_COMPONENTS_RIGHTNOW2_SPEAKNOW_H

#include "esp_now.h"
#include <stdalign.h>
#include <stdint.h>

enum SnVersion {
  SnV1,
};

#define SN_MAGIC_BYTES                                                         \
  { 0x68, 0x87, 0x00, 0x12, 0x01, 0x34, 0x82, 0xFF }
const static uint8_t SN_MAGIC[8] = SN_MAGIC_BYTES;

struct __attribute__((aligned(alignof(uint32_t)), packed)) SnMessage {
  uint32_t capacity;
  uint32_t data_len;
  const uint8_t magic[8];
  const enum SnVersion version;
  uint8_t data[] __attribute__((counted_by(data_len)));
};
_Static_assert(sizeof(struct SnMessage) == sizeof(uint32_t) * 2 +
                                               sizeof(uint8_t[8]) +
                                               sizeof(enum SnVersion),
               "bad speaknow message size");
_Static_assert(alignof(struct SnMessage) == alignof(uint32_t),
               "bad speaknow message alignment");

// The maximum number of bytes than can be sent in a single message
static const uint32_t SN_MAX_MESSAGE_LEN =
    ESP_NOW_MAX_DATA_LEN - sizeof(struct SnMessage);

// @brief Create a new message with the provided capacity
//
// @params
__attribute__((malloc)) struct SnMessage *sn_new_message(uint32_t capacity);

// @brief Returns a pointer to the portion of the message that will be sent
__attribute__((always_inline, nonnull(1),
               returns_nonnull)) static inline uint8_t *
sn_message_copy_point(const struct SnMessage *snm);
#endif
