AC_INIT([libprogbuf], [1.0], [ahmet@artulab.com])
AM_INIT_AUTOMAKE([-Wall -Werror gnu])
AC_PROG_CC
AM_PROG_AR
LT_INIT

PKG_PROG_PKG_CONFIG
PKG_INSTALLDIR

PKG_CHECK_MODULES([CHECK], [check])

AC_CONFIG_MACRO_DIRS([m4])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
 Makefile
 src/Makefile
 src/libprogbuf.pc:libprogbuf.pc.in
])
AC_OUTPUT
