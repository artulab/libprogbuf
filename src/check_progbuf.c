#include <check.h>
#include <stdlib.h>

#include "common.h"
#include "progbuf.h"

START_TEST (test_progbuf_alloc_version)
{
  int ret;
  long message_tag;

  ret = progbuf_get_message_tag (NULL, &message_tag);
  ck_assert (ret == PROGBUF_ERROR_NULL_PARAM);

  progbuf_h buf = progbuf_alloc (1);

  ck_assert (buf);

  ret = progbuf_get_message_tag (buf, NULL);
  ck_assert (ret == PROGBUF_ERROR_NULL_PARAM);

  ret = progbuf_get_message_tag (buf, &message_tag);

  ck_assert (ret == PROGBUF_SUCCESS);
  ck_assert (message_tag == 1);

  size_t size;
  ret = progbuf_get_message_buffer (buf, NULL, &size);

  ck_assert (ret == PROGBUF_SUCCESS);

  ck_assert (size == 1);

  ret = progbuf_free (buf);
  ck_assert (ret == PROGBUF_SUCCESS);
}
END_TEST

void
parametric_test_progbuf_write_read_long (const long val,
                                         const int expected_val_size)
{
  progbuf_h buf = progbuf_alloc (1);

  ck_assert (buf);

  long p_val = val;

  int ret = progbuf_set_long (buf, p_val);

  ck_assert (ret == PROGBUF_SUCCESS);

  size_t size;
  ret = progbuf_get_message_buffer (buf, NULL, &size);

  ck_assert (ret == PROGBUF_SUCCESS);

  ck_assert (size == 1 + 1 + expected_val_size);

  progbuf_it_h iter = progbuf_iter_alloc (buf);

  ck_assert (iter);

  ret = progbuf_get_long (iter, &p_val);

  ck_assert (ret == PROGBUF_SUCCESS);
  ck_assert (val == p_val);

  struct progbuf_it_s *iter_internal = iter;

  ck_assert (iter_internal->read_pos == 1 + 1 + expected_val_size);

  ret = progbuf_iter_free (iter);
  ck_assert (ret == PROGBUF_SUCCESS);

  ret = progbuf_free (buf);
  ck_assert (ret == PROGBUF_SUCCESS);
}

START_TEST (test_progbuf_write_read_positive_long)
{
  parametric_test_progbuf_write_read_long (0, 1);
  parametric_test_progbuf_write_read_long (1, 1);
  parametric_test_progbuf_write_read_long (0x3F, 1);

  parametric_test_progbuf_write_read_long (0x7F, 2);
  parametric_test_progbuf_write_read_long (0x1FFF, 2);

  parametric_test_progbuf_write_read_long (0x3FFF, 3);
  parametric_test_progbuf_write_read_long (0xFFFFF, 3);

  parametric_test_progbuf_write_read_long (0x1FFFFF, 4);
  parametric_test_progbuf_write_read_long (0x7FFFFFF, 4);

  parametric_test_progbuf_write_read_long (0xFFFFFFF, 5);
  parametric_test_progbuf_write_read_long (0x7FFFFFFF, 5);
}
END_TEST

START_TEST (test_progbuf_write_read_negative_long)
{
  parametric_test_progbuf_write_read_long (-1, 1);
  parametric_test_progbuf_write_read_long (-0x3F, 1);

  parametric_test_progbuf_write_read_long (-0x7F, 2);
  parametric_test_progbuf_write_read_long (-0x1FFF, 2);

  parametric_test_progbuf_write_read_long (-0x3FFF, 3);
  parametric_test_progbuf_write_read_long (-0xFFFFF, 3);

  parametric_test_progbuf_write_read_long (-0x1FFFFF, 4);
  parametric_test_progbuf_write_read_long (-0x7FFFFFF, 4);

  parametric_test_progbuf_write_read_long (-0xFFFFFFF, 5);
  parametric_test_progbuf_write_read_long (-0x7FFFFFFF, 5);
}
END_TEST

START_TEST (test_progbuf_write_read_ulong)
{
  progbuf_h buf = progbuf_alloc (1);

  ck_assert (buf);

  unsigned long val = 0xFFFFFFFF;
  unsigned long p_val = val;

  int ret = progbuf_set_ulong (buf, p_val);

  ck_assert (ret == PROGBUF_SUCCESS);

  size_t size;

  ret = progbuf_get_message_buffer (NULL, NULL, NULL);
  ck_assert (ret == PROGBUF_ERROR_NULL_PARAM);

  ret = progbuf_get_message_buffer (NULL, NULL, &size);
  ck_assert (ret == PROGBUF_ERROR_NULL_PARAM);

  ret = progbuf_get_message_buffer (buf, NULL, NULL);
  ck_assert (ret == PROGBUF_ERROR_NULL_PARAM);

  ret = progbuf_get_message_buffer (buf, NULL, &size);
  ck_assert (ret == PROGBUF_SUCCESS);

  ck_assert (size == 1 + 6);

  progbuf_it_h iter = progbuf_iter_alloc (buf);

  ck_assert (iter);

  ret = progbuf_get_ulong (iter, &p_val);

  ck_assert (ret == PROGBUF_SUCCESS);
  ck_assert (val == p_val);

  struct progbuf_it_s *iter_internal = iter;

  ck_assert (iter_internal->read_pos == 1 + 6);

  ret = progbuf_iter_free (iter);
  ck_assert (ret == PROGBUF_SUCCESS);

  ret = progbuf_free (buf);
  ck_assert (ret == PROGBUF_SUCCESS);
}
END_TEST

void
parametric_test_progbuf_write_read_longlong (const long long val,
                                             const int expected_val_size)
{
  progbuf_h buf = progbuf_alloc (1);

  ck_assert (buf);

  long long p_val = val;

  int ret = progbuf_set_longlong (buf, p_val);

  ck_assert (ret == PROGBUF_SUCCESS);

  size_t size;
  ret = progbuf_get_message_buffer (buf, NULL, &size);

  ck_assert (ret == PROGBUF_SUCCESS);

  ck_assert (size == 1 + 1 + expected_val_size);

  progbuf_it_h iter = progbuf_iter_alloc (buf);

  ck_assert (iter);

  ret = progbuf_get_longlong (iter, &p_val);

  ck_assert (ret == PROGBUF_SUCCESS);
  ck_assert (val == p_val);

  struct progbuf_it_s *iter_internal = iter;

  ck_assert (iter_internal->read_pos == 1 + 1 + expected_val_size);

  ret = progbuf_iter_free (iter);
  ck_assert (ret == PROGBUF_SUCCESS);

  ret = progbuf_free (buf);
  ck_assert (ret == PROGBUF_SUCCESS);
}

START_TEST (test_progbuf_write_read_positive_longlong)
{
  parametric_test_progbuf_write_read_longlong (0, 1);
  parametric_test_progbuf_write_read_longlong (1, 1);
  parametric_test_progbuf_write_read_longlong (0x3F, 1);

  parametric_test_progbuf_write_read_longlong (0x7F, 2);
  parametric_test_progbuf_write_read_longlong (0x1FFF, 2);

  parametric_test_progbuf_write_read_longlong (0x3FFF, 3);
  parametric_test_progbuf_write_read_longlong (0xFFFFF, 3);

  parametric_test_progbuf_write_read_longlong (0x1FFFFF, 4);
  parametric_test_progbuf_write_read_longlong (0x7FFFFFF, 4);

  parametric_test_progbuf_write_read_longlong (0xFFFFFFF, 5);
  parametric_test_progbuf_write_read_longlong (0x3FFFFFFFF, 5);

  parametric_test_progbuf_write_read_longlong (0x7FFFFFFFF, 6);
  parametric_test_progbuf_write_read_longlong (0x1FFFFFFFFFF, 6);

  parametric_test_progbuf_write_read_longlong (0x3FFFFFFFFFF, 7);
  parametric_test_progbuf_write_read_longlong (0xFFFFFFFFFFFF, 7);

  parametric_test_progbuf_write_read_longlong (0x1FFFFFFFFFFFF, 8);
  parametric_test_progbuf_write_read_longlong (0x7FFFFFFFFFFFFF, 8);

  parametric_test_progbuf_write_read_longlong (0xFFFFFFFFFFFFFF, 9);
  parametric_test_progbuf_write_read_longlong (0x3FFFFFFFFFFFFFFF, 9);

  parametric_test_progbuf_write_read_longlong (0x7FFFFFFFFFFFFFFF, 10);
}
END_TEST

START_TEST (test_progbuf_write_read_negative_longlong)
{
  parametric_test_progbuf_write_read_longlong (-1, 1);
  parametric_test_progbuf_write_read_longlong (-0x3F, 1);

  parametric_test_progbuf_write_read_longlong (-0x7F, 2);
  parametric_test_progbuf_write_read_longlong (-0x1FFF, 2);

  parametric_test_progbuf_write_read_longlong (-0x3FFF, 3);
  parametric_test_progbuf_write_read_longlong (-0xFFFFF, 3);

  parametric_test_progbuf_write_read_longlong (-0x1FFFFF, 4);
  parametric_test_progbuf_write_read_longlong (-0x7FFFFFF, 4);

  parametric_test_progbuf_write_read_longlong (-0xFFFFFFF, 5);
  parametric_test_progbuf_write_read_longlong (-0x7FFFFFFF, 5);

  parametric_test_progbuf_write_read_longlong (-0x7FFFFFFFF, 6);
  parametric_test_progbuf_write_read_longlong (-0x1FFFFFFFFFF, 6);

  parametric_test_progbuf_write_read_longlong (-0x3FFFFFFFFFF, 7);
  parametric_test_progbuf_write_read_longlong (-0xFFFFFFFFFFFF, 7);

  parametric_test_progbuf_write_read_longlong (-0x1FFFFFFFFFFFF, 8);
  parametric_test_progbuf_write_read_longlong (-0x7FFFFFFFFFFFFF, 8);

  parametric_test_progbuf_write_read_longlong (-0xFFFFFFFFFFFFFF, 9);
  parametric_test_progbuf_write_read_longlong (-0x3FFFFFFFFFFFFFFF, 9);

  parametric_test_progbuf_write_read_longlong (-0x7FFFFFFFFFFFFFFF, 10);
}
END_TEST

START_TEST (test_progbuf_write_read_ulonglong)
{
  progbuf_h buf = progbuf_alloc (1);

  ck_assert (buf);

  unsigned long long val = 0xFFFFFFFFFFFFFFFF;
  unsigned long long p_val = val;

  int ret = progbuf_set_ulonglong (buf, p_val);

  ck_assert (ret == PROGBUF_SUCCESS);

  size_t size;

  ret = progbuf_get_message_buffer (NULL, NULL, NULL);
  ck_assert (ret == PROGBUF_ERROR_NULL_PARAM);

  ret = progbuf_get_message_buffer (NULL, NULL, &size);
  ck_assert (ret == PROGBUF_ERROR_NULL_PARAM);

  ret = progbuf_get_message_buffer (buf, NULL, NULL);
  ck_assert (ret == PROGBUF_ERROR_NULL_PARAM);

  ret = progbuf_get_message_buffer (buf, NULL, &size);
  ck_assert (ret == PROGBUF_SUCCESS);

  ck_assert (size == 1 + 1 + 10);

  progbuf_it_h iter = progbuf_iter_alloc (buf);

  ck_assert (iter);

  ret = progbuf_get_ulonglong (iter, &p_val);

  ck_assert (ret == PROGBUF_SUCCESS);
  ck_assert (val == p_val);

  struct progbuf_it_s *iter_internal = iter;

  ck_assert (iter_internal->read_pos == 1 + 1 + 10);

  ret = progbuf_iter_free (iter);
  ck_assert (ret == PROGBUF_SUCCESS);

  ret = progbuf_free (buf);
  ck_assert (ret == PROGBUF_SUCCESS);
}
END_TEST

static Suite *
progbuf_suite (void)
{
  Suite *s;
  TCase *tc_basic, *tc_long, *tc_longlong;

  s = suite_create ("progbuf test suite");

  tc_basic = tcase_create ("basic");

  tcase_add_test (tc_basic, test_progbuf_alloc_version);

  tc_long = tcase_create ("encode_long");

  tcase_add_test (tc_long, test_progbuf_write_read_positive_long);
  tcase_add_test (tc_long, test_progbuf_write_read_negative_long);
  tcase_add_test (tc_long, test_progbuf_write_read_ulong);

  tc_longlong = tcase_create ("encode_longlong");

  tcase_add_test (tc_longlong, test_progbuf_write_read_positive_longlong);
  tcase_add_test (tc_longlong, test_progbuf_write_read_negative_longlong);
  tcase_add_test (tc_longlong, test_progbuf_write_read_ulonglong);

  suite_add_tcase (s, tc_basic);
  suite_add_tcase (s, tc_long);
  suite_add_tcase (s, tc_longlong);

  return s;
}

int
main ()
{
  int number_failed;
  Suite *s;
  SRunner *sr;

  s = progbuf_suite ();
  sr = srunner_create (s);

  srunner_run_all (sr, CK_NORMAL);
  number_failed = srunner_ntests_failed (sr);
  srunner_free (sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
