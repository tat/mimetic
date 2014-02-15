#include <mimetic/contentdisposition.h>
#include <string>
#include "t.contentdisposition.h"

namespace mimetic
{
using namespace std;

void contentdisposition::parser1()
{
    ContentDisposition* pCd;

    pCd = new ContentDisposition("attachment; filename=d&g.jpeg;"
         "modification-date=\"Wed, 12 Feb 1997 16:29:51 -0500\";");

    TEST_ASSERT_EQUALS_P(pCd->type(), "attachment");
    TEST_ASSERT_EQUALS_P(pCd->param("filename"), "d&g.jpeg");
    TEST_ASSERT_EQUALS_P(pCd->param("modification-date"), 
        "Wed, 12 Feb 1997 16:29:51 -0500");

    delete pCd;
}

void contentdisposition::parser0()
{
    ContentDisposition* pCd;

    pCd = new ContentDisposition("attachment; filename=\"d&g.jpeg\";"
         "modification-date=\"Wed, 12 Feb 1997 16:29:51 -0500\";");

    TEST_ASSERT_EQUALS_P(pCd->type(), "attachment");
    TEST_ASSERT_EQUALS_P(pCd->param("filename"), "d&g.jpeg");
    TEST_ASSERT_EQUALS_P(pCd->param("modification-date"), 
        "Wed, 12 Feb 1997 16:29:51 -0500");

    delete pCd;
}

}
