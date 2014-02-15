#ifndef _T_UTILS_H_
#define _T_UTILS_H_
#include <iostream>
#include <fstream>
#include <iterator>
#include <iomanip>
#include <string>
#include <sstream>
#include <list>
#include <mimetic/utils.h>
#include "cutee.h"


namespace mimetic
{


struct TEST_CLASS( t_utils )
{
    void TEST_FUNCTION( t_int2str )
    {
        using namespace std;
        for(int i = -123; i < 123; ++i)
        {
            stringstream ss;
            ss << i;
            TEST_ASSERT_EQUALS_P(ss.str(), utils::int2str(i));
        }
    }
    void TEST_FUNCTION( t_int2hex )
    {
        using namespace std;
        for(int i = -123; i < 123; ++i)
        {
            stringstream ss;
            ss << hex << i;
            TEST_ASSERT_EQUALS_P(ss.str(), utils::int2hex(i));
        }
    }
};

};

#endif

