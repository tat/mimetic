#ifndef _T_MIME_H_
#define _T_MIME_H_
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <mimetic/mimetic.h>
#include "cutee.h"


namespace mimetic
{

class TEST_CLASS( test_mime )
{
    std::ofstream m_out;
public:
#define    SEP "................................................\n"
    test_mime()
    : m_out("tests.out")
    {
    }
    void TEST_FUNCTION( testMimeEntityBuild )
    {
        MimeEntity mm;

        mm.header().from("stefano@codesink.org");
        mm.header().to("you@home.net");
        mm.header().subject("Hi!");

        mm.body().assign("Hi!\n\nThank you for trying mimetic!");
        TEST_ASSERT_EQUALS_P(mm.size(), 96);
        m_out << std::endl << SEP << mm << std::endl;
    }
    void TEST_FUNCTION( testMimeBinaryQpAttach )
    {
        MimeEntity me;
        QP::Encoder qp;
        me.body().load("file.bin", qp);
        TEST_ASSERT_EQUALS_P(me.size(), 50650);
        m_out << std::endl << SEP << me << std::endl;
    }
    void TEST_FUNCTION( testMimeBinaryQpBinaryAttach )
    {
        MimeEntity me;
        QP::Encoder qp(true);
        me.body().load("file.bin", qp);
        TEST_ASSERT_EQUALS_P(me.size(), 50650);
        m_out << std::endl << SEP << me << std::endl;
    }
    void TEST_FUNCTION( testMimeBinaryBase64Attach )
    {
        MimeEntity me;
        Base64::Encoder b64;
        me.body().load("file.bin", b64);
        TEST_ASSERT_EQUALS_P(me.size(), 30631);
        m_out << std::endl << SEP << me << std::endl;
    }
    void TEST_FUNCTION( testMimeMessageRfc822 )
    {
        MimeEntity mm;
        mm.header().from("stefano@codesink.org");
        mm.header().to("you@home.net");
        mm.header().subject("Hi!");
        mm.body().assign("Hi!\n\nThank you for trying mimetic!");

        MessageRfc822 mr(mm);
        TEST_ASSERT_EQUALS_P(mr.size(), 126);
        m_out << std::endl << SEP << mr << std::endl;
    }
    void TEST_FUNCTION( testMimeHeaderFold )
    {
        /* test for the bug reported by M.Pinna (2008-09-23)*/

        string in(100, 'X');

        MimeEntity mm;
        mm.header().subject(in);

        std::ostringstream buf;
        buf << mm;
        string str = buf.str();

        MimeEntity mm2(str.begin(), str.end());
        string out = mm2.header().subject();

        TEST_ASSERT_EQUALS_P(in, out);
        m_out << std::endl << SEP << mm << std::endl;
    }
};

};

#endif

