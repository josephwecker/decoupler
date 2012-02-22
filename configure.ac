AC_INIT([dio], [0.0.1], [jwecker@justin.tv])
AC_CONFIG_MACRO_DIR([.m4])
AC_CONFIG_AUX_DIR([.build-aux])
AM_INIT_AUTOMAKE([foreign -Wall -Werror -Wno-portability])
LT_INIT
AC_PROG_CC
AC_CONFIG_SRCDIR([lib/dio.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile lib/Makefile src/Makefile])
AC_FUNC_MALLOC
CFLAGS='-O3 -Wall -W -Winline -Werror -Wno-unused-parameter -Wno-unused-result'
AC_OUTPUT
