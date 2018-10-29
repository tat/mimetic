#ifndef _T_CODEC_H_
#define _T_CODEC_H_
#include <string>
#include "cutee.h"


namespace mimetic
{

struct TEST_CLASS( test_codec )
{
    void TEST_FUNCTION( one );
    void TEST_FUNCTION( lf2crlf );
    void TEST_FUNCTION( toUpper );
    void TEST_FUNCTION( toLower);
    void TEST_FUNCTION( maxLine );
};

}

#endif

