#include <_ansi.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "sys/syscall.h"

int errno;

int __trap0 ();

#define TRAP0(f, p1, p2, p3) __trap0(f, (p1), (p2), (p3))

int
_write ( int file,
	 char *ptr,
	 size_t len)
{
  return TRAP0 (SYS_write, file, ptr, len);
}
