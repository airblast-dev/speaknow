#include "speaknow.h"
#include "unity.h"

TEST_CASE("create_message_zero_capacity",
          "create a new speaknow message with zero space allocated for data") {
  struct SnMessage *msg = sn_new_message(0);
  TEST_ASSERT_EQUAL_UINT8_ARRAY_MESSAGE(SN_MAGIC, msg->magic, sizeof(SN_MAGIC),
                                        "incorrect magic bytes");
  TEST_ASSERT_EQUAL_UINT8_MESSAGE(0, msg->capacity, "incorrect capacity");
  TEST_ASSERT_EQUAL_UINT8_MESSAGE(0, msg->data_len, "incorrect len");
  free(msg);
}

TEST_CASE("create_message_non_zero_capacity",
          "create a new speaknow message with space for data") {
  struct SnMessage *msg = sn_new_message(12);
  TEST_ASSERT_EQUAL_UINT8_ARRAY_MESSAGE(SN_MAGIC, msg->magic, sizeof(SN_MAGIC),
                                        "incorrect magic bytes");
  TEST_ASSERT_EQUAL_UINT8_MESSAGE(12, msg->capacity, "incorrect capacity");
  TEST_ASSERT_EQUAL_UINT8_MESSAGE(0, msg->data_len, "incorrect len");
  free(msg);
}

TEST_CASE("create_message_capacity_overflow",
          "create a new speaknow message with its capacity overflowing or "
          "malloc failing") {
  struct SnMessage *msg = sn_new_message(UINT32_MAX - 1);
  TEST_ASSERT_NULL(msg);
}

TEST_CASE("create_message_builder_zero_capacity",
          "create a zero capacity message builder") {
  struct SnMessageBuilder b;
  TEST_ASSERT_EQUAL_UINT8_MESSAGE(0, sn_new_message_builder(0, &b),
                                  "failed to allocate space for message");
  TEST_ASSERT_NULL(b.messages->next);
}
