#ifndef LIBPROGBUF_H
#define LIBPROGBUF_H

#include <stddef.h>

#define PROGBUF_SUCCESS                  0
#define PROGBUF_ERROR_NULL_PARAM        -1
#define PROGBUF_ERROR_MEM_ALLOC         -2
#define PROGBUF_ERROR_END_OF_ITER       -3
#define PROGBUF_ERROR_UNEXPECTED_TYPE   -4
#define PROGBUF_ERROR_READ              -5
#define PROGBUF_ERROR_INCORRECT_SIGN    -6
#define PROGBUF_ERROR_NOT_OWNING        -7

typedef struct progbuf_s *progbuf_h;
typedef struct progbuf_it_s *progbuf_it_h;

progbuf_h progbuf_alloc (long message_tag);

int progbuf_message_tag (progbuf_h buf, long *message_tag);
int progbuf_own_buffer (progbuf_h buf, char **buffer, size_t *size);
int progbuf_copy_buffer (progbuf_h buf, char **buffer, size_t *size);
int progbuf_buffer_size (progbuf_h buf, size_t *size);
int progbuf_free (progbuf_h buf);

int progbuf_set_int (progbuf_h buf, int value);
int progbuf_get_int (progbuf_it_h iter, int *value);
int progbuf_set_uint (progbuf_h buf, unsigned int value);
int progbuf_get_uint (progbuf_it_h iter, unsigned int *value);

int progbuf_set_long (progbuf_h buf, long value);
int progbuf_get_long (progbuf_it_h iter, long *value);
int progbuf_set_ulong (progbuf_h buf, unsigned long value);
int progbuf_get_ulong (progbuf_it_h iter, unsigned long *value);

int progbuf_set_longlong (progbuf_h buf, long long value);
int progbuf_get_longlong (progbuf_it_h iter, long long *value);
int progbuf_set_ulonglong (progbuf_h buf, unsigned long long value);
int progbuf_get_ulonglong (progbuf_it_h iter, unsigned long long *value);

progbuf_it_h progbuf_iter_alloc (progbuf_h buf);
int progbuf_iter_free (progbuf_it_h iter);
int progbuf_iter_reset (progbuf_it_h iter);

#endif
