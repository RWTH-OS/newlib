/* libc/sys/hermit/sys/ucontext.h - get and set user thread context */

/*
 * Written 2016 by Stefan Lankes
 *
 * derived from libc/sys/linux/sys/ucontext.h
 */


#ifndef _SYS_UCONTEXT_H
#define _SYS_UCONTEXT_H

#include <stdint.h>
#include <signal.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	uint32_t cwd;
	uint32_t swd;
	uint32_t twd;
	uint32_t fip;
	uint32_t fcs;
	uint32_t foo;
	uint32_t fos;
	uint32_t st_space[20];
	uint32_t status;
} i387_fsave_t;

typedef struct {
	uint16_t cwd;
	uint16_t swd;
	uint16_t twd;
	uint16_t fop;
	union {
		struct {
			uint64_t rip;
			uint64_t rdp;
		};
		struct {
			uint32_t fip;
			uint32_t fcs;
			uint32_t foo;
			uint32_t fos;
		};
	};
	uint32_t mxcsr;
	uint32_t mxcsr_mask;
	uint32_t st_space[32];
	uint32_t xmm_space[64];
	uint32_t padding[12];
	union {
		uint32_t padding1[12];
		uint32_t sw_reserved[12];
	};
} i387_fxsave_t __attribute__ ((aligned (16)));

typedef struct {
	uint64_t xstate_bv;
	uint64_t xcomp_bv;
	uint64_t reserved[6];
} xsave_header_t;

typedef struct {
	uint32_t ymmh_space[64];
} ymmh_t;

typedef struct {
	i387_fxsave_t fxsave;
	xsave_header_t hdr;
	ymmh_t ymmh;
} xsave_t __attribute__ ((aligned (64)));

union fpu_state {
	i387_fsave_t    fsave;
	i387_fxsave_t   fxsave;
	xsave_t xsave;
};

typedef struct mregs {
	uint64_t r15;
	uint64_t r14;
	uint64_t r13;
	uint64_t r12;
	uint64_t r9;
	uint64_t r8;
	uint64_t rdi;
	uint64_t rsi;
	uint64_t rbp;
	uint64_t rbx;
	uint64_t rdx;
	uint64_t rcx;
	uint64_t rsp;
	uint64_t rip;
} mregs_t;

typedef struct ucontext {
	mregs_t		uc_mregs;
	union fpu_state	uc_fpu;
	struct ucontext	*uc_link;
	stack_t		uc_stack;
} ucontext_t;

int getcontext(ucontext_t *ucp);
int setcontext(const ucontext_t *ucp);
void makecontext(ucontext_t *ucp, void (*func)(), int argc, ...);
int swapcontext(ucontext_t *oucp, const ucontext_t *ucp);

#ifdef __cplusplus
}
#endif

#endif
