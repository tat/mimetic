#include "t.tokenizer.h"
#include <string>

namespace mimetic
{
using namespace std;

test_item<2> test_tokenizer::test_2[] = { 
    { "uno/due", "/", {"uno", "due"} }, 
    { "uno/", "/", { "uno", "" } },
    { "/due", "/", {"", "due"} },
    { "/", "/", { "", "" } },    
    { "", "/", { "", "" } },

    { "uno/due;", "/;", {"uno", "due"} },
    { "uno/", "/;", { "uno", "" } },
    { "/due", "/;", {"", "due"} },
    { "/", "/;", { "", "" } },    
    { "", "/;", { "", "" } },

    { "uno;due", ",;", {"uno", "due"} },
    { "uno,due", ",;", {"uno", "due"} },
    { "unodue", ",;", {"unodue", ""} },
    { ";unodue", ",;", {"", "unodue"} },
    { 0,0, {0} }

    };

void test_tokenizer::testTwo()
{
    for(int i =0 ; test_2[i].input != 0; ++i)
    {
        const int pc = test_item<2>::part_count;
        string ss, input;
        input = test_2[i].input;
        StringTokenizer stok(&input, test_2[i].delims);
        for(int t =0; t < pc; ++t)
        {
            stok.next(ss);
            TEST_ASSERT_EQUALS(ss, test_2[i].part[t]);
        }
    }
}

}
