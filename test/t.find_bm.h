#ifndef _T_FILE_BM_H_
#define _T_FILE_BM_H_
#include <sstream>
#include <iterator>
#include <algorithm>
#include <fstream>
#include <string>
#include <cctype>
#include "cutee.h"
#include <mimetic/utils.h>


namespace mimetic 
{

class TEST_CLASS( test_find_bm )
{
    struct test_s
    {
        const char* text, *pattern;
        int off;
    };
    static const test_s test[];
public:
    void TEST_FUNCTION( find )
    {
        for(int i = 0; test[i].text != 0; ++i)
        {
            const char *bit, *eit, *exp, *got;
            bit = test[i].text;
            eit = bit + strlen(bit);
            exp = bit+test[i].off;
            got = utils::find_bm(bit, eit, test[i].pattern);
            TEST_ASSERT_EQUALS_M(exp, got, std::string() << "text: " << test[i].text << " pattern: " << test[i].pattern << " exp: " << exp-bit << " got: " << got-bit);
        }
    }
};

}

#endif
