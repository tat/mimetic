#include "t.circular_buffer.h"
using namespace mimetic;

const test_circular_buffer::test_item
test_circular_buffer::tb[] = 
    {
        // { 0, "a", ""}, 
        { 1, "a", "a"}, 
        { 1, "ab", "b"}, 
        { 1, "abc", "c"}, 
        { 2, "a", "a"},
        { 2, "aa", "aa"},
        { 2, "abc", "cb"},
        { 2, "abcd", "cd"},
        { 2, "abcde", "ed"},
        { 3, "a", "a"},
        { 3, "ab", "ab"},
        { 3, "abc", "abc"},
        { 3, "abcd", "dbc"},
        { 3, "abcde", "dec"},
        { 3, "abcdef", "def"},
        { 32, "abcdefighilmno", "abcdefighilmno"}
    };

const unsigned int test_circular_buffer::test_count = sizeof(test_circular_buffer::tb)/sizeof(test_circular_buffer::test_item);
