#include "speaknow.h"
#include "unity.h"
#include <string.h>

TEST_CASE("create_message_zero_capacity",
          "create a new speaknow message with zero space allocated for data") {
  struct SnMessage *msg = sn_message_new(0);
  TEST_ASSERT_EQUAL_UINT8_ARRAY_MESSAGE(SN_MAGIC, msg->magic, sizeof(SN_MAGIC),
                                        "incorrect magic bytes");
  TEST_ASSERT_EQUAL_UINT8_MESSAGE(0, msg->capacity, "incorrect capacity");
  TEST_ASSERT_EQUAL_UINT8_MESSAGE(0, msg->data_len, "incorrect len");
  free(msg);
}

TEST_CASE("create_message_non_zero_capacity",
          "create a new speaknow message with space for data") {
  struct SnMessage *msg = sn_message_new(12);
  TEST_ASSERT_EQUAL_UINT8_ARRAY_MESSAGE(SN_MAGIC, msg->magic, sizeof(SN_MAGIC),
                                        "incorrect magic bytes");
  TEST_ASSERT_EQUAL_UINT8_MESSAGE(12, msg->capacity, "incorrect capacity");
  TEST_ASSERT_EQUAL_UINT8_MESSAGE(0, msg->data_len, "incorrect len");
  free(msg);
}

TEST_CASE("create_message_capacity_overflow",
          "create a new speaknow message with its capacity overflowing or "
          "malloc failing") {
  struct SnMessage *msg = sn_message_new(UINT32_MAX - 1);
  TEST_ASSERT_NULL(msg);
}

TEST_CASE("create_message_builder_zero_capacity",
          "create a zero capacity message builder") {
  struct SnMessageBuilder b;
  TEST_ASSERT_EQUAL_UINT8_MESSAGE(0, sn_new_message_builder(0, &b),
                                  "failed to allocate space for message");
  TEST_ASSERT_NOT_NULL(b.messages);
  TEST_ASSERT_NULL(b.messages->next);
  TEST_ASSERT_EQUAL_UINT32(0, b.messages->data_len);
  TEST_ASSERT_EQUAL_UINT32(0, b.messages->capacity);
}

TEST_CASE("create_message_builder_under_limit_capacity",
          "create a message builder with capacity slightly less than the "
          "maximum for a single message") {
  struct SnMessageBuilder b;
  TEST_ASSERT_EQUAL_UINT8_MESSAGE(0, sn_new_message_builder(900, &b),
                                  "failed to allocate space for message");
  TEST_ASSERT_NOT_NULL(b.messages);
  TEST_ASSERT_EQUAL_UINT32(900, b.messages->capacity);
  memset(b.messages->data, 0, b.messages->data_len);
  TEST_ASSERT_EQUAL_UINT32(0, b.messages->data_len);
}

TEST_CASE("create_message_builder_over_limit_capacity",
          "create message builder with larger capacity than the maximum for a "
          "single message") {
  struct SnMessageBuilder b;
  TEST_ASSERT_EQUAL_UINT8_MESSAGE(
      0, sn_new_message_builder(SN_MAX_MESSAGE_LEN * 3, &b),
      "failed to allocate space for message");

  TEST_ASSERT_NOT_NULL(b.messages);
  TEST_ASSERT_EQUAL_UINT32(SN_MAX_MESSAGE_LEN, b.messages->capacity);
  TEST_ASSERT_EQUAL_UINT32(0, b.messages->data_len);
  TEST_ASSERT_NOT_NULL(b.messages->next);
  TEST_ASSERT_EQUAL_UINT32(SN_MAX_MESSAGE_LEN, b.messages->next->capacity);
  TEST_ASSERT_EQUAL_UINT32(0, b.messages->next->data_len);
  TEST_ASSERT_NOT_NULL(b.messages->next->next);
  TEST_ASSERT_EQUAL_UINT32(SN_MAX_MESSAGE_LEN,
                           b.messages->next->next->capacity);
  TEST_ASSERT_EQUAL_UINT32(0, b.messages->next->next->data_len);
}
