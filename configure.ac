AC_INIT([dio], [0.0.1], [jwecker@justin.tv])
AC_CONFIG_MACRO_DIR([.m4])
AC_CONFIG_AUX_DIR([.build-aux])
AC_CANONICAL_SYSTEM
AC_CANONICAL_TARGET
AC_USE_SYSTEM_EXTENSIONS
AM_INIT_AUTOMAKE([foreign -Wall -Werror -Wno-portability])
LT_INIT
AC_PROG_CC
AC_CONFIG_SRCDIR([lib/dio.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile lib/Makefile src/Makefile])
AC_FUNC_MALLOC

case "$target_os" in
linux*)
     AC_DEFINE(LINUX,1,[This is a Linux system])
     AC_DEFINE_UNQUOTED(LINUX,1,[This is a LINUX system])
     ;;
darwin*)
     AC_DEFINE_UNQUOTED(DARWIN,1,[This is a Darwin system])
     CFLAGS="-fno-common $CFLAGS"
     ;;
esac

CFLAGS='-O3 -Wall -W -Winline -Werror -Wno-unused-parameter -Wno-unused-result'
AC_OUTPUT
