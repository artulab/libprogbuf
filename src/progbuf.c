#include "progbuf.h"
#include "common.h"

#include <stdlib.h>
#include <string.h>

typedef unsigned int uint;
typedef unsigned long ulong;
typedef long long longlong;
typedef unsigned long long ulonglong;

#define DETERMINE_VAR_SIZE(type)                                              \
  static inline int determine_var_##type##_size (type value)                  \
  {                                                                           \
    type mask;                                                                \
    memset (&mask, 0xFF, sizeof (mask));                                      \
                                                                              \
    mask <<= 6;                                                               \
                                                                              \
    if ((value & mask) == 0)                                                  \
      return 1;                                                               \
                                                                              \
    int s = 2;                                                                \
    while (1)                                                                 \
      {                                                                       \
        mask <<= 7;                                                           \
                                                                              \
        if ((value & mask) == 0)                                              \
          return s;                                                           \
                                                                              \
        s++;                                                                  \
      }                                                                       \
  }

#define READ_VAR_SIZE(type)                                                   \
  static inline int read_var_##type (progbuf_it_h iter, type *value,          \
                                     int *negative)                           \
  {                                                                           \
    type tmp = 0;                                                             \
    size_t prev_read_pos = iter->read_pos;                                    \
                                                                              \
    int i = 0;                                                                \
    char byte;                                                                \
    const int bitsize = sizeof (type) * 8;                                    \
    while (i < bitsize)                                                       \
      {                                                                       \
        byte = iter->buf->buffer[iter->read_pos++];                           \
        if (i == 0)                                                           \
          {                                                                   \
            *negative = byte & 0x40;                                          \
            tmp |= (type)(byte & 0x3F) << i;                                  \
            i += 6;                                                           \
          }                                                                   \
        else                                                                  \
          {                                                                   \
            tmp |= (type)(byte & 0x7F) << i;                                  \
            i += 7;                                                           \
          }                                                                   \
                                                                              \
        if ((byte & 0x80) == 0)                                               \
          {                                                                   \
            *value = tmp;                                                     \
            return 0;                                                         \
          }                                                                   \
      }                                                                       \
                                                                              \
    iter->read_pos = prev_read_pos;                                           \
    return -1;                                                                \
  }

#define WRITE_VAR_SIZE(type)                                                  \
  static inline void write_var_##type (progbuf_h buf, type value, int size,   \
                                       int negative)                          \
  {                                                                           \
    for (int i = 0; i < size - 1; ++i)                                        \
      {                                                                       \
        if (i == 0)                                                           \
          {                                                                   \
            if (negative)                                                     \
              {                                                               \
                buf->buffer[buf->size] = (char)((value & 0x3F) | 0xC0);       \
              }                                                               \
            else                                                              \
              {                                                               \
                buf->buffer[buf->size] = (char)((value & 0x3F) | 0x80);       \
              }                                                               \
            value = (type)((type)value >> 6);                                 \
          }                                                                   \
        else                                                                  \
          {                                                                   \
            buf->buffer[buf->size + i] = (char)((value & 0x7F) | 0x80);       \
            value = (type)((type)value >> 7);                                 \
          }                                                                   \
      }                                                                       \
                                                                              \
    if (size == 1 && negative)                                                \
      {                                                                       \
        buf->buffer[buf->size] = (char)value | 0x40;                          \
      }                                                                       \
    else                                                                      \
      {                                                                       \
        buf->buffer[buf->size + (size - 1)] = (char)value;                    \
      }                                                                       \
                                                                              \
    buf->size += size;                                                        \
  }

static inline int
check_buffer_and_expand (progbuf_h buf, int size)
{
  if (buf->size + size > buf->capacity)
    {
      char *newbuf = malloc (buf->capacity * 2);
      if (!newbuf)
        return PROGBUF_ERROR_MEM_ALLOC;

      memcpy (newbuf, buf->buffer, buf->capacity);
      free (buf->buffer);
      buf->buffer = newbuf;
      buf->capacity *= 2;
    }
  return 0;
}

DETERMINE_VAR_SIZE (uint)
READ_VAR_SIZE (uint)
WRITE_VAR_SIZE (uint)

DETERMINE_VAR_SIZE (ulong)
READ_VAR_SIZE (ulong)
WRITE_VAR_SIZE (ulong)

DETERMINE_VAR_SIZE (ulonglong)
READ_VAR_SIZE (ulonglong)
WRITE_VAR_SIZE (ulonglong)

DETERMINE_VAR_SIZE (size_t)
READ_VAR_SIZE (size_t)
WRITE_VAR_SIZE (size_t)

#define PROGBUF_SET_SIGNED(type, utype)                                       \
  int progbuf_set_##type (progbuf_h buf, type value)                          \
  {                                                                           \
    int val_size, ret;                                                        \
                                                                              \
    if (!buf)                                                                 \
      return PROGBUF_ERROR_NULL_PARAM;                                        \
                                                                              \
    if (!buf->buffer)                                                         \
      return PROGBUF_ERROR_NOT_OWNING;                                        \
                                                                              \
    val_size = determine_var_##utype##_size (ABS (value));                    \
                                                                              \
    ret = check_buffer_and_expand (buf, val_size + 1);                        \
    if (ret != 0)                                                             \
      return ret;                                                             \
                                                                              \
    buf->buffer[buf->size] = PROGBUF_TYPE_VAR_INT;                            \
    buf->size++;                                                              \
                                                                              \
    write_var_##utype (buf, ABS (value), val_size, value < 0);                \
                                                                              \
    return PROGBUF_SUCCESS;                                                   \
  }

#define PROGBUF_SET_UNSIGNED(type, utype)                                     \
  int progbuf_set_##type (progbuf_h buf, type value)                          \
  {                                                                           \
    int val_size, ret;                                                        \
                                                                              \
    if (!buf)                                                                 \
      return PROGBUF_ERROR_NULL_PARAM;                                        \
                                                                              \
    if (!buf->buffer)                                                         \
      return PROGBUF_ERROR_NOT_OWNING;                                        \
                                                                              \
    val_size = determine_var_##utype##_size (value);                          \
                                                                              \
    ret = check_buffer_and_expand (buf, val_size + 1);                        \
    if (ret != 0)                                                             \
      return ret;                                                             \
                                                                              \
    buf->buffer[buf->size] = PROGBUF_TYPE_VAR_INT;                            \
    buf->size++;                                                              \
                                                                              \
    write_var_##utype (buf, value, val_size, 0);                              \
                                                                              \
    return PROGBUF_SUCCESS;                                                   \
  }

#define PROGBUF_GET_SIGNED(type, utype)                                       \
  int progbuf_get_##type (progbuf_it_h iter, type *value)                     \
  {                                                                           \
    if (!iter || !value)                                                      \
      return PROGBUF_ERROR_NULL_PARAM;                                        \
                                                                              \
    if (!iter->buf->buffer)                                                   \
      return PROGBUF_ERROR_NOT_OWNING;                                        \
                                                                              \
    if (iter->read_pos >= iter->buf->size)                                    \
      return PROGBUF_ERROR_END_OF_ITER;                                       \
                                                                              \
    char val_type = iter->buf->buffer[iter->read_pos];                        \
                                                                              \
    if ((val_type & PROGBUF_TYPE_VAR_INT) != PROGBUF_TYPE_VAR_INT)            \
      return PROGBUF_ERROR_UNEXPECTED_TYPE;                                   \
                                                                              \
    iter->read_pos++;                                                         \
                                                                              \
    utype u_value;                                                            \
    int negative;                                                             \
    if (read_var_##utype (iter, &u_value, &negative) != 0)                    \
      {                                                                       \
        iter->read_pos--;                                                     \
        return PROGBUF_ERROR_READ;                                            \
      }                                                                       \
                                                                              \
    *value = (negative ? -u_value : u_value);                                 \
                                                                              \
    return PROGBUF_SUCCESS;                                                   \
  }

#define PROGBUF_GET_UNSIGNED(type, utype)                                     \
  int progbuf_get_##type (progbuf_it_h iter, type *value)                     \
  {                                                                           \
    if (!iter || !value)                                                      \
      return PROGBUF_ERROR_NULL_PARAM;                                        \
                                                                              \
    if (!iter->buf->buffer)                                                   \
      return PROGBUF_ERROR_NOT_OWNING;                                        \
                                                                              \
    if (iter->read_pos >= iter->buf->size)                                    \
      return PROGBUF_ERROR_END_OF_ITER;                                       \
                                                                              \
    char val_type = iter->buf->buffer[iter->read_pos];                        \
                                                                              \
    if ((val_type & PROGBUF_TYPE_VAR_INT) != PROGBUF_TYPE_VAR_INT)            \
      return PROGBUF_ERROR_UNEXPECTED_TYPE;                                   \
                                                                              \
    iter->read_pos++;                                                         \
                                                                              \
    utype u_value;                                                            \
    int negative;                                                             \
    if (read_var_##utype (iter, &u_value, &negative) != 0)                    \
      {                                                                       \
        iter->read_pos--;                                                     \
        return PROGBUF_ERROR_READ;                                            \
      }                                                                       \
                                                                              \
    *value = u_value;                                                         \
                                                                              \
    return PROGBUF_SUCCESS;                                                   \
  }

PROGBUF_SET_SIGNED (int, uint)
PROGBUF_GET_SIGNED (int, uint)

PROGBUF_SET_SIGNED (long, ulong)
PROGBUF_GET_SIGNED (long, ulong)

PROGBUF_SET_SIGNED (longlong, ulonglong)
PROGBUF_GET_SIGNED (longlong, ulonglong)

PROGBUF_SET_UNSIGNED (uint, uint)
PROGBUF_GET_UNSIGNED (uint, uint)

PROGBUF_SET_UNSIGNED (ulong, ulong)
PROGBUF_GET_UNSIGNED (ulong, ulong)

PROGBUF_SET_UNSIGNED (ulonglong, ulonglong)
PROGBUF_GET_UNSIGNED (ulonglong, ulonglong)

PROGBUF_SET_UNSIGNED (size_t, size_t)
PROGBUF_GET_UNSIGNED (size_t, size_t)

#define PROGBUF_SET_FLOAT(type, type_enum)                                    \
  int progbuf_set_##type (progbuf_h buf, type value)                          \
  {                                                                           \
    int ret;                                                                  \
                                                                              \
    if (!buf)                                                                 \
      return PROGBUF_ERROR_NULL_PARAM;                                        \
                                                                              \
    if (!buf->buffer)                                                         \
      return PROGBUF_ERROR_NOT_OWNING;                                        \
                                                                              \
    ret = check_buffer_and_expand (buf, 1 + sizeof (type));                   \
    if (ret != 0)                                                             \
      return ret;                                                             \
                                                                              \
    buf->buffer[buf->size] = type_enum;                                       \
    buf->size++;                                                              \
                                                                              \
    memcpy (buf->buffer + buf->size, &value, sizeof (type));                  \
    buf->size += sizeof (type);                                               \
                                                                              \
    return PROGBUF_SUCCESS;                                                   \
  }

#define PROGBUF_GET_FLOAT(type, type_enum)                                    \
  int progbuf_get_##type (progbuf_it_h iter, type *value)                     \
  {                                                                           \
    if (!iter || !value)                                                      \
      return PROGBUF_ERROR_NULL_PARAM;                                        \
                                                                              \
    if (!iter->buf->buffer)                                                   \
      return PROGBUF_ERROR_NOT_OWNING;                                        \
                                                                              \
    if (iter->read_pos >= iter->buf->size)                                    \
      return PROGBUF_ERROR_END_OF_ITER;                                       \
                                                                              \
    char val_type = iter->buf->buffer[iter->read_pos];                        \
                                                                              \
    if ((val_type & type_enum) != type_enum)                                  \
      return PROGBUF_ERROR_UNEXPECTED_TYPE;                                   \
                                                                              \
    iter->read_pos++;                                                         \
                                                                              \
    *value = *((type *)(iter->buf->buffer + iter->read_pos));                 \
    iter->read_pos += sizeof (type);                                          \
                                                                              \
    return PROGBUF_SUCCESS;                                                   \
  }

PROGBUF_GET_FLOAT (float, PROGBUF_TYPE_FLOAT)
PROGBUF_SET_FLOAT (float, PROGBUF_TYPE_FLOAT)

PROGBUF_GET_FLOAT (double, PROGBUF_TYPE_DOUBLE)
PROGBUF_SET_FLOAT (double, PROGBUF_TYPE_DOUBLE)

#define PROGBUF_ARRAY_SET_SIGNED(type, utype, type_enum)                      \
  int progbuf_set_##type##_array (progbuf_h buf, const type *arr, size_t len) \
  {                                                                           \
    int val_size, ret;                                                        \
                                                                              \
    if (!buf || !arr)                                                         \
      return PROGBUF_ERROR_NULL_PARAM;                                        \
                                                                              \
    if (!buf->buffer)                                                         \
      return PROGBUF_ERROR_NOT_OWNING;                                        \
                                                                              \
    val_size = determine_var_size_t_size (len);                               \
    ret = check_buffer_and_expand (buf, val_size + 1);                        \
    if (ret != 0)                                                             \
      return ret;                                                             \
                                                                              \
    buf->buffer[buf->size] = type_enum;                                       \
    buf->size++;                                                              \
                                                                              \
    write_var_size_t (buf, len, val_size, 0);                                 \
    size_t total_written = 1 + val_size;                                      \
                                                                              \
    for (size_t i = 0; i < len; ++i)                                          \
      {                                                                       \
        type value = arr[i];                                                  \
        val_size = determine_var_##utype##_size (ABS (value));                \
        ret = check_buffer_and_expand (buf, val_size);                        \
                                                                              \
        if (ret != 0)                                                         \
          {                                                                   \
            buf->size -= total_written;                                       \
            return ret;                                                       \
          }                                                                   \
                                                                              \
        write_var_##utype (buf, ABS (value), val_size, value < 0);            \
        total_written += val_size;                                            \
      }                                                                       \
                                                                              \
    return PROGBUF_SUCCESS;                                                   \
  }

#define PROGBUF_ARRAY_SET_UNSIGNED(type, utype, type_enum)                    \
  int progbuf_set_##type##_array (progbuf_h buf, const type *arr, size_t len) \
  {                                                                           \
    int val_size, ret;                                                        \
                                                                              \
    if (!buf || !arr)                                                         \
      return PROGBUF_ERROR_NULL_PARAM;                                        \
                                                                              \
    if (!buf->buffer)                                                         \
      return PROGBUF_ERROR_NOT_OWNING;                                        \
                                                                              \
    val_size = determine_var_size_t_size (len);                               \
    ret = check_buffer_and_expand (buf, val_size + 1);                        \
    if (ret != 0)                                                             \
      return ret;                                                             \
                                                                              \
    buf->buffer[buf->size] = type_enum;                                       \
    buf->size++;                                                              \
                                                                              \
    write_var_size_t (buf, len, val_size, 0);                                 \
    size_t total_written = 1 + val_size;                                      \
                                                                              \
    for (size_t i = 0; i < len; ++i)                                          \
      {                                                                       \
        type value = arr[i];                                                  \
        val_size = determine_var_##utype##_size (value);                      \
        ret = check_buffer_and_expand (buf, val_size);                        \
                                                                              \
        if (ret != 0)                                                         \
          {                                                                   \
            buf->size -= total_written;                                       \
            return ret;                                                       \
          }                                                                   \
                                                                              \
        write_var_##utype (buf, value, val_size, 0);                          \
        total_written += val_size;                                            \
      }                                                                       \
                                                                              \
    return PROGBUF_SUCCESS;                                                   \
  }

#define PROGBUF_ARRAY_GET_SIGNED(type, utype, type_enum)                      \
  int progbuf_get_##type##_array (progbuf_it_h iter, type **arr, size_t *len) \
  {                                                                           \
    if (!iter || !arr || !len)                                                \
      return PROGBUF_ERROR_NULL_PARAM;                                        \
                                                                              \
    if (!iter->buf->buffer)                                                   \
      return PROGBUF_ERROR_NOT_OWNING;                                        \
                                                                              \
    if (iter->read_pos >= iter->buf->size)                                    \
      return PROGBUF_ERROR_END_OF_ITER;                                       \
                                                                              \
    char val_type = iter->buf->buffer[iter->read_pos];                        \
                                                                              \
    if ((val_type & type_enum) != type_enum)                                  \
      return PROGBUF_ERROR_UNEXPECTED_TYPE;                                   \
                                                                              \
    iter->read_pos++;                                                         \
                                                                              \
    size_t u_len;                                                             \
    size_t total_read = 1;                                                    \
    size_t prev_read_pos = iter->read_pos;                                    \
    int negative;                                                             \
    if (read_var_size_t (iter, &u_len, &negative) != 0)                       \
      {                                                                       \
        iter->read_pos -= total_read;                                         \
        return PROGBUF_ERROR_READ;                                            \
      }                                                                       \
                                                                              \
    total_read += (iter->read_pos - prev_read_pos);                           \
    type *l_arr = malloc (sizeof (type) * u_len);                             \
                                                                              \
    if (!l_arr)                                                               \
      {                                                                       \
        iter->read_pos -= total_read;                                         \
        return PROGBUF_ERROR_MEM_ALLOC;                                       \
      }                                                                       \
                                                                              \
    utype u_value;                                                            \
    prev_read_pos = iter->read_pos;                                           \
    for (size_t i = 0; i < u_len; ++i)                                        \
      {                                                                       \
        if (read_var_##utype (iter, &u_value, &negative) != 0)                \
          {                                                                   \
            total_read += (iter->read_pos - prev_read_pos);                   \
            iter->read_pos -= total_read;                                     \
            free (l_arr);                                                     \
            return PROGBUF_ERROR_READ;                                        \
          }                                                                   \
                                                                              \
        l_arr[i] = (negative ? -u_value : u_value);                           \
      }                                                                       \
                                                                              \
    *arr = l_arr;                                                             \
    *len = u_len;                                                             \
                                                                              \
    return PROGBUF_SUCCESS;                                                   \
  }

#define PROGBUF_ARRAY_GET_UNSIGNED(type, utype, type_enum)                    \
  int progbuf_get_##type##_array (progbuf_it_h iter, type **arr, size_t *len) \
  {                                                                           \
    if (!iter || !arr || !len)                                                \
      return PROGBUF_ERROR_NULL_PARAM;                                        \
                                                                              \
    if (!iter->buf->buffer)                                                   \
      return PROGBUF_ERROR_NOT_OWNING;                                        \
                                                                              \
    if (iter->read_pos >= iter->buf->size)                                    \
      return PROGBUF_ERROR_END_OF_ITER;                                       \
                                                                              \
    char val_type = iter->buf->buffer[iter->read_pos];                        \
                                                                              \
    if ((val_type & type_enum) != type_enum)                                  \
      return PROGBUF_ERROR_UNEXPECTED_TYPE;                                   \
                                                                              \
    iter->read_pos++;                                                         \
                                                                              \
    size_t u_len;                                                             \
    size_t total_read = 1;                                                    \
    size_t prev_read_pos = iter->read_pos;                                    \
    int negative;                                                             \
    if (read_var_size_t (iter, &u_len, &negative) != 0)                       \
      {                                                                       \
        iter->read_pos -= total_read;                                         \
        return PROGBUF_ERROR_READ;                                            \
      }                                                                       \
                                                                              \
    total_read += (iter->read_pos - prev_read_pos);                           \
    type *l_arr = malloc (sizeof (type) * u_len);                             \
                                                                              \
    if (!l_arr)                                                               \
      {                                                                       \
        iter->read_pos -= total_read;                                         \
        return PROGBUF_ERROR_MEM_ALLOC;                                       \
      }                                                                       \
                                                                              \
    utype u_value;                                                            \
    prev_read_pos = iter->read_pos;                                           \
    for (size_t i = 0; i < u_len; ++i)                                        \
      {                                                                       \
        if (read_var_##utype (iter, &u_value, &negative) != 0)                \
          {                                                                   \
            total_read += (iter->read_pos - prev_read_pos);                   \
            iter->read_pos -= total_read;                                     \
            free (l_arr);                                                     \
            return PROGBUF_ERROR_READ;                                        \
          }                                                                   \
                                                                              \
        l_arr[i] = u_value;                                                   \
      }                                                                       \
                                                                              \
    *arr = l_arr;                                                             \
    *len = u_len;                                                             \
                                                                              \
    return PROGBUF_SUCCESS;                                                   \
  }

PROGBUF_ARRAY_SET_SIGNED (int, uint, PROGBUF_TYPE_VAR_INT_ARRAY)
PROGBUF_ARRAY_GET_SIGNED (int, uint, PROGBUF_TYPE_VAR_INT_ARRAY)

PROGBUF_ARRAY_SET_UNSIGNED (uint, uint, PROGBUF_TYPE_VAR_INT_ARRAY)
PROGBUF_ARRAY_GET_UNSIGNED (uint, uint, PROGBUF_TYPE_VAR_INT_ARRAY)

PROGBUF_ARRAY_SET_SIGNED (long, ulong, PROGBUF_TYPE_VAR_INT_ARRAY)
PROGBUF_ARRAY_GET_SIGNED (long, ulong, PROGBUF_TYPE_VAR_INT_ARRAY)

PROGBUF_ARRAY_SET_UNSIGNED (ulong, ulong, PROGBUF_TYPE_VAR_INT_ARRAY)
PROGBUF_ARRAY_GET_UNSIGNED (ulong, ulong, PROGBUF_TYPE_VAR_INT_ARRAY)

PROGBUF_ARRAY_SET_SIGNED (longlong, ulonglong, PROGBUF_TYPE_VAR_INT_ARRAY)
PROGBUF_ARRAY_GET_SIGNED (longlong, ulonglong, PROGBUF_TYPE_VAR_INT_ARRAY)

PROGBUF_ARRAY_SET_UNSIGNED (ulonglong, ulonglong, PROGBUF_TYPE_VAR_INT_ARRAY)
PROGBUF_ARRAY_GET_UNSIGNED (ulonglong, ulonglong, PROGBUF_TYPE_VAR_INT_ARRAY)

PROGBUF_ARRAY_SET_UNSIGNED (size_t, size_t, PROGBUF_TYPE_VAR_INT_ARRAY)
PROGBUF_ARRAY_GET_UNSIGNED (size_t, size_t, PROGBUF_TYPE_VAR_INT_ARRAY)

#define PROGBUF_FLOAT_ARRAY_GET(type, type_enum)                              \
  int progbuf_set_##type##_array (progbuf_h buf, const type *arr, size_t len) \
  {                                                                           \
    int val_size, ret;                                                        \
                                                                              \
    if (!buf || !arr)                                                         \
      return PROGBUF_ERROR_NULL_PARAM;                                        \
                                                                              \
    if (!buf->buffer)                                                         \
      return PROGBUF_ERROR_NOT_OWNING;                                        \
                                                                              \
    val_size = determine_var_size_t_size (len);                               \
    const size_t len_bytes = len * sizeof (type);                             \
    ret = check_buffer_and_expand (buf, 1 + val_size + len_bytes);            \
    if (ret != 0)                                                             \
      return ret;                                                             \
                                                                              \
    buf->buffer[buf->size] = type_enum;                                       \
    buf->size++;                                                              \
                                                                              \
    write_var_size_t (buf, len, val_size, 0);                                 \
    memcpy (buf->buffer + buf->size, arr, len_bytes);                         \
    buf->size += len_bytes;                                                   \
                                                                              \
    return PROGBUF_SUCCESS;                                                   \
  }

#define PROGBUF_FLOAT_ARRAY_SET(type, type_enum)                              \
  int progbuf_get_##type##_array (progbuf_it_h iter, type **arr, size_t *len) \
  {                                                                           \
    if (!iter || !arr || !len)                                                \
      return PROGBUF_ERROR_NULL_PARAM;                                        \
                                                                              \
    if (!iter->buf->buffer)                                                   \
      return PROGBUF_ERROR_NOT_OWNING;                                        \
                                                                              \
    if (iter->read_pos >= iter->buf->size)                                    \
      return PROGBUF_ERROR_END_OF_ITER;                                       \
                                                                              \
    char val_type = iter->buf->buffer[iter->read_pos];                        \
                                                                              \
    if ((val_type & type_enum) != type_enum)                                  \
      return PROGBUF_ERROR_UNEXPECTED_TYPE;                                   \
                                                                              \
    iter->read_pos++;                                                         \
                                                                              \
    size_t u_len;                                                             \
    size_t prev_read_pos = iter->read_pos;                                    \
    int negative;                                                             \
    if (read_var_size_t (iter, &u_len, &negative) != 0)                       \
      {                                                                       \
        iter->read_pos--;                                                     \
        return PROGBUF_ERROR_READ;                                            \
      }                                                                       \
                                                                              \
    const size_t len_bytes = u_len * sizeof (type);                           \
    type *l_arr = malloc (len_bytes);                                         \
                                                                              \
    if (!l_arr)                                                               \
      {                                                                       \
        iter->read_pos -= (1 + iter->read_pos - prev_read_pos);               \
        return PROGBUF_ERROR_MEM_ALLOC;                                       \
      }                                                                       \
                                                                              \
    memcpy (l_arr, iter->buf->buffer + iter->read_pos, len_bytes);            \
    iter->read_pos += len_bytes;                                              \
                                                                              \
    *arr = l_arr;                                                             \
    *len = u_len;                                                             \
                                                                              \
    return PROGBUF_SUCCESS;                                                   \
  }

PROGBUF_FLOAT_ARRAY_GET (float, PROGBUF_TYPE_VAR_FLOAT_ARRAY)
PROGBUF_FLOAT_ARRAY_SET (float, PROGBUF_TYPE_VAR_FLOAT_ARRAY)

PROGBUF_FLOAT_ARRAY_GET (double, PROGBUF_TYPE_VAR_DOUBLE_ARRAY)
PROGBUF_FLOAT_ARRAY_SET (double, PROGBUF_TYPE_VAR_DOUBLE_ARRAY)

int
progbuf_set_raw_data (progbuf_h buf, const void *ptr, size_t len)
{
  int val_size, ret;

  if (!buf || !ptr)
    return PROGBUF_ERROR_NULL_PARAM;

  if (!buf->buffer)
    return PROGBUF_ERROR_NOT_OWNING;

  val_size = determine_var_size_t_size (len);
  ret = check_buffer_and_expand (buf, 1 + val_size + len);
  if (ret != 0)
    return ret;

  buf->buffer[buf->size] = PROGBUF_TYPE_RAW;
  buf->size++;

  write_var_size_t (buf, len, val_size, 0);
  memcpy (buf->buffer + buf->size, ptr, len);
  buf->size += len;

  return PROGBUF_SUCCESS;
}

int
progbuf_get_raw_data (progbuf_it_h iter, void **ptr, size_t *len)
{
  if (!iter || !ptr || !len)
    return PROGBUF_ERROR_NULL_PARAM;

  if (!iter->buf->buffer)
    return PROGBUF_ERROR_NOT_OWNING;

  if (iter->read_pos >= iter->buf->size)
    return PROGBUF_ERROR_END_OF_ITER;

  char val_type = iter->buf->buffer[iter->read_pos];

  if ((val_type & PROGBUF_TYPE_RAW) != PROGBUF_TYPE_RAW)
    return PROGBUF_ERROR_UNEXPECTED_TYPE;

  iter->read_pos++;

  size_t u_len;
  size_t prev_read_pos = iter->read_pos;
  int negative;
  if (read_var_size_t (iter, &u_len, &negative) != 0)
    {
      iter->read_pos--;
      return PROGBUF_ERROR_READ;
    }

  void *l_ptr = malloc (u_len);

  if (!l_ptr)
    {
      iter->read_pos -= (1 + iter->read_pos - prev_read_pos);
      return PROGBUF_ERROR_MEM_ALLOC;
    }

  memcpy (l_ptr, iter->buf->buffer + iter->read_pos, u_len);
  iter->read_pos += u_len;

  *ptr = l_ptr;
  *len = u_len;

  return PROGBUF_SUCCESS;
}

int
progbuf_set_message (progbuf_h buf, const progbuf_h message)
{
  int val_size, ret;

  if (!buf || !message)
    return PROGBUF_ERROR_NULL_PARAM;

  if (!buf->buffer || !message->buffer)
    return PROGBUF_ERROR_NOT_OWNING;

  val_size = determine_var_size_t_size (message->size);
  ret = check_buffer_and_expand (buf, 1 + val_size + message->size);
  if (ret != 0)
    return ret;

  buf->buffer[buf->size] = PROGBUF_TYPE_MESSAGE;
  buf->size++;

  write_var_size_t (buf, message->size, val_size, 0);
  memcpy (buf->buffer + buf->size, message->buffer, message->size);
  buf->size += message->size;

  return PROGBUF_SUCCESS;
}

int
progbuf_get_message (progbuf_it_h iter, progbuf_h *message)
{
  if (!iter || !message)
    return PROGBUF_ERROR_NULL_PARAM;

  if (!iter->buf->buffer)
    return PROGBUF_ERROR_NOT_OWNING;

  if (iter->read_pos >= iter->buf->size)
    return PROGBUF_ERROR_END_OF_ITER;

  char val_type = iter->buf->buffer[iter->read_pos];

  if ((val_type & PROGBUF_TYPE_MESSAGE) != PROGBUF_TYPE_MESSAGE)
    return PROGBUF_ERROR_UNEXPECTED_TYPE;

  iter->read_pos++;

  size_t size;
  size_t prev_read_pos = iter->read_pos;
  int negative;
  if (read_var_size_t (iter, &size, &negative) != 0)
    {
      iter->read_pos--;
      return PROGBUF_ERROR_READ;
    }

  void *buffer = malloc (size);

  if (!buffer)
    {
      iter->read_pos -= (1 + iter->read_pos - prev_read_pos);
      return PROGBUF_ERROR_MEM_ALLOC;
    }

  memcpy (buffer, iter->buf->buffer + iter->read_pos, size);
  iter->read_pos += size;

  ulong message_tag;

  struct progbuf_s *buf = malloc (sizeof (struct progbuf_s));
  buf->buffer = buffer;

  struct progbuf_it_s tag_iter;
  tag_iter.buf = buf;
  tag_iter.read_pos = 0;

  if (read_var_ulong (&tag_iter, &message_tag, &negative) != 0)
    {
      iter->read_pos -= (1 + size + iter->read_pos - prev_read_pos);
      free (buffer);
      free (buf);
      return PROGBUF_ERROR_READ;
    }

  buf->message_tag = (negative ? -message_tag : message_tag);
  buf->header_size = determine_var_ulong_size (ABS (buf->message_tag));

  buf->size = size;
  buf->capacity = size;

  *message = buf;

  return PROGBUF_SUCCESS;
}

int
progbuf_set_string (progbuf_h buf, const char *str)
{
  int val_size, ret;

  if (!buf || !str)
    return PROGBUF_ERROR_NULL_PARAM;

  if (!buf->buffer)
    return PROGBUF_ERROR_NOT_OWNING;

  size_t len = strlen (str) + 1;
  val_size = determine_var_size_t_size (len);
  ret = check_buffer_and_expand (buf, 1 + val_size + len);
  if (ret != 0)
    return ret;

  buf->buffer[buf->size] = PROGBUF_TYPE_STRING;
  buf->size++;

  write_var_size_t (buf, len, val_size, 0);

  memcpy (buf->buffer + buf->size, str, len);
  buf->size += len;

  return PROGBUF_SUCCESS;
}

int
progbuf_get_string (progbuf_it_h iter, char **str)
{
  if (!iter || !str)
    return PROGBUF_ERROR_NULL_PARAM;

  if (!iter->buf->buffer)
    return PROGBUF_ERROR_NOT_OWNING;

  if (iter->read_pos >= iter->buf->size)
    return PROGBUF_ERROR_END_OF_ITER;

  char val_type = iter->buf->buffer[iter->read_pos];

  if ((val_type & PROGBUF_TYPE_STRING) != PROGBUF_TYPE_STRING)
    return PROGBUF_ERROR_UNEXPECTED_TYPE;

  iter->read_pos++;

  size_t str_len;
  size_t prev_read_pos = iter->read_pos;
  int negative;
  if (read_var_size_t (iter, &str_len, &negative) != 0)
    {
      iter->read_pos--;
      return PROGBUF_ERROR_READ;
    }

  char *str_buf = malloc (str_len);

  if (!str_buf)
    {
      iter->read_pos -= (1 + iter->read_pos - prev_read_pos);
      return PROGBUF_ERROR_MEM_ALLOC;
    }

  memcpy (str_buf, iter->buf->buffer + iter->read_pos, str_len);
  iter->read_pos += str_len;

  *str = str_buf;

  return PROGBUF_SUCCESS;
}

progbuf_h
progbuf_alloc (long message_tag)
{
  struct progbuf_s *buf = malloc (sizeof (struct progbuf_s));

  if (!buf)
    return 0;

  buf->buffer = malloc (PROGBUF_INIT_BUFSIZ);
  if (!buf->buffer)
    {
      free (buf);
      return 0;
    }

  buf->capacity = PROGBUF_INIT_BUFSIZ;
  buf->size = 0;
  buf->message_tag = message_tag;

  int tag_size = determine_var_ulong_size (ABS (message_tag));
  write_var_ulong (buf, ABS (message_tag), tag_size, message_tag < 0);
  buf->header_size = tag_size;

  return buf;
}

progbuf_h
progbuf_from_buffer (void *buffer, size_t size)
{
  if (!buffer || size == 0)
    return 0;

  ulong u_value;
  int negative;

  struct progbuf_s *buf = malloc (sizeof (struct progbuf_s));
  buf->buffer = buffer;

  struct progbuf_it_s iter;
  iter.buf = buf;
  iter.read_pos = 0;

  if (read_var_ulong (&iter, &u_value, &negative) != 0)
    {
      free (buf);
      return 0;
    }

  buf->message_tag = (negative ? -u_value : u_value);
  buf->header_size = determine_var_ulong_size (ABS (buf->message_tag));

  buf->size = size;
  buf->capacity = size;

  /* update buffer with the new address */
  buf->buffer = malloc (size);
  if (!buf->buffer)
    {
      free (buf);
      return 0;
    }

  memcpy (buf->buffer, buffer, size);

  return buf;
}

progbuf_it_h
progbuf_iter_alloc (progbuf_h buf)
{
  if (!buf)
    return 0;

  if (!buf->buffer)
    return 0;

  struct progbuf_it_s *iter = malloc (sizeof (struct progbuf_it_s));

  if (!iter)
    return 0;

  iter->buf = buf;
  iter->read_pos = buf->header_size;

  return iter;
}

int
progbuf_free (progbuf_h buf)
{
  if (!buf)
    return PROGBUF_ERROR_NULL_PARAM;

  if (buf->buffer)
    free (buf->buffer);

  free (buf);

  return PROGBUF_SUCCESS;
}

int
progbuf_iter_free (progbuf_it_h iter)
{
  if (!iter)
    return PROGBUF_ERROR_NULL_PARAM;

  free (iter);

  return PROGBUF_SUCCESS;
}

int
progbuf_message_tag (progbuf_h buf, long *message_tag)
{
  if (!buf || !message_tag)
    return PROGBUF_ERROR_NULL_PARAM;

  if (!buf->buffer)
    return PROGBUF_ERROR_NOT_OWNING;

  *message_tag = buf->message_tag;

  return PROGBUF_SUCCESS;
}

int
progbuf_own_buffer (progbuf_h buf, void **buffer, size_t *size)
{
  if (!buf || !buffer || !size)
    return PROGBUF_ERROR_NULL_PARAM;

  if (!buf->buffer)
    return PROGBUF_ERROR_NOT_OWNING;

  *buffer = buf->buffer;
  buf->buffer = 0;

  *size = buf->size;

  return 0;
}

int
progbuf_copy_buffer (progbuf_h buf, void **buffer, size_t *size)
{
  if (!buf || !buffer || !size)
    return PROGBUF_ERROR_NULL_PARAM;

  if (!buf->buffer)
    return PROGBUF_ERROR_NOT_OWNING;

  *buffer = malloc (buf->size);

  if (!*buffer)
    return PROGBUF_ERROR_MEM_ALLOC;

  memcpy (*buffer, buf->buffer, buf->size);

  *size = buf->size;

  return 0;
}

int
progbuf_buffer_size (progbuf_h buf, size_t *size)
{
  if (!buf || !size)
    return PROGBUF_ERROR_NULL_PARAM;

  if (!buf->buffer)
    return PROGBUF_ERROR_NOT_OWNING;

  *size = buf->size;

  return 0;
}

int
progbuf_iter_reset (progbuf_it_h iter)
{
  if (!iter)
    return PROGBUF_ERROR_NULL_PARAM;

  if (!iter->buf->buffer)
    return PROGBUF_ERROR_NOT_OWNING;

  iter->read_pos = iter->buf->header_size;

  return PROGBUF_SUCCESS;
}
