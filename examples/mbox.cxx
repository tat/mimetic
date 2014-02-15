/***************************************************************************
    copyright            : (C) 2002-2008 by Stefano Barbato
    email                : stefano@codesink.org

    $Id: mbox.cxx,v 1.3 2008-10-07 11:06:25 tat Exp $
 ***************************************************************************/

/** \example mbox.cc
 *  extract a Part based on command line parameters
 *  more info on:
 *      mbox -h
 */ 
#include <iostream>
#include <sstream>
#include <memory>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <cassert>
#include <mimetic/mimetic.h>
#include <mimetic/parser/itparser.h>
//#include <mimetic/mbx/mbox.h>

using namespace std;
using namespace mimetic;

unsigned int g_matches = 0;

int g_verbose; // verbose mode on/off
int g_quiet; // quiet mode
int g_entityCount; // num of entities found

void usage()
{
    cout << "mbox [params] [in_file...]" << endl;
    exit(-1);
}


void die_if(bool b, const string& msg)
{
    if(b)
    {
        cerr << "Error: " << msg << endl << endl;
        usage();
    }
}

inline int isnl(char c)
{
    return c == '\n' || c == '\r';
}

void printTabs(int c)
{
    while(c--)
        cout << "    ";
}

void printMimeStructure(MimeEntity* pMe, int tabcount = 0)
{
    ++g_entityCount;
    if(!g_quiet)
    {
        Header& h = pMe->header();
        ContentType ct = h.contentType();
        printTabs(tabcount);
        cout << ct.type() << "/" << ct.subtype() << endl;
        if(g_verbose)
        {
            ContentType::ParamList::iterator bit, eit;
            bit = ct.paramList().begin();
            eit = ct.paramList().end();
            for(; bit != eit; ++bit)
            {
                printTabs(tabcount);
                cout << "param: " << bit->name() << " = " 
                     << bit->value() << endl;
            }
            if(h.hasField(ContentTransferEncoding::label))
            {
                printTabs(tabcount);
                cout << "encoding: " 
                     << h.contentTransferEncoding().mechanism() 
                     << endl;
            }
            if(h.hasField(ContentDisposition::label))
            {
                printTabs(tabcount);
                const ContentDisposition cd = h.contentDisposition();
                cout << "disposition: " << cd.type() << endl;
                ContentDisposition::ParamList::const_iterator 
                    bit, eit;
                bit = cd.paramList().begin();
                eit = cd.paramList().end();
                for(; bit != eit; ++bit)
                {
                    printTabs(tabcount);
                    cout << "param: " << bit->name() << " = " 
                         << bit->value() << endl;
                }
            }
            Header::iterator hbit, heit;
            hbit = pMe->header().begin();
            heit = pMe->header().end();
            for(; hbit != heit; ++hbit)
            {
                printTabs(tabcount);
                cout << "h: " << hbit->name() << " = " 
                     << hbit->value() << endl;
            }
            if(pMe->body().preamble().length())
            {
                printTabs(tabcount);
                cout << "preamble: " << pMe->body().preamble() 
                    << endl;
            }
            if(pMe->body().epilogue().length())
            {
                printTabs(tabcount);
                cout << "epilogue: " << pMe->body().epilogue() 
                    << endl;
            }
            printTabs(tabcount);
            cout << "part size: " << pMe->size() << endl;
            printTabs(tabcount);
            cout << "body size: " << pMe->body().length() << endl;
            cout << endl;
        }
    }
    MimeEntityList::iterator mbit = pMe->body().parts().begin(),
                    meit = pMe->body().parts().end();
    for(;mbit!=meit;++mbit)
        printMimeStructure(*mbit, 1 + tabcount);
}

int main(int argc, char** argv)
{
    std::ios_base::sync_with_stdio(false);

    die_if(argc < 2, "mbox filename required");
    die_if(!File::exists(argv[1]), 
        string("Unable to access file: ") + argv[1]);

    File f(argv[1]);
    File::iterator bit = f.begin(), eit = f.end(), it;
    int count = 0;
    do
    {
        // skip "From blah blah" line
        for(; !isnl(*bit); ++bit)
            ;
        if(isnl(*++bit)) 
            ++bit; // jump 2 chars new lines
        it = utils::find_bm(bit, eit, "From ");
        if(!isnl(*(it-1)))
            continue;
        MimeEntity me(bit, it);
        printMimeStructure(&me);
        bit = it;
        ++count;
    } while(bit != eit);

    cerr << count << " messages parsed" << endl;
    /*
    MboxMailbox mm(argv[1], ~Parser::imHeader);
    copy(mm.begin(), mm.end(), osi);
    cout << endl;
    return 0;

    int count;
    MboxMailbox mm(argv[1], ~Parser::imHeader);
    MboxMailbox::const_iterator mbit = mm.begin(), meit = mm.end();
    for(count = 0; mbit != meit; ++mbit, ++count, ++osi)
    {
        const MimeEntity& e = *mbit;
        *osi = *mbit;
    }
    cerr << count << " messages parsed" << endl;
    */
    return 0;
}

