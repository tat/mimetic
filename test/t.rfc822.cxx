#include "t.rfc822.h"
#include <string>

namespace mimetic
{
using namespace std;



void testRfc822::testGroup()
{
    struct test_item
    {
        enum { max_n = 16 };
        const char *input, *name, *mailboxes[max_n];
    };
    static const test_item data[] = {
        {
             "empty: ;", 
               "empty", 
            { 0 } 
        },
        {
             "name: e@mail.com;", 
               "name", 
            {"e@mail.com", 0 } 
        },
        {
             " group name : e@mail.com, b@mail.com", 
               "group name", 
            {"e@mail.com", 0 } 
        },
        {
             "group(comment)name: e@mail.com   , se@cond.com, th@ird.com;", 
               "group(comment)name", 
            {"e@mail.com", "se@cond.com", "th@ird.com", 0 } 
        },

        { 0, 0, {0} }
        };
    for( int i = 0; data[i].input; ++i)
    {
        Group grp(data[i].input);
        TEST_ASSERT_EQUALS_M(grp.name(), data[i].name,
            PRINT_ON_FAILURE_2(grp.name(), data[i].name) );
        Group::const_iterator bit = grp.begin(), eit = grp.end();
        for( int t = 0; data[i].mailboxes[t]; ++t, ++bit)
        {
            TEST_ASSERT( bit != eit );
            const Mailbox& mbx = *bit;
            /*
            cerr << mbx.mailbox() << endl;
            cerr << mbx.domain() << endl;
            cerr << mbx.str() << endl;
            */
            TEST_ASSERT_EQUALS_M(mbx, data[i].mailboxes[t],
                PRINT_ON_FAILURE_3(t, mbx.str(), data[i].mailboxes[t]) );
        }
    }

}

void testRfc822::testMailbox()
{
    struct test_item
    {
        const char *input, *mailbox, *domain, *label, *sourceroute;
    };
    static const test_item data[] = {
        { "e@mail.com", "e", "mail.com", "", "" },
        { "<e@mail.com>", "e", "mail.com", "", "" },
        { "(My Name) <e@mail.com>", "e", "mail.com", "(My Name)", "" },
        { "descr <e@mail.com>", "e", "mail.com", "descr", "" },
        { "descr <e@mail.com>", "e", "mail.com", "descr", "" },
        { "\"with a space\"@mail.com", "\"with a space\"", 
          "mail.com", "", "" },
        { "<\"with a space\"@mail.com>", "\"with a space\"", 
          "mail.com", "", "" },
        { "name surname <@dom1.com@dom2.com:e@mail.com>", "e", 
          "mail.com", "name surname", "@dom1.com@dom2.com" },
        // with comments
        { "de(this is a comment)scr <e(boom)@mail.com>", "e", 
          "mail.com", "de(this is a comment)scr", "" },
        { "<e(boom). (boo)d@mail.com>", "e.d", "mail.com", "", "" },
        { "Bella Ragga <e(boom). (boo)d@mail.com>", "e.d", 
          "mail.com", "Bella Ragga", "" },
        { 0,0,0,0,0 }
        };
    // test parser
    for( int i = 0; data[i].input; ++i)
    {
        Mailbox mbx(data[i].input);
        TEST_ASSERT_EQUALS_M(mbx.mailbox(), data[i].mailbox, 
            PRINT_ON_FAILURE_4(
                i, 
                data[i].input, 
                mbx.mailbox(), 
                data[i].mailbox));
        TEST_ASSERT_EQUALS_M(mbx.domain(), data[i].domain,
            PRINT_ON_FAILURE_4(
                i, 
                data[i].input, 
                mbx.domain(), 
                data[i].domain));
        TEST_ASSERT_EQUALS_M(mbx.label(), data[i].label, 
            PRINT_ON_FAILURE_4(
                i, 
                data[i].input, 
                mbx.label(), 
                data[i].label));
        TEST_ASSERT_EQUALS_M(mbx.sourceroute(), data[i].sourceroute,
            PRINT_ON_FAILURE_4(
                i, 
                data[i].input, 
                mbx.sourceroute(), 
                data[i].sourceroute));
    }
    
    // test == and != operators
    for( int i = 0; data[i].input; ++i)
    {
        Mailbox mbx, mbxParsed(data[i].input);
        mbx.mailbox(data[i].mailbox);
        mbx.domain(data[i].domain);
        mbx.label(data[i].label);
        mbx.sourceroute(data[i].sourceroute);
        TEST_ASSERT_EQUALS_M(mbxParsed, mbx,
            PRINT_ON_FAILURE_4(
                i, 
                data[i].input, 
                mbx.str(),
                mbxParsed.str()));
    }
}

void testRfc822::testAddress()
{
    Address a("e@mail.com"), b;
    int i;
    TEST_ASSERT(!a.isGroup());

    b.set("e@mail.com");
    TEST_ASSERT_EQUALS(a, b);

    a.set("<e@mail.com>");
    TEST_ASSERT(!a.isGroup());

    a.set(" desc <e@mail.com>");
    TEST_ASSERT(!a.isGroup());
    
    a.set("group label: e@mail.com; <b@aa.com>; blah <email@di.com>; ");
    TEST_ASSERT(a.isGroup());

    b.set("group label: ;");
    TEST_ASSERT(b.isGroup());

}


}
