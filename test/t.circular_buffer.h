#ifndef _T_CIRCULAR_BUFFER_H_
#define _T_CIRCULAR_BUFFER_H_
#include <string>
#include "cutee.h"
#include <mimetic/circular_buffer.h>


namespace mimetic
{

class TEST_CLASS( test_circular_buffer)
{
    struct test_item
    {
        int buf_sz;
        const char* push_back_items;
        const char* result;
    };
    static const test_item tb[];
    static const unsigned int test_count;
    test_item ti;
    const char* putstr;
    const char* result;
public:
    test_circular_buffer()
    : ti(tb[0])
    {
    }
    void TEST_FUNCTION( testOverflow )
    {
        circular_buffer<char> cb(3);
        for(unsigned i = 0; i < 1000; i++)
            cb.push_back('a');
    }
    void TEST_FUNCTION( testCount )
    {
        unsigned int maxsize = 64;
        circular_buffer<char> cb(maxsize);
        for(unsigned int i = 0; i < 1000; i++)
        {
            if(cb.count() < maxsize)
            {
                TEST_ASSERT_EQUALS_P( i, cb.count());
            } else {
                TEST_ASSERT_EQUALS_P( maxsize, cb.count());
            }
            cb.push_back('a');
        }
    }
    void TEST_FUNCTION( testCmpCh )
    {
        for(unsigned int i = 0 ; i < test_count; ++i)
        {
            ti = tb[i];
            putstr = ti.push_back_items;
            result = ti.result;
            circular_buffer<char> cb(ti.buf_sz);
            char c;
            while(0 != (c = *putstr++))
                cb.push_back(c);
            while(0 != (c = *result++))
            {
                TEST_ASSERT_EQUALS_P( cb.front(), c );
                cb.pop_front();
            }
        }
    }

    void TEST_FUNCTION( testInOut )
    {
        for(unsigned int i =0 ; i < test_count; ++i)
        {
            ti = tb[i];
            putstr = ti.push_back_items;
            circular_buffer<char> cb(ti.buf_sz);
            char c;
            while(0 != (c = *putstr++))
            {
                cb.push_back(c);
                TEST_ASSERT_EQUALS_P( cb.front(), c );
                cb.pop_front();
            }
        }

    }
    void TEST_FUNCTION( testFill )
    {
        for(unsigned int i =0 ; i < test_count; ++i)
        {
            ti = tb[i];
            putstr = ti.push_back_items;
            circular_buffer<char> cb(ti.buf_sz);
            char c;
            const char * cmp = putstr;
            while(0 != (c = *putstr++))
            {
                cb.push_back(c);
                if(cb.count() < cb.max_size()) continue;
                c = *cmp++;
                TEST_ASSERT_EQUALS_P( cb.front(), c );
                cb.pop_front();
            }
        }
    }
    void TEST_FUNCTION( testIdx )
    {
        for(unsigned int i =0 ; i < test_count; i++)
        {
            ti = tb[i];
            putstr = ti.push_back_items;
            result = ti.result;
            circular_buffer<char> cb(ti.buf_sz);
            char c;
            while(0 != (c = *putstr++))
                cb.push_back(c);
            for(unsigned int t = 0 ; t < cb.count(); ++t)
            {
                TEST_ASSERT_EQUALS_P( cb[t], result[t]);
            }
        }
    }
    void TEST_FUNCTION( testEq )
    {
        for(unsigned int i =0 ; i < test_count; i++)
        {
            ti = tb[i];
            putstr = ti.push_back_items;
            result = ti.result;
            circular_buffer<char> cb(ti.buf_sz);
            char c;
            while(0 != (c = *putstr++))
                cb.push_back(c);
            // test == and != operators
            TEST_ASSERT( cb == result );
            TEST_ASSERT( false == (cb != result) );
        }

    }
    void TEST_FUNCTION( testCompare )
    {
        for(unsigned int i =0 ; i < test_count; i++)
        {
            ti = tb[i];
            putstr = ti.push_back_items;
            //result = ti.result;
            std::string res = ti.result;
            circular_buffer<char> cb(ti.buf_sz);
            char c;
            while(0 != (c = *putstr++))
                cb.push_back(c);
            for(unsigned int c = 0; c < res.length(); c++)
            {
                std::string subs = res.substr(c);
                TEST_ASSERT( cb.compare(c,subs.length(),subs) );
            }
            for(unsigned int c = 0; c < res.length(); c++)
            {
                std::string subs = res.substr(c, res.length()-c);
                TEST_ASSERT( cb.compare(c,subs.length(),subs) );
            }
            for(unsigned int c = res.length(); c != 0;  c--)
            {
                std::string subs = res.substr(0,c);
                TEST_ASSERT( cb.compare(0,subs.length(),subs) );
            }
            for(unsigned int c = res.length(); c != 0;  c--)
            {
                TEST_ASSERT( cb.compare(0,c,res) );
            }
        }

    }
    void TEST_FUNCTION( testToStr )
    {
        for(unsigned int i =0 ; i < test_count; ++i)
        {
            ti = tb[i];
            putstr = ti.push_back_items;
            result = ti.result;
            circular_buffer<char> cb(ti.buf_sz);
            char c;
            while(0 != (c = *putstr++))
                cb.push_back(c);
            TEST_ASSERT_EQUALS_P( cb.str(), result );
        }
    }

};

}

#endif

