#include "esp_now.h"
#include <stdint.h>

enum SnVersion {
  SnV1,
};

#define SN_MAGIC_BYTES                                                         \
  { 0x68, 0x87, 0x00, 0x12, 0x01, 0x34, 0x82, 0xFF }
const static uint8_t SN_MAGIC[8] = SN_MAGIC_BYTES;

struct SnMessage {
  uint32_t capacity;
  const uint8_t magic[8];
  const enum SnVersion version;
  uint32_t data_len;
  uint8_t data[] __attribute__((counted_by(data_len)));
};

// The maximum number of bytes than can be sent in a single message
static const uint32_t SN_MAX_MESSAGE_LEN =
    ESP_NOW_MAX_DATA_LEN - sizeof(struct SnMessage);

__attribute__((malloc)) struct SnMessage *sn_new_message(uint32_t capacity);

__attribute__((always_inline)) static inline uint8_t *
sn_message_copy_point(const struct SnMessage *snm);
