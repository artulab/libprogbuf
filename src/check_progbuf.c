#include <check.h>
#include <float.h>
#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "progbuf.h"

START_TEST (test_progbuf_alloc_version)
{
  int ret;
  long message_tag;

  ret = progbuf_message_tag (NULL, &message_tag);
  ck_assert (ret == PROGBUF_ERROR_NULL_PARAM);

  progbuf_h buf = progbuf_alloc (1);

  ck_assert (buf);

  ret = progbuf_message_tag (buf, NULL);
  ck_assert (ret == PROGBUF_ERROR_NULL_PARAM);

  ret = progbuf_message_tag (buf, &message_tag);

  ck_assert (ret == PROGBUF_SUCCESS);
  ck_assert (message_tag == 1);

  size_t size;
  ret = progbuf_buffer_size (buf, &size);

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
  ret = progbuf_buffer_size (buf, &size);

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

  ret = progbuf_buffer_size (buf, &size);
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
  ret = progbuf_buffer_size (buf, &size);

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

  ret = progbuf_buffer_size (buf, &size);
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

START_TEST (test_progbuf_load_from_buffer)
{
  int ret;
  const long tag = -11;

  progbuf_h buf = progbuf_alloc (tag);

  long p_tag;

  ck_assert (buf);

  ret = progbuf_message_tag (buf, &p_tag);

  ck_assert (ret == PROGBUF_SUCCESS);
  ck_assert (tag == p_tag);

  long val = -0xFF;
  long p_val = val;

  ret = progbuf_set_long (buf, p_val);

  ck_assert (ret == PROGBUF_SUCCESS);

  size_t size;

  ret = progbuf_buffer_size (buf, &size);
  ck_assert (ret == PROGBUF_SUCCESS);
  ck_assert (size == 4);

  progbuf_it_h iter = progbuf_iter_alloc (buf);

  ck_assert (iter);

  ret = progbuf_get_long (iter, &p_val);

  ck_assert (ret == PROGBUF_SUCCESS);
  ck_assert (val == p_val);

  struct progbuf_it_s *iter_internal = iter;

  ck_assert (iter_internal->read_pos == 4);

  void *buffer;
  progbuf_own_buffer (buf, &buffer, &size);

  ck_assert (buffer);
  ck_assert (size == 4);

  progbuf_h buf_2 = progbuf_from_buffer (buffer, size);

  ck_assert (buf_2);

  ret = progbuf_buffer_size (buf_2, &size);
  ck_assert (ret == PROGBUF_SUCCESS);
  ck_assert (size == 4);

  ret = progbuf_message_tag (buf_2, &p_tag);

  ck_assert (ret == PROGBUF_SUCCESS);
  ck_assert (tag == p_tag);

  progbuf_it_h iter2 = progbuf_iter_alloc (buf_2);

  ck_assert (iter2);

  ret = progbuf_get_long (iter2, &p_val);

  ck_assert (ret == PROGBUF_SUCCESS);
  ck_assert (val == p_val);

  iter_internal = iter2;

  ck_assert (iter_internal->read_pos == 4);

  val = 0xFF;
  p_val = val;

  ret = progbuf_set_long (buf_2, p_val);
  ck_assert (ret == PROGBUF_SUCCESS);

  ret = progbuf_get_long (iter2, &p_val);

  ck_assert (ret == PROGBUF_SUCCESS);
  ck_assert (val == p_val);

  ck_assert (iter_internal->read_pos == 7);

  ret = progbuf_get_long (iter2, &p_val);
  ck_assert (ret == PROGBUF_ERROR_END_OF_ITER);

  ret = progbuf_buffer_size (buf_2, &size);
  ck_assert (ret == PROGBUF_SUCCESS);
  ck_assert (size == 7);

  ret = progbuf_iter_free (iter);
  ck_assert (ret == PROGBUF_SUCCESS);

  ret = progbuf_free (buf);
  ck_assert (ret == PROGBUF_SUCCESS);

  ret = progbuf_iter_free (iter2);
  ck_assert (ret == PROGBUF_SUCCESS);

  ret = progbuf_free (buf_2);
  ck_assert (ret == PROGBUF_SUCCESS);
}
END_TEST

START_TEST (test_progbuf_write_read_float)
{
  int ret;

  progbuf_h buf = progbuf_alloc (1);

  ck_assert (buf);

  float val = FLT_MAX;
  float p_val = val;

  ret = progbuf_set_float (buf, p_val);
  ck_assert (ret == PROGBUF_SUCCESS);

  size_t size;
  ret = progbuf_buffer_size (buf, &size);
  ck_assert (ret == PROGBUF_SUCCESS);
  ck_assert (size == 6);

  progbuf_it_h iter = progbuf_iter_alloc (buf);

  ck_assert (iter);

  ret = progbuf_get_float (iter, &p_val);

  ck_assert (ret == PROGBUF_SUCCESS);
  ck_assert (val == p_val);

  struct progbuf_it_s *iter_internal = iter;

  ck_assert (iter_internal->read_pos == 6);

  ck_assert (ret == PROGBUF_SUCCESS);

  ret = progbuf_iter_free (iter);
  ck_assert (ret == PROGBUF_SUCCESS);

  ret = progbuf_free (buf);
  ck_assert (ret == PROGBUF_SUCCESS);
}

START_TEST (test_progbuf_write_read_double)
{
  int ret;

  progbuf_h buf = progbuf_alloc (1);

  ck_assert (buf);

  double val = DBL_MAX;
  double p_val = val;

  ret = progbuf_set_double (buf, p_val);
  ck_assert (ret == PROGBUF_SUCCESS);

  size_t size;
  ret = progbuf_buffer_size (buf, &size);
  ck_assert (ret == PROGBUF_SUCCESS);
  ck_assert (size == 10);

  progbuf_it_h iter = progbuf_iter_alloc (buf);

  ck_assert (iter);

  ret = progbuf_get_double (iter, &p_val);

  ck_assert (ret == PROGBUF_SUCCESS);
  ck_assert (val == p_val);

  struct progbuf_it_s *iter_internal = iter;

  ck_assert (iter_internal->read_pos == 10);

  ck_assert (ret == PROGBUF_SUCCESS);

  ret = progbuf_iter_free (iter);
  ck_assert (ret == PROGBUF_SUCCESS);

  ret = progbuf_free (buf);
  ck_assert (ret == PROGBUF_SUCCESS);
}

START_TEST (test_progbuf_write_read_int_array)
{
  progbuf_h buf = progbuf_alloc (1);

  ck_assert (buf);

  const int val[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
  const size_t actual_size = sizeof (val) / sizeof (int);

  int ret = progbuf_set_int_array (buf, val, actual_size);

  ck_assert (ret == PROGBUF_SUCCESS);

  size_t size;

  ret = progbuf_buffer_size (buf, &size);
  ck_assert (ret == PROGBUF_SUCCESS);

  ck_assert (size == 12);

  progbuf_it_h iter = progbuf_iter_alloc (buf);

  ck_assert (iter);

  int *p_val;
  size_t p_size;
  ret = progbuf_get_int_array (iter, &p_val, &p_size);

  ck_assert (ret == PROGBUF_SUCCESS);
  ck_assert (p_val);
  ck_assert (p_size == actual_size);

  for (int i = 0; i < actual_size; ++i)
    {
      ck_assert (val[i] == p_val[i]);
    }

  struct progbuf_it_s *iter_internal = iter;

  ck_assert (iter_internal->read_pos == 12);

  free (p_val);

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
  TCase *tc_basic, *tc_long, *tc_longlong, *tc_load, *tc_float, *tc_array;

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

  tc_load = tcase_create ("load_from_buffer");
  tcase_add_test (tc_load, test_progbuf_load_from_buffer);

  tc_float = tcase_create ("encode_floating_point");
  tcase_add_test (tc_float, test_progbuf_write_read_float);
  tcase_add_test (tc_float, test_progbuf_write_read_double);

  tc_array = tcase_create ("encode_array");
  tcase_add_test (tc_array, test_progbuf_write_read_int_array);

  suite_add_tcase (s, tc_basic);
  suite_add_tcase (s, tc_long);
  suite_add_tcase (s, tc_longlong);
  suite_add_tcase (s, tc_load);
  suite_add_tcase (s, tc_float);
  suite_add_tcase (s, tc_array);

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
