dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.68)
AC_INIT(Mimic, 0.1)


AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE([foreign])

AC_LANG_CPLUSPLUS
AC_PROG_CXX
AC_PROG_RANLIB

m4_include([m4/ax_cxx_compile_stdcxx.m4])
AX_CXX_COMPILE_STDCXX(14, noext, mandatory)

m4_include([m4/ax_cxx_have_filesystem.m4])
AX_CXX_HAVE_FILESYSTEM
m4_include([m4/ax_cxx_have_variant.m4])
AX_CXX_HAVE_VARIANT
m4_include([m4/ax_cxx_have_optional.m4])
AX_CXX_HAVE_OPTIONAL

m4_include([m4/ax_boost_base.m4])
AX_BOOST_BASE([1.52])

AC_CHECK_LIB(stdc++fs, main)

AC_CONFIG_FILES(Makefile)
AC_OUTPUT

