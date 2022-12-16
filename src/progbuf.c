#include "progbuf.h"
#include "common.h"

#include <stdlib.h>

progbuf_h
progbuf_alloc (int message_tag, int version)
{
  struct progbuf_s *buf = calloc (1, sizeof (struct progbuf_s));
  return buf;
}

void
progbuf_free (progbuf_h buf)
{
  if (buf)
    {
      if (buf->buffer)
        free (buf->buffer);

      free (buf);
    }
}

int
progbuf_set_int (progbuf_h buf, int value)
{
  return -1;
}
