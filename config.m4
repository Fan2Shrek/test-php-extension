dnl Autotools config.m4 for PHP extension test

dnl Comments in this file start with the string 'dnl' (discard to next line).
dnl Remove where necessary.

dnl If extension references and depends on an external library package, use
dnl the '--with-test' configure option:
dnl PHP_ARG_WITH([test],
dnl   [for test support],
dnl   [AS_HELP_STRING([--with-test],
dnl     [Include test support])])

dnl Otherwise use the '--enable-test' configure option:
PHP_ARG_ENABLE([test],
  [whether to enable test support],
  [AS_HELP_STRING([--enable-test],
    [Enable test support])],
  [no])

AS_VAR_IF([PHP_TEST], [no],, [
  dnl This section is executed when extension is enabled with one of the above
  dnl configure options. Adjust and add tests here.

  dnl
  dnl Use and adjust this code block if extension depends on external library
  dnl package which supports pkg-config.
  dnl
  dnl Find library package with pkg-config.
  dnl PKG_CHECK_MODULES([LIBFOO], [foo])
  dnl
  dnl Or if you need to check for a particular library version with pkg-config,
  dnl you can use comparison operators. For example:
  dnl PKG_CHECK_MODULES([LIBFOO], [foo >= 1.2.3])
  dnl PKG_CHECK_MODULES([LIBFOO], [foo < 3.4])
  dnl PKG_CHECK_MODULES([LIBFOO], [foo = 1.2.3])
  dnl
  dnl Add library compilation and linker flags to extension.
  dnl PHP_EVAL_INCLINE([$LIBFOO_CFLAGS])
  dnl PHP_EVAL_LIBLINE([$LIBFOO_LIBS], [TEST_SHARED_LIBADD])
  dnl
  dnl Check for library and symbol presence.
  dnl LIBNAME=test # you may want to change this
  dnl LIBSYMBOL=test # you most likely want to change this
  dnl
  dnl If you need to check for a particular library function (e.g. a conditional
  dnl or version-dependent feature) and you are using pkg-config:
  dnl PHP_CHECK_LIBRARY([$LIBNAME], [$LIBSYMBOL],
  dnl   [AC_DEFINE([HAVE_TEST_FEATURE], [1],
  dnl     [Define to 1 if test has the 'FEATURE'.])],
  dnl   [AC_MSG_FAILURE([FEATURE not supported by your test library.])],
  dnl   [$LIBFOO_LIBS])
  dnl

  dnl
  dnl Or use and adjust this code block if extension depends on external library
  dnl package, which does not support pkg-config.
  dnl
  dnl Path to library package can be given as parameter (--with-test=<DIR>)
  dnl SEARCH_PATH="/usr/local /usr" # you might want to change this
  dnl SEARCH_FOR="/include/test.h" # you most likely want to change this
  dnl AS_IF([test -r $PHP_TEST/$SEARCH_FOR],
  dnl   [TEST_DIR=$PHP_TEST],
  dnl   [
  dnl     for i in $SEARCH_PATH; do
  dnl       AS_IF([test -r $i/$SEARCH_FOR],
  dnl         [TEST_DIR=$i; break;])
  dnl     done
  dnl   ])
  dnl
  dnl AC_MSG_CHECKING([for test library package])
  dnl AS_VAR_IF([TEST_DIR],, [
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the test library package])
  dnl ], [AC_MSG_RESULT([found in $TEST_DIR])])
  dnl
  dnl Add include flag where library package headers are located on the system.
  dnl PHP_ADD_INCLUDE([$TEST_DIR/include])
  dnl
  dnl Check for library and symbol presence.
  dnl LIBNAME=test # you may want to change this
  dnl LIBSYMBOL=test # you most likely want to change this
  dnl
  dnl If you need to check for a particular library function (e.g. a conditional
  dnl or version-dependent feature) and you are not using pkg-config:
  dnl PHP_CHECK_LIBRARY([$LIBNAME], [$LIBSYMBOL], [
  dnl     PHP_ADD_LIBRARY_WITH_PATH([$LIBNAME],
  dnl       [$TEST_DIR/$PHP_LIBDIR],
  dnl       [TEST_SHARED_LIBADD])
  dnl     AC_DEFINE([HAVE_TEST_FEATURE], [1],
  dnl       [Define to 1 if test has the 'FEATURE'.])
  dnl   ],
  dnl   [AC_MSG_FAILURE([FEATURE not supported by your test library.])],
  dnl   [-L$TEST_DIR/$PHP_LIBDIR -lm])
  dnl

  dnl Add linked libraries flags for shared extension to the generated Makefile.
  dnl PHP_SUBST([TEST_SHARED_LIBADD])

  dnl Define a preprocessor macro to indicate that this PHP extension can
  dnl be dynamically loaded as a shared module or is statically built into PHP.
  AC_DEFINE([HAVE_TEST], [1],
    [Define to 1 if the PHP extension 'test' is available.])

  dnl Configure extension sources and compilation flags.
  PHP_NEW_EXTENSION([test],
    [test.c],
    [$ext_shared],,
    [-DZEND_ENABLE_STATIC_TSRMLS_CACHE=1])
])
