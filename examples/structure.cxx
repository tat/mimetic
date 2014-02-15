/***************************************************************************
    copyright            : (C) 2002-2008 by Stefano Barbato
    email                : stefano@codesink.org

    $Id: structure.cxx,v 1.6 2008-10-07 11:06:25 tat Exp $
 ***************************************************************************/

/** \example structure.cc
 * Usage: structure [-ed] [in_file [out_file]]
 *
 * Reads in_file (or standard input) and writes its MIME structure to out_file
 * (or standard output)
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <streambuf>
#include <mimetic/mimetic.h>
#include <mimetic/utils.h>

using namespace std;
using namespace mimetic;

int g_verbose; // verbose mode on/off
int g_quiet; // quiet mode
int g_entityCount; // num of entities found

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
        cout << g_entityCount << " ";
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



void usage()
{
    cout << "structure [-v] [in_file]..." << endl;
    cout << "    -v Verbose mode" << endl;
    cout << "    -q totaly quiet; exit code = num of entities" << endl;
    cout << endl;
    exit(1);
}

int main(int argc, char** argv)
{
    std::ios_base::sync_with_stdio(false);
    int fidx = 1;
    int iMask = imBody | imPreamble | imEpilogue;

    if(argc > 1)
    {    
        g_verbose = 0;
        string first = argv[1];
        if(first == "-h")
            usage();
        else if(first == "-v")
            g_verbose = 1;
        else if(first == "-q")
            g_quiet = 1;
        fidx = (g_verbose || g_quiet ? 2 : 1); // first filename idx
    }
    if(argc == fidx)
    {
        istreambuf_iterator<char> bit(std::cin), eit;
        MimeEntity me(bit,eit, iMask);
        printMimeStructure(&me);
    } else {
        for(int fc = fidx; fc < argc; ++fc)
        {
            File in(argv[fc]);
            if(!in)
            {
                cerr << "ERR: unable to open file " 
                    << argv[fc]
                    << endl;
                continue;
            }
            MimeEntity me(in.begin(), in.end(),iMask);
            printMimeStructure(&me);
        }
    }
    return g_entityCount;
}

