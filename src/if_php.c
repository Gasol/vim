/* vi:set ts=8 sts=4 sw=4 noet:
 *
 * VIM - Vi IMproved	by Bram Moolenaar
 *
 * PHP interface by Gasol Wu
 */

#ifdef HAVE_CONFIG_H
# include "auto/config.h"
#endif
#include "vim.h"
#include <sapi/embed/php_embed.h>

static int php_initialized = 0;
static int ensure_php_initialized(void);

void ex_php(exarg_T *eap)
{
    char *script = NULL;

    if (eap->skip) {
	return;
    }

    script = (char *) script_get(eap, eap->arg);

    if (ensure_php_initialized()) {
	if (NULL == script) {
	    zend_first_try {
		zend_eval_string(eap->arg, NULL, "VIM");
	    } zend_end_try();
	} else {
	    zend_first_try {
		zend_eval_string(script, NULL, "VIM");
	    } zend_end_try();
	}
    }
    vim_free(script);
}

static int ensure_php_initialized(void)
{
    if (!php_initialized) {
	if (SUCCESS == php_embed_init(0, NULL)) {
	    php_initialized = 1;
	}
    }
    return php_initialized;
}

void php_end(void)
{
    // php_embed_shutdown();
    php_initialized = 0;
}
