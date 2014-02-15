#ifndef _T_TOKENIZER_H_
#define _T_TOKENIZER_H_
#include <string>
#include <mimetic/tokenizer.h>
#include "cutee.h"


namespace mimetic
{

template<int sz>
struct test_item
{
    enum { part_count = sz };
    const char *input;
    const char *delims;
    const char *part[sz];
};

class TEST_CLASS( test_tokenizer )
{
    static struct test_item<2> test_2[];
    static const int test_count;
public:
    void TEST_FUNCTION( testTwo );
};

}

#endif

