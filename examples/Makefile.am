EXTRA_DIST=test.msg
INCLUDES=-I$(top_srcdir)
LDADD=../mimetic/libmimetic.la
EXAMPLES=b64 qp exbin structure catpart buildidx search mbox mm \
    smime2xml smimestruct

clean-local:
	rm -f b64 qp exbin structure catpart buildidx search mbox mm \
        smime2xml smimestruct

examples: $(EXAMPLES)

EXTRA_PROGRAMS=$(EXAMPLES)

b64_SOURCES=b64.cxx

qp_SOURCES=qp.cxx

exbin_SOURCES=exbin.cxx

structure_SOURCES=structure.cxx

catpart_SOURCES=catpart.cxx

buildidx_SOURCES=buildidx.cxx

search_SOURCES=search.cxx

mbox_SOURCES=mbox.cxx

mm_SOURCES=mm.cxx mm.h engine.h engine.cxx cmdline.cxx cmdline.h

smimestruct_SOURCES=smimestruct.cxx
smimestruct_LDADD=-lcrypto -lssl ../mimetic/libmimetic.la

smime2xml_SOURCES=smime2xml.cxx
smime2xml_LDADD=-lcrypto -lssl ../mimetic/libmimetic.la

