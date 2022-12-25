lib_LTLIBRARIES = libprogbuf.la
libprogbuf_la_SOURCES = progbuf.c
libprogbuf_la_LDFLAGS = -version-info 1:0:0
libprogbuf_la_CFLAGS = -Wall -Wextra -Wpedantic -Werror

pkginclude_HEADERS = progbuf.h
pkgconfig_DATA = libprogbuf.pc

noinst_PROGRAMS = check_progbuf
TESTS = check_progbuf
check_progbuf_SOURCES = check_progbuf.c
check_progbuf_CFLAGS = $(CFLAGS) $(CHECK_CFLAGS)
check_progbuf_LDADD = libprogbuf.la $(LDFLAGS) $(CHECK_LIBS)
check_progbuf_LDFLAGS = -no-install
