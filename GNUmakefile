
ifeq ($(wildcard Makefile),Makefile)
include Makefile
else

default: Makefile

configure:
	autoreconf -fvi

Makefile: configure
	./configure

clean:
	rm -rf autom4te.cache maniac/autom4te.cache maniac/.deps
	rm -f aclocal.m4 compile config.h config.h.in config.log config.status configure \
	      depcomp install-sh missing stamp-h1 Makefile Makefile.in maniac/Makefile \
		  maniac/Makefile.in

.PHONY: default clean

endif

