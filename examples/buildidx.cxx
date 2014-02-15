/***************************************************************************
    copyright            : (C) 2002-2008 by Stefano Barbato
    email                : stefano@codesink.org

    $Id: buildidx.cxx,v 1.3 2008-10-07 11:06:25 tat Exp $
 ***************************************************************************/

/** \example buildidx.cc
 *  extract a Part based on command line parameters
 *  more info on:
 *      buildidx -h
 */ 
#include <iostream>
#include <sstream>
#include <iterator>
#include <fstream>
#include <cassert>
#include <sys/types.h>
#include <dirent.h>
#include <mimetic/mimetic.h>

using namespace std;
using namespace mimetic;

unsigned int g_indexed = 0;

void usage()
{
    cout << "buildidx [file|dir...]" << endl;
    exit(-1);
}

void die(bool b, const string& msg)
{
    if(b)
    {
        cerr << "Error: " << msg << endl << endl;
        usage();
    }
}

void indexFile(MimeEntity& head, const string& fqn)
{
    File in(fqn);
    if(in)
    {
        MimeEntity* pMe = new MimeEntity;
        pMe->load(in.begin(), in.end(), ~imHeader);
        pMe->header().push_back(Field("X-Filename",fqn));
        head.body().parts().push_back(pMe);
        cerr << "." << flush;
        g_indexed++;
    } else
        cerr << "ERR: unable to open file " << fqn << endl;

}

void indexDirectory(MimeEntity& head, const string& dirFqn)
{
    DIR* dir = opendir(dirFqn.c_str());
    if(dir == 0)
    {
        cerr << "ERR: unable to read from " << dirFqn << endl;
        return;
    }
    struct dirent* item;
    struct stat st;
    string ffqn;
    while(0 != (item = readdir(dir)))
    {
        ffqn = dirFqn + "/" + item->d_name;
        memset(&st,0,sizeof(struct stat));
        if(stat(ffqn.c_str(),&st) == -1)
        {
            cerr << "ERR: unable to stat " << item->d_name<< endl;
            return;
        }
        if(S_ISREG(st.st_mode) && item->d_name[0] != '.') // reg file
        {
            indexFile(head, ffqn);
        }
    }
    closedir(dir);
}

void indexItem(MimeEntity& head, const string& item)
{
    struct stat st;
    if(stat(item.c_str(), &st) == -1)
    {
        cerr << "ERR: unable to stat " << item.c_str() << endl;
        return;
    }    
    if(S_ISREG(st.st_mode)) 
        indexFile(head, item);
    else if (S_ISDIR(st.st_mode))
        indexDirectory(head, item);
    else
        cerr << "ERR: unknown file type " << item << endl;
}

int main(int argc, char** argv)
{
    std::ios_base::sync_with_stdio(false);

    if(argc > 1 && string(argv[1]) == "-h")
        usage();
    MultipartMixed head;
    string fqn;
    if(argc == 1) { 
        // read filenames from stdin
        while(getline(cin, fqn))
            indexItem(head, fqn);
    } else {
        for(int fc = 1; fc < argc; ++fc)
            indexItem(head, argv[fc]);
    }
    cout << head << endl;
    cerr << "index of " << g_indexed << " file(s) built" << endl; 
    return g_indexed;
}

