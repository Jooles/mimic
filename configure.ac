dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.68)
AC_INIT(Mimic, 0.1)


AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE([foreign])

AC_PROG_CXX
AC_PROG_RANLIB

m4_include([m4/ax_cxx_compile_stdcxx.m4])
AX_CXX_COMPILE_STDCXX(14, noext, mandatory)

AC_CONFIG_FILES(Makefile)
AC_OUTPUT

