#ifndef _T_VERSION_H_
#define _T_VERSION_H_
#include <mimetic/version.h>
#include "cutee.h"


namespace mimetic 
{

class TEST_CLASS( test_version )
{
    Version v001, v01, v010, v12, v120, v123, v345;
public:
    test_version()
    : v001("0.0.1"), v01("0.1"), v010("0.1.0"),v12("1.2"), 
      v120("1.2.0"), v123("1.2.3"), v345("3.4.5")
    {
    }
    void TEST_FUNCTION( ctor )
    {
        TEST_ASSERT_EQUALS_P(v001.maj(), 0);
        TEST_ASSERT_EQUALS_P(v001.min(), 0);
        TEST_ASSERT_EQUALS_P(v001.build(), 1);
        
        TEST_ASSERT_EQUALS_P(v01.maj(), 0);
        TEST_ASSERT_EQUALS_P(v01.min(), 1);
        TEST_ASSERT_EQUALS_P(v01.build(), 0);

        TEST_ASSERT_EQUALS_P(v010.maj(), 0);
        TEST_ASSERT_EQUALS_P(v010.min(), 1);
        TEST_ASSERT_EQUALS_P(v010.build(), 0);

        TEST_ASSERT_EQUALS_P(v123.maj(), 1);
        TEST_ASSERT_EQUALS_P(v123.min(), 2);
        TEST_ASSERT_EQUALS_P(v123.build(), 3);
    }
    void TEST_FUNCTION( eq )
    {

        TEST_ASSERT_EQUALS_P(v01, v01);
        TEST_ASSERT_EQUALS_P(v01, v010);

        TEST_ASSERT(v001 <= v001);
        TEST_ASSERT(v001 <= v01);
        TEST_ASSERT(v001 <= v345);
        TEST_ASSERT(v123 <= v345);

        TEST_ASSERT(v123 >= v123);
        TEST_ASSERT(v123 >= v120);
        TEST_ASSERT(v123 >= v12);
        TEST_ASSERT(v123 >= v001);
        TEST_ASSERT(v345 >= v123);
    }
    void TEST_FUNCTION( diff )
    {
        TEST_ASSERT_DIFFERS(v123, v345);
        TEST_ASSERT_DIFFERS(v001, v12);
        TEST_ASSERT_DIFFERS(v001, v01);
        TEST_ASSERT_DIFFERS(v001, v123);
    }
    void TEST_FUNCTION( less )
    {
        TEST_ASSERT(v001 < v01);
        TEST_ASSERT(v001 < v010);
        TEST_ASSERT(v12 < v123);
        TEST_ASSERT(v12 < v345);
        TEST_ASSERT(v123 < v345);
    }
    void TEST_FUNCTION( greater )
    {
        TEST_ASSERT(v345 > v123);
        TEST_ASSERT(v345 > v12);
        TEST_ASSERT(v123 > v12);
        TEST_ASSERT(v123 > v120);
        TEST_ASSERT(v12 > v010);
        TEST_ASSERT(v12 > v01);
        TEST_ASSERT(v12 > v001);
    }
};

}

#endif
