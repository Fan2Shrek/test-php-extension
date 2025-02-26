/* test extension for PHP */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "php_test.h"
#include "test_arginfo.h"

/* For compatibility with older PHP versions */
#ifndef ZEND_PARSE_PARAMETERS_NONE
#define ZEND_PARSE_PARAMETERS_NONE() \
	ZEND_PARSE_PARAMETERS_START(0, 0) \
	ZEND_PARSE_PARAMETERS_END()
#endif

ZEND_DECLARE_MODULE_GLOBALS(test)

PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("test.scale", "1", PHP_INI_ALL, OnUpdateLong, scale, zend_test_globals, test_globals)
PHP_INI_END()

PHP_FUNCTION(test1)
{
	ZEND_PARSE_PARAMETERS_NONE();

	php_printf("The extension %s is loaded and working!\r\n", "test");
}

PHP_FUNCTION(test2)
{
	char *var = "World";
	size_t var_len = sizeof("World") - 1;
	zend_string *retval;

	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_STRING(var, var_len)
	ZEND_PARSE_PARAMETERS_END();

	retval = strpprintf(0, "Hello %s", var);

	RETURN_STR(retval);
}

PHP_FUNCTION(la_verite)
{
	ZEND_PARSE_PARAMETERS_NONE();

    RETURN_STR(strpprintf(0, "Fan2Shrek est le meilleur dev :)\r\n"));
}

static int do_scale(zval *return_value, zval *x, zend_long factor)
{
    if (Z_TYPE_P(x) == IS_LONG) {
        RETVAL_LONG(Z_LVAL_P(x) * factor);
    } else if (Z_TYPE_P(x) == IS_DOUBLE) {
        RETVAL_DOUBLE(Z_DVAL_P(x) * factor);
    } else if (Z_TYPE_P(x) == IS_STRING) {
        zend_string *ret = zend_string_safe_alloc(Z_STRLEN_P(x), factor, 0, 0);

        char *p = ZSTR_VAL(ret);
        while(factor-- > 0) {
            memcpy(p, Z_STRVAL_P(x), Z_STRLEN_P(x));
            p += Z_STRLEN_P(x);
        }
        *p = '\000';
        RETVAL_STR(ret);
    } else if (Z_TYPE_P(x) == IS_ARRAY) {
        zend_array *ret = zend_new_array(zend_array_count(Z_ARR_P(x)));
        zend_ulong idx;
        zend_string *key;
        zval *val, tmp;

        ZEND_HASH_FOREACH_KEY_VAL(Z_ARR_P(x), idx, key, val) {
            if (do_scale(&tmp, val, factor) == FAILURE) {
                return FAILURE;
            }

            if (key) {
                zend_hash_add(ret, key, &tmp);
            } else {
                zend_hash_index_add(ret, idx, &tmp);
            }
        } ZEND_HASH_FOREACH_END();
        RETVAL_ARR(ret);
    } else {
        php_error_docref(NULL, E_WARNING, "Unsupported type");
        return FAILURE;
    }

    return SUCCESS;
}

PHP_FUNCTION(test_scale)
{
    zval *x;
    zend_long factor = TEST_G(scale); // default value

    ZEND_PARSE_PARAMETERS_START(1, 2)
        Z_PARAM_ZVAL(x)
        Z_PARAM_OPTIONAL
        Z_PARAM_LONG(factor)
    ZEND_PARSE_PARAMETERS_END();
    do_scale(return_value, x, factor);
}

// Unusued
PHP_RINIT_FUNCTION(test)
{
#if defined(ZTS) && defined(COMPILE_DL_TEST)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif

	return SUCCESS;
}

PHP_MINIT_FUNCTION(test)
{
    REGISTER_INI_ENTRIES();

	return SUCCESS;
}

PHP_MINFO_FUNCTION(test)
{
	php_info_print_table_start();
	php_info_print_table_row(2, "test support", "enabled");
	php_info_print_table_end();
}

static PHP_GINIT_FUNCTION(test)
{
#if defined(COMPILE_DL_BCMATH) && defined(ZTS)
    ZEND_TSRMLS_CACHE_UPDATE();
#endif
    test_globals->scale = 1;
}

zend_module_entry test_module_entry = {
	STANDARD_MODULE_HEADER,
	"test",					/* Extension name */
	ext_functions,					/* zend_function_entry */
	PHP_MINIT(test),							/* PHP_MINIT - Module initialization */
	NULL,							/* PHP_MSHUTDOWN - Module shutdown */
	NULL,  			/* PHP_RINIT - Request initialization */
	NULL,							/* PHP_RSHUTDOWN - Request shutdown */
	PHP_MINFO(test),			/* PHP_MINFO - Module info */
	PHP_TEST_VERSION,		/* Version */
    PHP_MODULE_GLOBALS(test),  /* Module globals */
    PHP_GINIT(test),           /* PHP_GINIT – Globals initialization */
    NULL,                      /* PHP_GSHUTDOWN – Globals shutdown */
    NULL,
    STANDARD_MODULE_PROPERTIES_EX
};

#ifdef COMPILE_DL_TEST
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(test)
#endif
