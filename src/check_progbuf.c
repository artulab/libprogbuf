#include <check.h>
#include <float.h>
#include <limits.h>
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

START_TEST (test_progbuf_write_read_float_array)
{
  progbuf_h buf = progbuf_alloc (1);

  ck_assert (buf);

  const float val[] = { FLT_MIN, FLT_MAX };
  const size_t actual_size = 2;

  int ret = progbuf_set_float_array (buf, val, actual_size);

  ck_assert (ret == PROGBUF_SUCCESS);

  size_t size;

  ret = progbuf_buffer_size (buf, &size);
  ck_assert (ret == PROGBUF_SUCCESS);

  ck_assert (size == 11);

  progbuf_it_h iter = progbuf_iter_alloc (buf);

  ck_assert (iter);

  float *p_val;
  size_t p_size;
  ret = progbuf_get_float_array (iter, &p_val, &p_size);

  ck_assert (ret == PROGBUF_SUCCESS);
  ck_assert (p_val);
  ck_assert (p_size == actual_size);

  for (int i = 0; i < actual_size; ++i)
    {
      ck_assert (val[i] == p_val[i]);
    }

  struct progbuf_it_s *iter_internal = iter;

  ck_assert (iter_internal->read_pos == 11);

  free (p_val);

  ret = progbuf_iter_free (iter);
  ck_assert (ret == PROGBUF_SUCCESS);

  ret = progbuf_free (buf);
  ck_assert (ret == PROGBUF_SUCCESS);
}
END_TEST

START_TEST (test_progbuf_write_read_double_array)
{
  progbuf_h buf = progbuf_alloc (1);

  ck_assert (buf);

  const double val[] = { DBL_MIN, DBL_MAX };
  const size_t actual_size = 2;

  int ret = progbuf_set_double_array (buf, val, actual_size);

  ck_assert (ret == PROGBUF_SUCCESS);

  size_t size;

  ret = progbuf_buffer_size (buf, &size);
  ck_assert (ret == PROGBUF_SUCCESS);

  ck_assert (size == 19);

  progbuf_it_h iter = progbuf_iter_alloc (buf);

  ck_assert (iter);

  double *p_val;
  size_t p_size;
  ret = progbuf_get_double_array (iter, &p_val, &p_size);

  ck_assert (ret == PROGBUF_SUCCESS);
  ck_assert (p_val);
  ck_assert (p_size == actual_size);

  for (int i = 0; i < actual_size; ++i)
    {
      ck_assert (val[i] == p_val[i]);
    }

  struct progbuf_it_s *iter_internal = iter;

  ck_assert (iter_internal->read_pos == 19);

  free (p_val);

  ret = progbuf_iter_free (iter);
  ck_assert (ret == PROGBUF_SUCCESS);

  ret = progbuf_free (buf);
  ck_assert (ret == PROGBUF_SUCCESS);
}
END_TEST

START_TEST (test_progbuf_write_read_string)
{
  progbuf_h buf = progbuf_alloc (1);

  ck_assert (buf);

  const char *val = "abcdefghij";
  const size_t actual_size = 10;

  int ret = progbuf_set_string (buf, val);

  ck_assert (ret == PROGBUF_SUCCESS);

  size_t size;

  ret = progbuf_buffer_size (buf, &size);
  ck_assert (ret == PROGBUF_SUCCESS);

  ck_assert (size == actual_size + 1 + 3);

  progbuf_it_h iter = progbuf_iter_alloc (buf);

  ck_assert (iter);

  char *p_val;
  size_t p_size;
  ret = progbuf_get_string (iter, &p_val);

  ck_assert (ret == PROGBUF_SUCCESS);
  ck_assert (p_val);
  ck_assert (strlen (val) == strlen (p_val));
  ck_assert (strcmp (val, p_val) == 0);

  struct progbuf_it_s *iter_internal = iter;

  ck_assert (iter_internal->read_pos == actual_size + 1 + 3);

  free (p_val);

  ret = progbuf_iter_free (iter);
  ck_assert (ret == PROGBUF_SUCCESS);

  ret = progbuf_free (buf);
  ck_assert (ret == PROGBUF_SUCCESS);
}
END_TEST

START_TEST (test_progbuf_write_read_raw_data)
{
  progbuf_h buf = progbuf_alloc (1);

  ck_assert (buf);

  const char val[] = { 1, 2, 3, 4, 5 };
  const size_t actual_size = 5;

  int ret = progbuf_set_raw_data (buf, val, actual_size);

  ck_assert (ret == PROGBUF_SUCCESS);

  size_t size;

  ret = progbuf_buffer_size (buf, &size);
  ck_assert (ret == PROGBUF_SUCCESS);

  ck_assert (size == actual_size + 3);

  progbuf_it_h iter = progbuf_iter_alloc (buf);

  ck_assert (iter);

  void *p_val = 0;
  size_t p_size;
  ret = progbuf_get_raw_data (iter, &p_val, &p_size);

  ck_assert (ret == PROGBUF_SUCCESS);
  ck_assert (p_val);
  ck_assert (actual_size == p_size);
  ck_assert (memcmp (val, p_val, actual_size) == 0);

  struct progbuf_it_s *iter_internal = iter;

  ck_assert (iter_internal->read_pos == actual_size + 3);

  free (p_val);

  ret = progbuf_iter_free (iter);
  ck_assert (ret == PROGBUF_SUCCESS);

  ret = progbuf_free (buf);
  ck_assert (ret == PROGBUF_SUCCESS);
}
END_TEST

START_TEST (test_progbuf_write_read_multiple_arrays)
{
  progbuf_h buf = progbuf_alloc (1);

  ck_assert (buf);

  const long val_long[] = { LONG_MAX, LONG_MIN };
  const size_t size_long = 2;

  int ret = progbuf_set_long_array (buf, val_long, size_long);

  ck_assert (ret == PROGBUF_SUCCESS);

  const unsigned long val_ulong[] = { ULONG_MAX, 0, 10 };
  const size_t size_ulong = 3;

  ret = progbuf_set_ulong_array (buf, val_ulong, size_ulong);

  ck_assert (ret == PROGBUF_SUCCESS);

  progbuf_it_h iter = progbuf_iter_alloc (buf);

  ck_assert (iter);

  /* get long array and test */
  long *actual_val_long;
  size_t actual_size_long;
  ret = progbuf_get_long_array (iter, &actual_val_long, &actual_size_long);

  ck_assert (ret == PROGBUF_SUCCESS);
  ck_assert (actual_val_long);
  ck_assert (size_long == actual_size_long);

  for (int i = 0; i < size_long; ++i)
    {
      ck_assert (val_long[i] == actual_val_long[i]);
    }

  free (actual_val_long);

  /* get unsinged long array and test */

  unsigned long *actual_val_ulong;
  size_t actual_size_ulong;
  ret = progbuf_get_ulong_array (iter, &actual_val_ulong, &actual_size_ulong);

  ck_assert (ret == PROGBUF_SUCCESS);
  ck_assert (actual_val_ulong);
  ck_assert (size_ulong == actual_size_ulong);

  for (int i = 0; i < size_ulong; ++i)
    {
      ck_assert (val_ulong[i] == actual_val_ulong[i]);
    }

  free (actual_val_ulong);

  /* try to get inexistent array data */
  ret = progbuf_get_ulong_array (iter, &actual_val_ulong, &actual_size_ulong);
  ck_assert (PROGBUF_ERROR_END_OF_ITER == ret);

  /* add one long long data and read it */
  ret = progbuf_set_longlong (buf, LLONG_MAX);
  ck_assert (ret == PROGBUF_SUCCESS);

  /* try to get wrong type of data */
  float float_value;
  ret = progbuf_get_float (iter, &float_value);
  ck_assert (PROGBUF_ERROR_UNEXPECTED_TYPE == ret);

  /* now get long long value which is of correct type */
  long long llong_value;
  ret = progbuf_get_longlong (iter, &llong_value);
  ck_assert (llong_value == LLONG_MAX);

  /* try to get inexistent data again */
  ret = progbuf_get_longlong (iter, &llong_value);
  ck_assert (PROGBUF_ERROR_END_OF_ITER == ret);

  ret = progbuf_iter_free (iter);
  ck_assert (ret == PROGBUF_SUCCESS);

  ret = progbuf_free (buf);
  ck_assert (ret == PROGBUF_SUCCESS);
}
END_TEST

START_TEST (test_progbuf_write_read_message)
{
  int ret;

  progbuf_h buf = progbuf_alloc (7);
  ck_assert (buf);

  ret = progbuf_set_float (buf, FLT_MAX);
  ck_assert (ret == PROGBUF_SUCCESS);

  ret = progbuf_set_float (buf, FLT_MIN);
  ck_assert (ret == PROGBUF_SUCCESS);

  progbuf_h container_buf = progbuf_alloc (11);
  ck_assert (container_buf);

  ret = progbuf_set_int (container_buf, 17);
  ck_assert (ret == PROGBUF_SUCCESS);

  ret = progbuf_set_message (container_buf, buf);
  ck_assert (ret == PROGBUF_SUCCESS);

  ret = progbuf_free (buf);
  ck_assert (ret == PROGBUF_SUCCESS);

  ret = progbuf_set_int (container_buf, 21);
  ck_assert (ret == PROGBUF_SUCCESS);

  size_t size;
  ret = progbuf_buffer_size (container_buf, &size);
  ck_assert (ret == PROGBUF_SUCCESS);
  ck_assert (size == 18);

  progbuf_it_h iter = progbuf_iter_alloc (container_buf);
  ck_assert (iter);

  long message_tag;
  ret = progbuf_message_tag (container_buf, &message_tag);
  ck_assert (ret == PROGBUF_SUCCESS);
  ck_assert (message_tag == 11);

  int i_value;
  ret = progbuf_get_int (iter, &i_value);
  ck_assert (ret == PROGBUF_SUCCESS);
  ck_assert (i_value == 17);

  progbuf_h nested_buf;
  ret = progbuf_get_message (iter, &nested_buf);
  ck_assert (ret == PROGBUF_SUCCESS);

  ret = progbuf_get_int (iter, &i_value);
  ck_assert (ret == PROGBUF_SUCCESS);
  ck_assert (i_value == 21);

  ret = progbuf_iter_free (iter);
  ck_assert (ret == PROGBUF_SUCCESS);

  ret = progbuf_buffer_size (nested_buf, &size);
  ck_assert (ret == PROGBUF_SUCCESS);
  ck_assert (size == 11);

  ret = progbuf_message_tag (nested_buf, &message_tag);
  ck_assert (ret == PROGBUF_SUCCESS);
  ck_assert (message_tag == 7);

  /* let's extend the nested message now*/
  ret = progbuf_set_int (nested_buf, 23);
  ck_assert (ret == PROGBUF_SUCCESS);

  ret = progbuf_buffer_size (nested_buf, &size);
  ck_assert (ret == PROGBUF_SUCCESS);
  ck_assert (size == 13);

  progbuf_it_h iter_nested = progbuf_iter_alloc (nested_buf);
  ck_assert (iter_nested);

  float float_value;
  ret = progbuf_get_float (iter_nested, &float_value);

  ck_assert (ret == PROGBUF_SUCCESS);
  ck_assert (float_value == FLT_MAX);

  ret = progbuf_get_float (iter_nested, &float_value);

  ck_assert (ret == PROGBUF_SUCCESS);
  ck_assert (float_value == FLT_MIN);

  ret = progbuf_get_float (iter_nested, &float_value);
  ck_assert (ret == PROGBUF_ERROR_UNEXPECTED_TYPE);

  ret = progbuf_get_int (iter_nested, &i_value);
  ck_assert (ret == PROGBUF_SUCCESS);
  ck_assert (i_value == 23);

  struct progbuf_it_s *iter_internal = iter_nested;
  ck_assert (iter_internal->read_pos == 13);

  ret = progbuf_iter_free (iter_nested);
  ck_assert (ret == PROGBUF_SUCCESS);

  ret = progbuf_free (nested_buf);
  ck_assert (ret == PROGBUF_SUCCESS);

  ret = progbuf_free (container_buf);
  ck_assert (ret == PROGBUF_SUCCESS);
}

static Suite *
progbuf_suite (void)
{
  Suite *s;
  TCase *tc_basic, *tc_long, *tc_longlong, *tc_load, *tc_float, *tc_array,
      *tc_message;

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
  tcase_add_test (tc_array, test_progbuf_write_read_float_array);
  tcase_add_test (tc_array, test_progbuf_write_read_double_array);
  tcase_add_test (tc_array, test_progbuf_write_read_string);
  tcase_add_test (tc_array, test_progbuf_write_read_raw_data);
  tcase_add_test (tc_array, test_progbuf_write_read_multiple_arrays);

  tc_message = tcase_create ("encode_message");
  tcase_add_test (tc_message, test_progbuf_write_read_message);

  suite_add_tcase (s, tc_basic);
  suite_add_tcase (s, tc_long);
  suite_add_tcase (s, tc_longlong);
  suite_add_tcase (s, tc_load);
  suite_add_tcase (s, tc_float);
  suite_add_tcase (s, tc_array);
  suite_add_tcase (s, tc_message);

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
