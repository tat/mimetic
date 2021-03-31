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
        std::shared_ptr<MimeEntity> mm = MimeEntity::create();

        mm->header().from("stefano@codesink.org");
        mm->header().to("you@home.net");
        mm->header().subject("Hi!");

        mm->body().assign("Hi!\n\nThank you for trying mimetic!");
        TEST_ASSERT_EQUALS_P(mm->size(), 96);
        m_out << std::endl << SEP << *mm << std::endl;
    }
    void TEST_FUNCTION( testMimeBinaryQpAttach )
    {
        std::shared_ptr<MimeEntity> me = MimeEntity::create();
        QP::Encoder qp;
        me->body().load("file.bin", qp);
        TEST_ASSERT_EQUALS_P(me->size(), 50650);
        m_out << std::endl << SEP << *me << std::endl;
    }
    void TEST_FUNCTION( testMimeBinaryQpBinaryAttach )
    {
        std::shared_ptr<MimeEntity> me = MimeEntity::create();
        QP::Encoder qp(true);
        me->body().load("file.bin", qp);
        TEST_ASSERT_EQUALS_P(me->size(), 50650);
        m_out << std::endl << SEP << *me << std::endl;
    }
    void TEST_FUNCTION( testMimeBinaryBase64Attach )
    {
        std::shared_ptr<MimeEntity> me = MimeEntity::create();
        Base64::Encoder b64;
        me->body().load("file.bin", b64);
        TEST_ASSERT_EQUALS_P(me->size(), 30631);
        m_out << std::endl << SEP << *me << std::endl;
    }
    void TEST_FUNCTION( testMimeMessageRfc822 )
    {
        std::shared_ptr<MimeEntity> mm = MimeEntity::create();
        mm->header().from("stefano@codesink.org");
        mm->header().to("you@home.net");
        mm->header().subject("Hi!");
        mm->body().assign("Hi!\n\nThank you for trying mimetic!");

        std::shared_ptr<MessageRfc822> mr = MessageRfc822::create(mm);
        TEST_ASSERT_EQUALS_P(mr->size(), 126);
        m_out << std::endl << SEP << *mr << std::endl;
    }
    void TEST_FUNCTION( testMimeHeaderFold )
    {
        /* test for the bug reported by M.Pinna (2008-09-23)*/

        string in(100, 'X');

        std::shared_ptr<MimeEntity> mm = MimeEntity::create();
        mm->header().subject(in);

        std::ostringstream buf;
        buf << *mm;
        string str = buf.str();

        std::shared_ptr<MimeEntity> mm2 = MimeEntity::create(str.begin(), str.end());
        string out = mm2->header().subject();

        TEST_ASSERT_EQUALS_P(in, out);
        m_out << std::endl << SEP << *mm << std::endl;
    }
};

};

#endif

