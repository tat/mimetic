#ifndef _T_STRUTILS_H_
#define _T_STRUTILS_H_
#include <string>
#include <mimetic/strutils.h>
#include "cutee.h"


namespace mimetic
{

struct TEST_CLASS( testStrUtils  )
{
    void TEST_FUNCTION( test_remove_external_blanks )
    {
        struct test_item { const char *in, *out; };
        test_item data[] = { 
                { "", "" },
                { "a", "a" },
                { "aaaabbbb", "aaaabbbb" },
                { " a", "a" },
                { "a ", "a" },
                { " a ", "a" },
                { " abcd ", "abcd" },
                { "aaaabbbb", "aaaabbbb" },
                { "   aaaabbbb", "aaaabbbb" },
                { "   aaaabbbb   ", "aaaabbbb" },
                { 0, 0 }
                };
        for(int i = 0; data[i].in; ++i)
        {
            std::string s(data[i].in);
            std::string r = remove_external_blanks(s);
            TEST_ASSERT_EQUALS_P(r, data[i].out);

        }

    }
};

}

#endif

