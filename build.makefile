# -*-Makefile-*- for maintenance jobs
# Copyright (C) 2013  Kaz Nishimura

# Copying and distribution of this file, with or without modification, are
# permitted in any medium without royalty provided the copyright notice and
# this notice are preserved.  This file is offered as-is, without any
# warranty.

# This file SHOULD NOT be contained in the source package.

builddir = build
prefix = /tmp/xllmnrd

AUTORECONF = autoreconf
TAR = tar

CFLAGS = -g -O2 -Wall -Wextra

build: clean check dist
	hg status || true

all check clean dist distcheck: $(builddir)/Makefile
	cd $(builddir) && $(MAKE) CFLAGS='$(CFLAGS)' $@

install: $(builddir)/Makefile
	cd $(builddir) && \
	  $(MAKE) CFLAGS='$(CFLAGS)' DESTDIR=$$(pwd)/root $@

image: install
	@rm -f $(builddir)/xllmnrd-image.tar.gz
	(cd $(builddir)/root && $(TAR) -c -f - .) | \
	  gzip -9c > $(builddir)/xllmnrd-image.tar.gz
	rm -rf $(builddir)/root

$(builddir)/Makefile: stamp-configure build.makefile
	test -d $(builddir) || mkdir $(builddir)
	rm -f $(builddir)/xllmnrd-*.tar.*
	srcdir=$$(pwd); \
	cd $(builddir) && $$srcdir/configure --prefix=$(prefix)

configure: stamp-configure
stamp-configure: configure.ac
	@rm -f $@
	$(AUTORECONF) --install
	touch $@

.PHONY: build all check clean dist distcheck install image
