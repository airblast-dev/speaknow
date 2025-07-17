#ifndef RIGHTNOW2_COMPONENTS_RIGHTNOW2_SPEAKNOW_H
#define RIGHTNOW2_COMPONENTS_RIGHTNOW2_SPEAKNOW_H

#include <stdalign.h>
#include <stdint.h>

enum SnVersion {
  SnV1,
};

#define INTERNAL_SPEAKNOW_ESP_NOW_MAX_LEN_V2 1470
#define SN_MAGIC_BYTES                                                         \
  { 0x68, 0x87, 0x00, 0x12, 0x01, 0x34, 0x82, 0xFF }
const static uint8_t SN_MAGIC[8] = SN_MAGIC_BYTES;

/* START OF MESSAGE DEFS
 * START OF MESSAGE DEFS
 * START OF MESSAGE DEFS
 * START OF MESSAGE DEFS
 */

struct __attribute__((packed)) SnMessage {
  struct SnMessage *next;
  uint32_t capacity;
  uint32_t data_len;
  const uint8_t magic[8];
  const enum SnVersion version;
  uint8_t data[] __attribute__((counted_by(data_len)));
};
_Static_assert(sizeof(struct SnMessage) ==
                   sizeof(uint32_t) * 2 + sizeof(uint8_t[8]) +
                       sizeof(enum SnVersion) + sizeof(struct SnMessage *),
               "bad speaknow message size");
// TODO fix alignment stuff
_Static_assert(alignof(struct SnMessage) == 1,
               "bad speaknow message alignment");

// The maximum number of bytes than can be sent in a single message
static const uint32_t SN_MAX_MESSAGE_LEN =
    INTERNAL_SPEAKNOW_ESP_NOW_MAX_LEN_V2 - sizeof(struct SnMessage);

// @brief Create a new message with the provided capacity
//
// @params
__attribute__((malloc,
               assume_aligned(alignof(struct SnMessage *)))) struct SnMessage *
sn_message_new(uint32_t capacity);
__attribute__((nonnull(1))) void
sn_message_destroy_chain(struct SnMessage *restrict snm);
__attribute__((nonnull(1), always_inline)) void
sn_message_dealloc_current(struct SnMessage **restrict snm);

// @brief Returns a pointer to the portion of the message that will be sent
__attribute__((always_inline, nonnull(1),
               returns_nonnull)) static inline uint8_t *
sn_message_copy_point(const struct SnMessage *snm);
inline uint8_t *sn_message_copy_point(const struct SnMessage *snm) {
  return (uint8_t *)&snm->data_len;
}

/* END OF MESSAGE DEFS
 * END OF MESSAGE DEFS
 * END OF MESSAGE DEFS
 * END OF MESSAGE DEFS
 */

struct SnMessageBuilder {
  // Singly linked list of messages
  struct SnMessage *messages;
  uint32_t total_data_len;
  uint16_t message_count;
};

__attribute__((access(write_only, 2), nonnull(2))) uint8_t
sn_new_message_builder(uint32_t capacity,
                       struct SnMessageBuilder *restrict builder);

#undef INTERNAL_SPEAKNOW_ESP_NOW_MAX_LEN_V2
#endif
