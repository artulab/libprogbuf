lib_LTLIBRARIES = libprogbuf.la
libprogbuf_la_SOURCES = progbuf.c
libprogbuf_la_LDFLAGS = -version-info 0:1:0
include_HEADERS = progbuf.h

bin_PROGRAMS = test_progbuf
test_progbuf_SOURCES = test_progbuf.c
test_progbuf_LDADD = libprogbuf.la
