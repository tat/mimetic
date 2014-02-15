#ifndef _T_DIRECTORY_H_
#define _T_DIRECTORY_H_
#include <string>
#include <iostream>
#include <fstream>
#include "cutee.h"
#include <mimetic/os/directory.h>
#include <mimetic/os/fileop.h>
#include <mimetic/utils.h>


namespace mimetic
{

class TEST_CLASS( test_directory )
{
public:
    #define TESTDIR "__t_directory_h_testdir"
    void setUp()
    {
        TEST_ASSERT( Directory::create(TESTDIR) );
    }
    void tearDown()
    {
        TEST_ASSERT( Directory::remove(TESTDIR) );
    }
    void TEST_FUNCTION( exists )
    {
        TEST_ASSERT( Directory::exists(TESTDIR) );
        TEST_ASSERT( ! Directory::exists("__ThisDirShouldNotExist__") );
    }
    void TEST_FUNCTION( emptyDir )
    {
        // only . and .. should exist in TESTDIR
        Directory d(TESTDIR);
        Directory::iterator bit = d.begin(), eit = d.end();
        for(; bit != eit; ++bit)
        {
            TEST_ASSERT( bit->name == "." || bit->name == ".." );
        }
    }
    void TEST_FUNCTION( withFiles )
    {
        using namespace std;
        {
        ofstream of(TESTDIR "/file1");
        of << endl << endl;
        }
        Directory d(TESTDIR);
        Directory::iterator bit = d.begin(), eit = d.end();
        for(int count = 2; bit != eit; ++bit, --count)
        {
            TEST_ASSERT( 
                bit->name == "." || 
                bit->name == ".." || 
                bit->name == "file1" 
                );
            TEST_ASSERT( count >= 0); // max 2 items
        }
        FileOp::remove( TESTDIR "/file1" );
    }
    void TEST_FUNCTION( moreFiles )
    {
        using namespace std;
        unsigned long tot = 0;
        int n;
        for(n = 1; n < 100; ++n)
        {
            string fqn = TESTDIR "/";
            fqn += utils::int2str(n);
            ofstream of(fqn.c_str());
            tot += n;
        }

        Directory d(TESTDIR);
        Directory::iterator bit = d.begin(), eit = d.end();
        for(; bit != eit; ++bit)
        {
            if( bit->name == "." || bit->name == ".." )
                continue;
            n = utils::str2int(bit->name);
            tot -= n;
        }
        TEST_ASSERT( tot == 0); 

        for(n = 1; n < 100; ++n)
        {
            string fqn = TESTDIR "/";
            fqn += utils::int2str(n);
            FileOp::remove( fqn );
        }

    }
};

}

#endif

