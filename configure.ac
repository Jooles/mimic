dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(Mimic, 0.1)


AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE([foreign])

AC_PROG_CXX

AC_CONFIG_MACRO_DIR([m4])
AX_CXX_COMPILE_STDCXX(14, noext, mandatory)

DX_INIT_DOXYGEN(Mimic)
DX_DOXYGEN_FEATURE(ON)
DX_DOT_FEATURE(ON)

AC_CONFIG_FILES(Makefile)
AC_OUTPUT

