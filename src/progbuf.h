#ifndef LIBPROGBUF_H
#define LIBPROGBUF_H

typedef struct progbuf_s *progbuf_h;

progbuf_h progbuf_alloc (int message_tag, int version);
void progbuf_free (progbuf_h buf);
int progbuf_set_int (progbuf_h buf, int value);

#endif
