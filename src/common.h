#ifndef LIBPROGBUF_COMMON_H
#define LIBPROGBUF_COMMON_H

#include <stddef.h>

#define ABS(N) ((N<0)?(-N):(N))

#define PROGBUF_INIT_BUFSIZ 128

#define PROGBUF_TYPE_VAR_INT            0x01
#define PROGBUF_TYPE_FLOAT              0x02
#define PROGBUF_TYPE_DOUBLE             0x03
#define PROGBUF_TYPE_CHAR               0x04
#define PROGBUF_TYPE_RAW                0x05
#define PROGBUF_TYPE_MESSAGE            0x06
#define PROGBUF_TYPE_ARRAY              0x10
#define PROGBUF_TYPE_VAR_INT_ARRAY      0x11
#define PROGBUF_TYPE_VAR_FLOAT_ARRAY    0x12
#define PROGBUF_TYPE_VAR_DOUBLE_ARRAY   0x13
#define PROGBUF_TYPE_STRING             0x14

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
