#ifndef LIBPROGBUF_COMMON_H
#define LIBPROGBUF_COMMON_H

#include <stddef.h>

#define ABS(N) ((N<0)?(-N):(N))

#define PROGBUF_INIT_BUFSIZ       128

#define PROGBUF_TYPE_VAR_INT     0x00
#define PROGBUF_TYPE_FLOAT32     0x01
#define PROGBUF_TYPE_FLOAT64     0x02
#define PROGBUF_TYPE_BYTE        0x03
#define PROGBUF_TYPE_ARRAY       0x04

struct progbuf_s
{
  char *buffer;
  long message_tag;
  int header_size;
  size_t capacity;
  size_t size;
};

struct progbuf_it_s
{
  struct progbuf_s* buf;
  size_t read_pos;
};

#endif // LIBPROGBUF_COMMON_H
