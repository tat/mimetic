MAINTAINERCLEANFILES = Makefile Makefile.in config.cache
EXTRA_DIST=LICENSE m4
SUBDIRS = mimetic doc examples test win32
INCLUDES=-I$(top_srcdir)
ACLOCAL_AMFLAGS=-I m4

dist-hook:
	rm -f $(distdir)/test/autocutee.mk
	touch $(distdir)/test/autocutee.mk
	rm -f $(distdir)/test/*.cutee.cxx
	rm -f $(distdir)/test/*.cutee.o
