/***************************************************************************
    copyright            : (C) by 2003-2004 Stefano Barbato
    email                : stefano@codesink.org

    $Id: cutee.cxx,v 1.2 2008-10-07 11:47:37 tat Exp $
 ***************************************************************************/
#include <iostream>
#include <iterator>
#include <fstream>
#include <algorithm>
#include <string>
#include <list>
#include <ctype.h>
#include <unistd.h>
#include "cutee.h"

#define DEFAULT_RUNNER_EXT     __FILE__
#define RUNTEST_NAME        "runtest"
#define CPP_EXT            " cc cxx cpp c++ CC CXX CPP C++ "

using namespace std;

// c++ unit testing easy environment
const char* g_usage = 
"cutee, C++ Unit Testing Easy Environment, Ver. " CUTEE_VERSION  "\n\
Copyright (c) 2003-2004 by Stefano Barbato - All rights reserved.\n\
Usage: cutee [options]... testfile(s)\n\
 -m     generates runtest source code\n\
 -o     specify the output filename\n\
 -p     generates autocutee.mk to include in your Makefile\n\
 -k     generates autocutee.mk to include in your Makefile.am\n\n";

typedef list<string> StringList;

#define die(msg) do { do_die_if(1, msg, __LINE__); } while(0);
#define die_if(a, msg) do { do_die_if(a, msg, __LINE__); } while(0);
void do_die_if(int b, const string& msg, int line)
{
    if(!b)
        return;
    cerr << "(" << line << ") " << msg << endl;
    exit(1);
}
#define _( code ) of << code << endl

enum {     
    MODE_RUNTEST, 
    MODE_MAIN, 
    MODE_AUTOMAKEFILE, 
    MODE_MAKEFILE  
};

struct CmdLineOpts
{
    string ifile, ofile, ext;
    string class_name;
    StringList ifList;
    int mode;

    CmdLineOpts()
    : ext(DEFAULT_RUNNER_EXT),mode(MODE_RUNTEST)
    {
    }
    void parse(int argc, char **argv)
    {
        int ret, i;
        mode = MODE_RUNTEST; // default
        while((ret=getopt(argc, argv, "mo:pk")) != -1)
        {
            switch(ret)
            {
            case 'o':
                ofile = optarg;
                break;
            case 'm':
                mode = MODE_MAIN;
                break;
            case 'k':
                mode = MODE_AUTOMAKEFILE;
                break;
            case 'p':
                mode = MODE_MAKEFILE;
                break;
            default:
                die(g_usage);
            }
        }
        switch(mode)
        {
        case MODE_RUNTEST:
            die_if( (argc - optind) != 1 || ofile.empty(), g_usage);
            for(; optind < argc; optind++)
                ifile = argv[optind];
            break;
        case MODE_MAIN:
            die_if( (argc - optind) != 0 || ofile.empty(), g_usage);
            break;
        case MODE_AUTOMAKEFILE:
            die_if( (argc - optind) < 1 || ofile.empty(), g_usage);
            for(i = 0; optind < argc; i++, optind++)
                ifList.push_back(argv[optind]);
            break;
        case MODE_MAKEFILE:
            die_if( (argc - optind) < 1 || ofile.empty(), g_usage);
            for(i = 0; optind < argc; i++, optind++)
                ifList.push_back(argv[optind]);
            break;
        };
    }
};


class String2WordList
{
    list<string> mList;
    string mS;
public:
    typedef list<string> WordList;
    typedef list<string>::iterator iterator;
    typedef list<string>::const_iterator const_iterator;

    String2WordList(const string& s)
    : mS(s)
    {
        if(mS.empty())
            return;
        split();
    }
    inline bool isWordChar(char c) const
    {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || 
            (c >= '0' && c <= '9') || (c == '_');
    }
    void split()
    {
        string word;
        string::const_iterator beg = mS.begin(), end = mS.end();
        for( ; beg != end; ++beg)
        {
            if(isWordChar(*beg))    
                word += *beg;
            else {
                if(word.empty())
                    continue;
                mList.push_back(word);
                word.erase(word.begin(), word.end());
            }
        }
        if(!word.empty())
            mList.push_back(word);
    }
    iterator begin()         {    return mList.begin();    }
    iterator end()            {    return mList.end();    }
    const_iterator begin() const    {    return mList.begin();    }
    const_iterator end() const    {    return mList.end();    }
};

struct GenMain
{
    GenMain(const string& ofqn)
    : mFqn(ofqn)
    {
    }
    void writeMain()
    {
        ofstream of(mFqn.c_str());
        die_if(!of.is_open(), 
            string("Unable to open output file: ") + mFqn);

        _( "#include <iostream>" );
        _( "#include <iomanip>" );
        _( "#include \"cutee.h\"" );
        _( "using namespace std;" );
        _( "using namespace cutee;" );
        _( "" );
        _( "// static vars initialization" );
        _( "CuteeTest* TestList::list[MAX_TEST_COUNT];" );
        _( "int TestList::list_idx = 0;" );
        _( "" );
        _( "int main(int argc, char **argv) " );
        _( "{" );
        _( "\tRunner r(argc, argv);" );
        _( "\tr.run();" );
        _( "}" );
    }
private:
    string mFqn;
};

struct GenRunTest
{
    GenRunTest(const string& ifqn, const string& ofqn)
    : m_ifqn(ifqn), mFqn(ofqn)
    {
    }
    void writeRunTestClass()
    {
        ofstream of(mFqn.c_str());
        die_if(!of.is_open(), 
            string("Unable to open output file: ") + mFqn);
        string cn = m_className, fqcn;
        if(m_nsList.empty())
            fqcn = cn;
        else {        
            StringList::const_iterator 
                    beg = m_nsList.begin(),
                    end = m_nsList.end();
            for(; beg != end; ++beg)
                fqcn += *beg + "::";
            fqcn += cn;
        }
        _( "#include \"cutee.h\"" );
        _( "#include \"" << m_ifqn << "\"" );
        _( "using namespace cutee;" );
        _( "struct run_" << cn << ": public " << fqcn);
        _( "{" );
        _( "  void run()" );
        _( "  {" );
        of << "    ";
        _( "    setUp();" );
        StringList::const_iterator 
                    beg = m_fnList.begin(),
                    end = m_fnList.end();
        for(; beg != end; ++beg)
            _( "    run_" << *beg << "();" );
        _( "    tearDown();" );
        _( "  }" );
        _( "  uint count() { return " << m_fnList.size() << "; }" );
        _( "};" );
        _("static struct "<<cn<<"_add_to_list: public cutee::TestList");
        _( "{" );
        _( "  " << cn << "_add_to_list()" );
        _( "  {" );
        _( "  list[list_idx++] = new run_" << cn << ";" );
        _( "  }" );
        _( "} " << cn << "_addit;" );
    }
    int parseInputFile()
    {
        enum { 
            START, GET_NAMESPACE_NAME, GET_CLASS_NAME, 
            WAITING_FUNCTION, GET_FUNCTION_NAME
        };
        ifstream inf(m_ifqn.c_str());
        die_if(!inf.is_open(), 
            string("Unable to open input file: ") + m_ifqn);

        int state = START;
        string line;
        while(getline(inf, line))
        {
            string w, uw, prev;
            String2WordList s2wl(line);
            String2WordList::iterator 
                    beg = s2wl.begin(), end = s2wl.end();
            while( beg != end )
            {
                prev = w;
                w = *beg;
                switch(state)
                {
                case START:
                    if(w == "namespace" && prev != "using")
                        state = GET_NAMESPACE_NAME;
                    else if(w == "TEST_CLASS")
                        state = GET_CLASS_NAME;
                    break;
                case GET_NAMESPACE_NAME:
                    m_nsList.push_back(w);
                    state = START;
                    break;
                case GET_CLASS_NAME:
                    m_className = w;
                    state = WAITING_FUNCTION;
                    break;
                case WAITING_FUNCTION:
                    if(w == "TEST_FUNCTION")
                        state = GET_FUNCTION_NAME;
                    break;
                case GET_FUNCTION_NAME:
                    m_fnList.push_back(w);
                    state = WAITING_FUNCTION;
                    break;
                default:
                    die("Unknown state");
                }
                ++beg;
            }
        }
        return !m_className.empty();
    }
private:
    string m_ifqn, mFqn, m_className;
    StringList m_nsList, m_fnList;
};

struct GenMakefile
{
    GenMakefile(const StringList& ifList, const string& ofqn, const string& ext)
    : mFqn(ofqn), mExt(ext), mIfList(ifList)  
    {
    }
    void writeMakefile()
    {
        string cc_exts = CPP_EXT;
        ofstream of(mFqn.c_str());
        die_if(!of.is_open(), 
            string("Unable to open output file: ") + mFqn);

        _( "# cutee autogen: begin" );
        _( "CUTEE=./cutee" );

        of << "object_files=";
        StringList::const_iterator 
                beg = mIfList.begin(),
                end = mIfList.end();
        for(; beg != end; ++beg)
        {
            GenRunTest grt(*beg, "");
            if(grt.parseInputFile())
                of << stripExt(*beg) << "." << stripExt(mExt) 
                    << ".o" << " ";
            else if(cc_exts.find(" " + getExt(*beg) + " ") != string::npos)
                of << stripExt(*beg) << "." << "o" << " ";
        }
        of << endl;

        beg = mIfList.begin(),    end = mIfList.end();
        for(; beg != end; ++beg)
        {
            _( "" );
            GenRunTest grt(*beg, "");
            if(grt.parseInputFile())
            {
                string dst = stripExt(*beg) + "." + mExt;
                of << dst << ": " << *beg << endl;
                of << "\t$(CUTEE) -o " << dst << " " << *beg 
                << endl;
            }

        }
        of << endl;

        _( "" );
        _( RUNTEST_NAME ".cxx: cutee" );
        _( "\t$(CUTEE) -m -o "RUNTEST_NAME".cxx" );
        _( "" );
        _( RUNTEST_NAME": autocutee.mk " RUNTEST_NAME ".o $(object_files)");
        _( "\t$(CXX) $(CXXFLAGS) $(LDFLAGS) -o "RUNTEST_NAME" "RUNTEST_NAME".o $(object_files)");
        _( "\t./"RUNTEST_NAME );
        _( "" );
        _( "# cutee autogen: end ");
    }
private:
    string getExt(const string& fqn)
    {
        string::size_type idx =    fqn.find_last_of('.');
        if(idx != string::npos)
            return string(fqn, ++idx);
        else
            return string();
    }
    string stripExt(const string& fqn)
    {
        string::size_type idx =    fqn.find_last_of('.');
        if(idx != string::npos)
            return string(fqn, 0, idx);
        else
            return fqn;
    }
    string mFqn, mExt;
    StringList mIfList;
};

struct GenAutomakefile
{
    GenAutomakefile(const StringList& ifList, const string& ofqn, const string& ext)
    : mFqn(ofqn), mExt(ext), mIfList(ifList)
    {
    }
    void writeMakefile()
    {
        ofstream of(mFqn.c_str());
        die_if(!of.is_open(), 
            string("Unable to open output file: ") + mFqn);

        _( "# cutee autogen: begin" );
        _( "CUTEE=./cutee" );

        of << "t_runners=";
        StringList::const_iterator 
                beg = mIfList.begin(),
                end = mIfList.end();
        for(; beg != end; ++beg)
        {
            GenRunTest grt(*beg, "dummy");
            if(grt.parseInputFile())
                of << stripPath(stripExt(*beg)) << "." << mExt << " ";
        }
        of << endl;

        _( "" );
        _( "%.cutee.cxx: $(srcdir)/%.h" );
        _( "\t$(CUTEE) -o $@ $<");

        _( "" );
        _( "" );
        _( RUNTEST_NAME ".cxx: cutee" );
        _( "\t$(CUTEE) -m -o $@" );

        _( "" );
        _( RUNTEST_NAME "-clean:");
        _( "\trm -f autocutee.mk cutee *.o *.cutee.cxx "RUNTEST_NAME" "RUNTEST_NAME".cxx");
        _( "\ttouch autocutee.mk");

        _( "" );
        _( "EXTRA_PROGRAMS="RUNTEST_NAME );
        _( RUNTEST_NAME "_SOURCES="RUNTEST_NAME".cxx $(test_files) $(t_runners)");
        _( RUNTEST_NAME"_DEPENDENCIES=cutee autocutee.mk" );
        _( "# cutee autogen: end ");
    }
private:
    string stripPath(const string& fqn)
    {
        string::size_type idx =    fqn.find_last_of('/');
        if(idx != string::npos)
            return string(fqn, ++idx);
        else
            return fqn;
    }
    string stripExt(const string& fqn)
    {
        string::size_type idx =    fqn.find_last_of('.');
        if(idx != string::npos)
            return string(fqn, 0, idx);
        else
            return fqn;
    }
    string mFqn, mExt;
    StringList mIfList;
};
//
// main
//
int main(int argc, char** argv)
{
    CmdLineOpts clo;
    clo.parse(argc,argv);
    GenRunTest* pGrt;
    GenMain* pGm;
    GenAutomakefile* pGamk;
    GenMakefile* pGmk;


    switch(clo.mode)
    {
    case MODE_RUNTEST:
        // i'm using new/delete to avoid a warning (jump to case 
        // label crosses initialization of ...) that throw an
        // internal compiler error using gcc 3.2(linux)
        pGrt = new GenRunTest(clo.ifile, clo.ofile);
        if(pGrt->parseInputFile())
            pGrt->writeRunTestClass();
        delete pGrt;
        break;
    case MODE_MAIN:
        pGm = new GenMain(clo.ofile);
        pGm->writeMain();
        delete pGm;
        break;
    case MODE_AUTOMAKEFILE:
        pGamk = new GenAutomakefile(clo.ifList, clo.ofile, clo.ext);
        pGamk->writeMakefile();
        delete pGamk;
        break;
    case MODE_MAKEFILE:
        pGmk = new GenMakefile(clo.ifList, clo.ofile, clo.ext);
        pGmk->writeMakefile();
        delete pGmk;
        break;
    default:
        die("UNKNOWN MODE");
        break;
    }
    return 0;
}


