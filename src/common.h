#ifndef LIBPROGBUF_COMMON_H
#define LIBPROGBUF_COMMON_H

struct progbuf_s
{
  int message_tag;
  int version;
  char *buffer;
  int bufsiz;
};

#endif // LIBPROGBUF_COMMON_H
