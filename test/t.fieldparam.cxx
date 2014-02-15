#include "t.fieldparam.h"
#include <string>

namespace mimetic
{
using namespace std;
struct test_item
{
    const char *input, *field, *value;
};

static test_item testData[] = { 
    { "field=value", "field", "value"}, 
    { "field =value", "field", "value"}, 
    { "field = value", "field", "value"}, 
    { " field = value", "field", "value"}, 
    { "  field = value", "field", "value"}, 
    { " field =value", "field", "value"}, 
    { " field =value  ", "field", "value"}, 
    { "\tfield =value  ", "field", "value"}, 
    { "\t field =value  ", "field", "value"}, 
    { "\t field = \t value\t\t ", "field", "value"}, 
    { 0,0, 0 }
    };

void testFieldParam::parser()
{
    for(int i = 0; testData[i].input; ++i)
    {
        FieldParam p(testData[i].input);
        TEST_ASSERT_EQUALS(p.name(), testData[i].field);
        TEST_ASSERT_EQUALS(p.value(), testData[i].value);
    }

}

}
