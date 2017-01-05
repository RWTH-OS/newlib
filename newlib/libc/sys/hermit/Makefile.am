AUTOMAKE_OPTIONS = cygnus
INCLUDES = $(NEWLIB_CFLAGS) $(CROSS_CFLAGS) $(TARGET_CFLAGS)
AM_CCASFLAGS = $(INCLUDES)
 
noinst_LIBRARIES = lib.a

lib_a_SOURCES = errno.c chown.c environ.c execve.c fork.c getpid.c kill.c lseek.c readlink.c stat.c times.c wait.c close.c _exit.c fstat.c gettod.c isatty.c link.c open.c read.c sbrk.c symlink.c unlink.c write.c signal.c
lib_a_CCASFLAGS = $(AM_CCASFLAGS)
lib_a_CFLAGS = $(AM_CFLAGS)
 
all-local: crt0.o
 
ACLOCAL_AMFLAGS = -I ../../..
CONFIG_STATUS_DEPENDENCIES = $(newlib_basedir)/configure.host

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(tooldir)/include/netinet; \
        for i in $(srcdir)/include/netinet/*.h; do \
          $(INSTALL_DATA) $$i $(DESTDIR)$(tooldir)/include/netinet/`basename $$i`; \
        done; \
	$(mkinstalldirs) $(DESTDIR)$(tooldir)/include/arch; \
	for i in $(srcdir)/include/arch/*.h; do \
	  $(INSTALL_DATA) $$i $(DESTDIR)$(tooldir)/include/arch/`basename $$i`; \
	done; \
	$(mkinstalldirs) $(DESTDIR)$(tooldir)/include/lwip; \
        for i in $(srcdir)/include/lwip/*.h; do \
          $(INSTALL_DATA) $$i $(DESTDIR)$(tooldir)/include/lwip/`basename $$i`; \
        done; \
	$(mkinstalldirs) $(DESTDIR)$(tooldir)/include/lwip/priv; \
        for i in $(srcdir)/include/lwip/priv/*.h; do \
          $(INSTALL_DATA) $$i $(DESTDIR)$(tooldir)/include/lwip/priv/`basename $$i`; \
        done; \
	$(mkinstalldirs) $(DESTDIR)$(tooldir)/include/lwip/prot; \
        for i in $(srcdir)/include/lwip/prot/*.h; do \
          $(INSTALL_DATA) $$i $(DESTDIR)$(tooldir)/include/lwip/prot/`basename $$i`; \
        done; \
	$(mkinstalldirs) $(DESTDIR)$(tooldir)/include/lwip/apps; \
        for i in $(srcdir)/include/lwip/apps/*.h; do \
          $(INSTALL_DATA) $$i $(DESTDIR)$(tooldir)/include/lwip/apps/`basename $$i`; \
        done;
