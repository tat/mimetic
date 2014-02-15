#ifndef _T_COMPOSITE_H_
#define _T_COMPOSITE_H_
#include <sstream>
#include <iterator>
#include <algorithm>
#include <string>
#include <cctype>
#include <mimetic/codec/codec.h>
#include "cutee.h"


namespace mimetic 
{

class TEST_CLASS( test_composite )
{
    static const char* test[][2];
    std::string uppercase(const std::string& s)
    {
        std::string r;
        for(uint i=0;i<s.length();i++)
            r+=toupper(s[i]);
        return r;
    }
    std::string lowercase(const std::string& s)
    {
        std::string r;
        for(uint i=0;i<s.length();i++)
            r+=tolower(s[i]);
        return r;
    }
public:
    void TEST_FUNCTION( testBase64ToUpperCase )
    {
        int i = 0;
        while(test[i][0] != 0)
        {
            std::string src = test[i][0];
            std::string exp = uppercase(test[i][1]);
            std::string got;
            Base64::Encoder base64;
            ToUpperCase tuc;
            encode(src.begin(), src.end(),
                base64 | tuc,
                std::back_inserter<std::string>(got));
            TEST_ASSERT_EQUALS_P( exp, got);
            i++;
        }
    }
    void TEST_FUNCTION( testBase64ToLowerCase )
    {
        int i = 0;
        while(test[i][0] != 0)
        {
            std::string src = test[i][0];
            std::string exp = lowercase(test[i][1]);
            std::string got;
            Base64::Encoder base64;
            ToLowerCase tlc;
            encode(src.begin(), src.end(),
                base64 | tlc,
                std::back_inserter<std::string>(got));
            TEST_ASSERT_EQUALS_P( exp, got);
            i++;
        }
    }
};

}

#endif
