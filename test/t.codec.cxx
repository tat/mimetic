#include <string>
#include <iostream>
#include <iterator>
#include <mimetic/codec/base64.h>
#include <mimetic/codec/qp.h>
#include <mimetic/codec/code.h>
#include <mimetic/codec/codec_base.h>
#include <mimetic/codec/codec_chain.h>
#include "t.codec.h"

namespace mimetic
{
using namespace std;


void test_codec::one()
{
    Base64::Decoder b64d;
    string o;
    string s("YWJjZGUBZmdoaQJsbW5vA3BxcgRzdHUFdno=");
    code(s.begin(),s.end(), b64d , back_inserter(o));
}

void test_codec::lf2crlf()
{
    Lf2CrLf l2cl;
    string src("a\nb\r\nc\n\n");
    string exp("a\r\nb\r\nc\r\n\r\n");
    string got;
    code(src.begin(), src.end(), l2cl , back_inserter(got));
    TEST_ASSERT_EQUALS_P(exp, got);
}

void test_codec::toUpper()
{
    ToUpperCase tue;
    string src("abc=\n");
    string exp("ABC=\n");
    string got;
    code(src.begin(), src.end(), tue , back_inserter(got));
    TEST_ASSERT_EQUALS_P(exp, got);
}

void test_codec::toLower()
{
    ToLowerCase tle;
    string src("ABC=\n");
    string exp("abc=\n");
    string got;
    code(src.begin(), src.end(), tle , back_inserter(got));
    TEST_ASSERT_EQUALS_P(exp, got);
}

void test_codec::maxLine()
{
    MaxLineLen mll(4);
    string src("abcdefghij");
    string exp("abcd\r\nefgh\r\nij");
    string got;
    code(src.begin(), src.end(), mll , back_inserter(got));
    TEST_ASSERT_EQUALS_P(exp, got);
}

}
