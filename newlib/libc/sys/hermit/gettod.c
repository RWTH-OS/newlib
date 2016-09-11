/*
 * Copyright (c) 2011, Stefan Lankes, RWTH Aachen University
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *    * Neither the name of the University nor the names of its contributors
 *      may be used to endorse or promote products derived from this
 *      software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include <reent.h>
#include <_ansi.h>
#include <_syslist.h>
#include <sys/time.h>
#include <sys/times.h>
#include <errno.h>
#include "warning.h"

extern unsigned int get_cpufreq(void);
static unsigned long long start_tsc;
static unsigned long long freq = 0;

#if 1
inline static unsigned long long rdtsc(void)
{
	unsigned int lo, hi;

	asm volatile ("rdtsc" : "=a"(lo), "=d"(hi) :: "memory");

	return ((unsigned long long)hi << 32ULL | (unsigned long long)lo);
}
#else
inline static unsigned long long rdtsc(void)
{
	unsigned int lo, hi;
	unsigned int id;

	asm volatile ("rdtscp" : "=a"(lo), "=c"(id), "=d"(hi));

	return ((unsigned long long)hi << 32ULL | (unsigned long long)lo);
}
#endif

__attribute__((constructor)) static void gettod_init(void)
{
	start_tsc = rdtsc();
	freq = get_cpufreq() * 1000000ULL;
}

int
_DEFUN (gettimeofday, (ptimeval, ptimezone),
        struct timeval  *ptimeval  _AND
        void *ptimezone)
{
	return _gettimeofday_r(_REENT, ptimeval, ptimezone);
}

int
_DEFUN (_gettimeofday_r, (ptr, ptimeval, ptimezone),
	struct _reent *ptr _AND
        struct timeval  *ptimeval  _AND
        void *ptimezone)
{
	if (ptimeval) {
		unsigned long long diff = rdtsc() - start_tsc;

		ptimeval->tv_sec = diff / freq;
		ptimeval->tv_usec = ((diff - ptimeval->tv_sec * freq) * 1000000ULL) / freq;
	}

	if (ptimezone) {
		ptr->_errno = ENOSYS;
		return -1;
	}

	return 0;
}
