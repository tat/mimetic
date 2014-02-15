/***************************************************************************
    copyright            : (C) 2002-2008 by Stefano Barbato
    email                : stefano@codesink.org

    $Id: b64.cxx,v 1.3 2008-10-07 11:06:25 tat Exp $
 ***************************************************************************/
/** \example b64.cc
 * Usage: b64 [-ed] [in_file [out_file]]
 *
 * Base64 encodes or decodes in_file (or standard input) and writes
 * output to out_file (or standard output)
 */
#include <iostream>
#include <iterator>
#include <fstream>
#include <mimetic/mimetic.h>

using namespace std;
using namespace mimetic;

void usage()
{
    cout << "b64 [-ed] [in_file [out_file]]" << endl;
    exit(1);
}

void die_if(bool b, const string& s)
{
    if(b)
    {
        cerr << s << endl;
        exit(-1);
    }
}

int main(int argc, char** argv)
{
    std::ios_base::sync_with_stdio(false);
    bool encoding = 0;
    if(argc < 2)
        usage();

    string opt = argv[1];    
    if(opt == "-e")
        encoding = true;
    else if(opt == "-d")
        encoding = false;
    else
        usage();
        
    filebuf fin;
    filebuf fout;
    if(argc > 2)
    {
        fin.open(argv[2], ios::in);
        die_if(!fin.is_open(), string("unable to open file ")+argv[2]);
    }
    if(argc > 3)
    {
        fout.open(argv[3], ios::out);
        die_if(!fout.is_open(), string("unable to open file ")+argv[3]);
    }
    istream is( fin.is_open() ? &fin : cin.rdbuf());
    ostream os( fout.is_open() ? &fout : cout.rdbuf());
    istreambuf_iterator<char> ibeg(is), iend;
    ostreambuf_iterator<char> out(os);
    if(encoding) {
        Base64::Encoder b64;
        code(ibeg, iend, b64, out);
    } else {
        Base64::Decoder b64;
        code(ibeg, iend, b64, out);
    }
}

