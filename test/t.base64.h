#ifndef _T_BASE64_H_
#define _T_BASE64_H_
#include <sstream>
#include <string>
#include <iterator>
#include <mimetic/codec/codec.h>
#include "cutee.h"

namespace mimetic 
{

class TEST_CLASS( test_base64 )
{
    static const char* test[][2];
public:
    void TEST_FUNCTION( testEncode )
    {
        int i = 0;
        while(test[i][1] != 0)
        {
            std::string src = test[i][0];
            std::string exp = test[i][1];
            std::string got;
            Base64::Encoder base64;
            //base64.m_maxlen = 12;
            encode(src.begin(), src.end(),base64, std::back_inserter<std::string>(got));
            TEST_ASSERT_EQUALS_P(exp, got);
            i++;
        }
    }
    void TEST_FUNCTION( testEncodeBlock )
    {
        int i = 0;
        while(test[i][1] != 0)
        {
            std::string src = test[i][0];
            std::string exp = test[i][1];
            std::string got;
            Base64::Encoder base64;
            //base64.m_maxlen = 12;
            base64.process(src.begin(), src.end(), std::back_inserter<std::string>(got));
            TEST_ASSERT_EQUALS_P( exp, got);
            i++;
        }
    }
    void TEST_FUNCTION( testDecode )
    {
        int i = 0;
        while(test[i][1] != 0)
        {
            std::string src = test[i][1];
            std::string exp = test[i][0];
            std::string got;
            Base64::Decoder base64;
            decode(src.begin(), src.end(),base64,std::back_inserter<std::string>(got));
            TEST_ASSERT_EQUALS_P( exp, got);
            i++;
        }
    }
    void TEST_FUNCTION( testDecodeBlock )
    {
        int i = 0;
        while(test[i][1] != 0)
        {
            std::string src = test[i][1];
            std::string exp = test[i][0];
            std::string got;
            Base64::Decoder base64;
            base64.process(src.begin(), src.end(),std::back_inserter<std::string>(got));
            TEST_ASSERT_EQUALS_P( exp, got);
            i++;
        }
    }
};

}

#endif
