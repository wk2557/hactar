AC_PREREQ([2.63])

AC_INIT([hactar], [0.1.1], [sam.yuen@foxmail.com])

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile])
AC_CONFIG_MACRO_DIR([m4])

AC_PROG_CXX
AC_PROG_CC
AC_PROG_LIBTOOL

AC_CHECK_LIB([pthread], [pthread_create])
AC_CHECK_HEADERS([stdint.h stdlib.h pthread.h])
AC_TYPE_UINT64_T
AC_TYPE_INT64_T

AM_INIT_AUTOMAKE
LT_INIT

AC_OUTPUT
