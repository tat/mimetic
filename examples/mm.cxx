/***************************************************************************
    copyright            : (C) 2002-2008 by Stefano Barbato
    email                : stefano@codesink.org

    $Id: mm.cxx,v 1.3 2008-10-07 11:06:25 tat Exp $
 ***************************************************************************/

#include <fstream>
#include <streambuf>
#include <cassert>
#include <getopt.h>
#include <mimetic/mimetic.h>
#include "mm.h"
#include "cmdline.h"
#include "engine.h"

using namespace std;
using namespace mimetic;


void print_cmd_info()
{
    static const char *cmd_info =
    "mm " MM_VERSION ", a multipurpose mail tool\n"
    "Copyright (c) 2003 by Stefano Barbato - All rights reserved.\n";
    cerr << cmd_info << endl;
}

void do_usage_if(int b, const string& file, int line)
{
    const static char *usage = 
        "WARNING: mm is still under development. It's NOT ready for production!\n"
        "Usage: mm [MATCH] [ACTION] [INFILE] [OUTFILE]\n" ;
    if(!b)
        return;
    cerr << usage << endl << "[" << file << ":" << line << "]" << endl;
    exit(1);
}

void do_die_if(int b, const std::string& msg, const string& file, int line)
{
    if(!b)
        return;
    cerr << "Error [" << file << ":" << line << "]: " << msg << endl;
    exit(1);
}


int main(int argc, char** argv)
{
    std::ios_base::sync_with_stdio(false);
    command_line cl;

    die_if(!cl.parse_cmd_line(argc, argv), "command line error");

    string fqn;
    filebuf fin;
    filebuf fout;
    if(cl.is_set("out"))
    {
        fqn = cl["out"];
        fout.open(fqn.c_str(), ios::out);
        die_if(!fout.is_open(), 
            string("unable to open output file ")+fqn);
    }
    if(cl.is_set("in"))
    {
        fqn = cl["in"];
        fin.open(fqn.c_str(), ios::in);
        die_if(!fin.is_open(), 
            string("unable to open input file ")+fqn);
    }
    print_cmd_info();
    engine e(cl);
    istream is( fin.is_open() ? &fin : cin.rdbuf());
    ostream os( fout.is_open() ? &fout : cout.rdbuf());
    istreambuf_iterator<char> ibeg(is), iend;
    ostreambuf_iterator<char> out(os);
    int iMask = (cl.is_set("ignore-child-parts") ? imChildParts: imNone);
    MimeEntity me(ibeg, iend, iMask);
    if(!e.match(me))
        cerr << "not ";
    cerr << "matched" << endl;
    return 0;
}

