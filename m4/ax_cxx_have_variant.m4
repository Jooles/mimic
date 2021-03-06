# SYNOPSIS
#
#   AX_CXX_HAVE_VARIANT()
#
# DESCRIPTION
#
#   This macro checks if std::variantis
#   defined in the <variant> header.
#
#   If it is, define the ax_cv_cxx_have_variant environment variable to
#   "yes" and define HAVE_CXX_VARIANT.
#
# LICENSE
#
#   Copyright (c) 2017 Julian Cromarty <julian.cromarty@gmail.com>
#
#   Copying and distribution of this file, with or without modification, are
#   permitted in any medium without royalty provided the copyright notice
#   and this notice are preserved.  This file is offered as-is, without any
#   warranty.

#serial 3

AC_DEFUN([AX_CXX_HAVE_VARIANT],
  [AC_CACHE_CHECK(
    [for std::monostate in variant],
    ax_cv_cxx_have_variant,
    [dnl
      AC_LANG_PUSH([C++])
      AC_COMPILE_IFELSE([AC_LANG_PROGRAM(
        [
          [#include <variant>]
          [using std::monostate;]
        ],
        []
        )],
        [ax_cv_cxx_have_variant=yes],
        [ax_cv_cxx_have_variant=no]
      )
    AC_LANG_POP([C++])])
    if test x"$ax_cv_cxx_have_variant" = "xyes"
    then
      AC_DEFINE(HAVE_CXX_VARIANT,
        1,
        [Define if variant defines the std::monostate class.])
    fi
  ])
