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
progbuf_h progbuf_from_buffer (char *buffer, size_t size);

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

int progbuf_set_size_t (progbuf_h buf, size_t value);
int progbuf_get_size_t (progbuf_it_h iter, size_t *value);

int progbuf_set_float (progbuf_h buf, float value);
int progbuf_get_float (progbuf_it_h iter, float *value);

int progbuf_set_double (progbuf_h buf, double value);
int progbuf_get_double (progbuf_it_h iter, double *value);

int progbuf_set_int_array (progbuf_h buf, const int *arr, size_t len);
int progbuf_get_int_array (progbuf_it_h iter, int **arr, size_t *len);
int progbuf_set_uint_array (progbuf_h buf, const unsigned int *arr, size_t len);
int progbuf_get_uint_array (progbuf_it_h iter, unsigned int **arr, size_t *len);

int progbuf_set_long_array (progbuf_h buf, const long *arr, size_t len);
int progbuf_get_long_array (progbuf_it_h iter, long **arr, size_t *len);
int progbuf_set_ulong_array (progbuf_h buf, const unsigned long *arr, size_t len);
int progbuf_get_ulong_array (progbuf_it_h iter, unsigned long **arr, size_t *len);

int progbuf_set_longlong_array (progbuf_h buf, const long long *arr, size_t len);
int progbuf_get_longlong_array (progbuf_it_h iter, long long **arr, size_t *len);
int progbuf_set_ulonglong_array (progbuf_h buf, const unsigned long long *arr, size_t len);
int progbuf_get_ulonglong_array (progbuf_it_h iter, unsigned long long **arr, size_t *len);

int progbuf_set_size_t_array (progbuf_h buf, const size_t *arr, size_t len);
int progbuf_get_size_t_array (progbuf_it_h iter, size_t **arr, size_t *len);

int progbuf_set_float_array (progbuf_h buf, const float *arr, size_t len);
int progbuf_get_float_array (progbuf_it_h iter, float **arr, size_t *len);

int progbuf_set_double_array (progbuf_h buf, const double *arr, size_t len);
int progbuf_get_double_array (progbuf_it_h iter, double **arr, size_t *len);

progbuf_it_h progbuf_iter_alloc (progbuf_h buf);
int progbuf_iter_free (progbuf_it_h iter);
int progbuf_iter_reset (progbuf_it_h iter);

#endif
