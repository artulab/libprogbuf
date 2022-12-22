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

#define PROGBUF_SET(type, utype)                                              \
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

#define PROGBUF_GET(type, utype)                                              \
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

PROGBUF_SET (int, uint)
PROGBUF_GET (int, uint)

PROGBUF_SET (uint, uint)
PROGBUF_GET (uint, uint)

PROGBUF_SET (long, ulong)
PROGBUF_GET (long, ulong)

PROGBUF_SET (ulong, ulong)
PROGBUF_GET (ulong, ulong)

PROGBUF_SET (longlong, ulonglong)
PROGBUF_GET (longlong, ulonglong)

PROGBUF_SET (ulonglong, ulonglong)
PROGBUF_GET (ulonglong, ulonglong)

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
progbuf_from_buffer (char *buffer, size_t size)
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
progbuf_own_buffer (progbuf_h buf, char **buffer, size_t *size)
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
progbuf_copy_buffer (progbuf_h buf, char **buffer, size_t *size)
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
