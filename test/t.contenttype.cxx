#include <mimetic/contenttype.h>
#include <string>
#include "t.contenttype.h"

namespace mimetic
{
using namespace std;

void contenttype::parser()
{
    ContentType * pCt;
    pCt = new ContentType("multipart/mixed; boundary=\"--abcdef\"");
    TEST_ASSERT_EQUALS_P(pCt->type(), "MuLtIpArT");
    TEST_ASSERT_EQUALS_P(pCt->subtype(), "MIXed");
    TEST_ASSERT_EQUALS_P(pCt->param("BOUNDary"), "--abcdef");
    delete pCt;

    pCt = new ContentType("multipart/mixed;\t boundary=\"--abcdef\";");
    TEST_ASSERT_EQUALS_P(pCt->type(), "MuLtIpArT");
    TEST_ASSERT_EQUALS_P(pCt->subtype(), "MIXed");
    TEST_ASSERT_EQUALS_P(pCt->param("BOUNDary"), "--abcdef");
    delete pCt;
}

}
