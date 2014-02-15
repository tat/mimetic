EXTRA_DIST=file.bin small.bin test.msg test3.msg
INCLUDES=-I$(top_srcdir)
LDADD=../mimetic/libmimetic.la

test_files=t.version.h t.tokenizer.h t.base64.h t.qp.h t.circular_buffer.h t.file.h t.mimemessage.h t.composite.h t.tokenizer.cxx t.base64.cxx t.qp.cxx t.circular_buffer.cxx t.composite.cxx t.directory.h t.fieldparam.h t.fieldparam.cxx t.strutils.h t.rfc822.h t.rfc822.cxx t.tree.h t.parser.h t.contenttype.h t.contenttype.cxx t.utils.h t.codec.h t.codec.cxx t.find_bm.h t.find_bm.cxx t.contentdisposition.h t.contentdisposition.cxx

all-local: cutee runtest
	@for f in $(EXTRA_DIST); do \
		if [ ! -e $$f ]; then \
			ln -s $(srcdir)/$$f  ; \
		fi ; \
	done
	./runtest

clean-local: touch-autocutee.mk runtest-clean
	rm -f tests.out

noinst_PROGRAMS=cutee
cutee_SOURCES=cutee.cxx cutee.h
cutee_LDADD=

touch-autocutee.mk:
	touch autocutee.mk

autocutee.mk: cutee Makefile.am $(test_files)
	./cutee -k -o autocutee.mk $(addprefix $(srcdir)/, $(test_files))

include autocutee.mk 
