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
    ToUpperCase tue;
    MaxLineLen mll(44);
    Base64::Encoder b64e;
    QP::Encoder qpe;
    Base64::Decoder b64d;
    string o;
    string s("YWJjZGUBZmdoaQJsbW5vA3BxcgRzdHUFdno=");
    code(s.begin(),s.end(), b64d , back_inserter(o));
}

}
