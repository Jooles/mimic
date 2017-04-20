# SYNOPSIS
#
#   AX_CXX_HAVE_OPTIONAL()
#
# DESCRIPTION
#
#   This macro checks if std::optional or std::experimental::optional are
#   defined in the <optional> or <experimental/optional> headers.
#
#   If it is, define the ax_cv_cxx_have_optional environment variable to
#   "yes" and define HAVE_CXX_OPTIONAL.
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

AC_DEFUN([AX_CXX_HAVE_OPTIONAL],
  [AC_CACHE_CHECK(
    [for std::nullopt_t in optional],
    ax_cv_cxx_have_optional,
    [dnl
      AC_LANG_PUSH([C++])
      AC_COMPILE_IFELSE([AC_LANG_PROGRAM(
        [
          [#include <optional>]
          [using std::nullopt_t;]
        ],
        []
        )],
        [ax_cv_cxx_have_optional=yes],
        [ax_cv_cxx_have_optional=no]
      )
    AC_LANG_POP([C++])])
    if test x"$ax_cv_cxx_have_optional" = "xyes"
    then
      AC_DEFINE(HAVE_CXX_OPTIONAL,
        1,
        [Define if optional defines the std::nullopt_t class.])
    fi
  AC_CACHE_CHECK(
    [for std::experimental::nullopt_t in experimental/optional],
    ax_cv_cxx_have_experimental_optional,
    [dnl
      AC_LANG_PUSH([C++])
      AC_COMPILE_IFELSE([AC_LANG_PROGRAM(
        [
          [#include <experimental/optional>]
          [using std::experimental::nullopt_t;]
        ],
        []
        )],
        [ax_cv_cxx_have_experimental_optional=yes],
        [ax_cv_cxx_have_experimental_optional=no]
      )
    AC_LANG_POP([C++])])
    if test x"$ax_cv_cxx_have_experimental_optional" = "xyes"
    then
      AC_DEFINE(HAVE_CXX_EXPERIMENTAL_OPTIONAL,
        1,
        [Define if optional defines the std::experimental::nullopt_t class.])
    fi
  ])
