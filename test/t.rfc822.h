#ifndef _T_RFC822_H_
#define _T_RFC822_H_
#include <mimetic/rfc822/rfc822.h>
#include "cutee.h"


namespace mimetic
{

struct TEST_CLASS( testRfc822 )
{
    void TEST_FUNCTION( testAddress );
    void TEST_FUNCTION( testMailbox );
    void TEST_FUNCTION( testGroup );
};

}

#endif

