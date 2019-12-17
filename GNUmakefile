
ifeq ($(wildcard Makefile),Makefile)
include Makefile
else

default: Makefile

configure:
	autoreconf -fvi

Makefile: configure
	./configure

.PHONY: default

endif

