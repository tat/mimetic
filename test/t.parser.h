#ifndef _T_PARSER_H_
#define _T_PARSER_H_
#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
#include <list>
#include <mimetic/mimeentity.h>
#include <mimetic/parser/itparser.h>
#include <mimetic/os/file.h>
#include <mimetic/utils.h>
#include <string.h>
#include "cutee.h"

namespace mimetic
{

struct TEST_CLASS( parser_fixed_bugs )
{
    void TEST_FUNCTION( t0_input_iterator )
    {
        // bug: the body of multipart msgs include the newline char
        //      preceding the boundary
        using namespace std;
        ifstream ifs("test3.msg");
        TEST_ASSERT_OR_RET(ifs.is_open(), "file open error");

        MimeEntity me(ifs);
        TEST_ASSERT(me.body().parts().size());
        ContentType& ct = me.header().contentType();
        TEST_ASSERT_EQUALS_P(ct.type(), "multipart");
        TEST_ASSERT_EQUALS_P(ct.subtype(), "form-data");
        MimeEntityList& mel = me.body().parts();
        MimeEntityList::iterator bit = mel.begin(), eit = mel.end();
        for( int i = 0; bit != eit; ++bit, ++i)
        {
            stringstream ss;
            ss << setfill('0') << setw(3) << i;
            TEST_ASSERT_EQUALS_P(ss.str(), (*bit)->body());
        }
    }
    void TEST_FUNCTION( t0_random_iterator )
    {
        using namespace std;
        File in("test3.msg");
        TEST_ASSERT_OR_RET(in, "file open error");
        MimeEntity me(in.begin(), in.end());
        TEST_ASSERT(me.body().parts().size());
        ContentType& ct = me.header().contentType();
        TEST_ASSERT_EQUALS_P(ct.type(), "multipart");
        TEST_ASSERT_EQUALS_P(ct.subtype(), "form-data");
        MimeEntityList& mel = me.body().parts();
        MimeEntityList::iterator bit = mel.begin(), eit = mel.end();
        for( int i = 0; bit != eit; ++bit, ++i)
        {
            stringstream ss;
            ss << setfill('0') << setw(3) << i;
            TEST_ASSERT_EQUALS_P(ss.str(), (*bit)->body());
        }
    }
};

};

#endif

