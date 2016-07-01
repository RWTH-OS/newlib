/*
 * signal.c
 * Original Author:	G. Haley
 *
 * signal associates the function pointed to by func with the signal sig. When
 * a signal occurs, the value of func determines the action taken as follows:
 * if func is SIG_DFL, the default handling for that signal will occur; if func
 * is SIG_IGN, the signal will be ignored; otherwise, the default handling for
 * the signal is restored (SIG_DFL), and the function func is called with sig
 * as its argument. Returns the value of func for the previous call to signal
 * for the signal sig, or SIG_ERR if the request fails.
 */

/* _init_signal initialises the signal handlers for each signal. This function
   is called by crt0 at program startup.  */


/*
 * Adapted for HermitCore by:
 *  - S. Lankes, RWTH Aachen University
 *  - Daniel Krebs, RWTH Aachen University
 */

#include <errno.h>
#include <signal.h>
#include <stddef.h>
#include <stdlib.h>
#include <reent.h>
#include <malloc.h>
#include <_syslist.h>

int
_DEFUN (_init_signal_r, (ptr),
	struct _reent *ptr)
{
	int i;

	if (ptr->_sig_func == NULL) {
		ptr->_sig_func = (_sig_func_ptr *)_malloc_r (ptr, sizeof (_sig_func_ptr) * NSIG);

		if (ptr->_sig_func == NULL) {
			return -1;
		}

		for (i = 0; i < NSIG; i++) {
			ptr->_sig_func[i] = SIG_DFL;
		}
	}

	return 0;
}

_sig_func_ptr
_DEFUN (_signal_r, (ptr, sig, func),
	struct _reent *ptr _AND
	int sig _AND
	_sig_func_ptr func)
{
	_sig_func_ptr old_func;

	if (sig < 0 || sig >= NSIG) {
		ptr->_errno = EINVAL;
		return SIG_ERR;
	}

	if (ptr->_sig_func == NULL && _init_signal_r (ptr) != 0) {
		return SIG_ERR;
	}

	old_func = ptr->_sig_func[sig];
	ptr->_sig_func[sig] = func;

	return old_func;
}

int
_DEFUN(_sigaction_r, (ptr, int, const struct sigaction *, struct sigaction *),
	struct _reent *ptr _AND
	int sig _AND
	const struct sigaction * act _AND
	struct sigaction * oact)
{
	return 0;
}

#ifndef _REENT_ONLY

int
_DEFUN(sigaction, (int, const struct sigaction *, struct sigaction *),
	int sig _AND
	const struct sigaction * act _AND
	struct sigaction * oact)
{
	return _sigaction_r (_REENT, sig, act, oact);
}

_sig_func_ptr
_DEFUN (signal, (sig, func),
	int sig _AND
	_sig_func_ptr func)
{
	return _signal_r (_REENT, sig, func);
}

int 
_DEFUN_VOID (_init_signal)
{
	return _init_signal_r (_REENT);
}

#endif
