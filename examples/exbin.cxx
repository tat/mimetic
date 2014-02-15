/***************************************************************************
    copyright            : (C) 2002-2008 by Stefano Barbato
    email                : stefano@codesink.org

    $Id: exbin.cxx,v 1.5 2008-10-07 11:06:25 tat Exp $
 ***************************************************************************/

/** \example exbin.cc
 *  extract a Part based on command line parameters
 *  more info on:
 *      exbin -h
 */ 
#include <iostream>
#include <sstream>
#include <iterator>
#include <fstream>
#include <cassert>
#include <mimetic/mimetic.h>

using namespace std;
using namespace mimetic;

unsigned int g_decoded = 0, g_messages = 0, g_files = 0, g_parts = 0;

void usage()
{
    cout << "exbin [params] [in_file...]" << endl;
    cout << "\t-t type[/subtype] matches Content-Type" << endl;
    cout << "\t-q                totaly quiet; exit code = num of decoded entities" << endl;
    exit(-1);
}


struct MatchParamRq
{
    MatchParamRq()
    : m_quiet(0), m_set(0)
    {
    }
    bool operator()(const MimeEntity* pMe) const
    {
        const Header& h = pMe->header();
        // if not set consider a part to be an attach if its
        // MIME type is not text/*
        if(!m_set)
            return h.contentType().type() != "text" && 
                h.contentType().type() != "message";
        // check for content type match
        if(m_type.length() && m_type != h.contentType().type())
            return false;
        if(m_subtype.length() && m_subtype != h.contentType().subtype())
            return false;
        return true;
    }
    void type(const string& s)     { m_type = s; ++m_set; }
    const istring& type() const    { return m_type; }
    void subtype(const string& s)    { m_subtype = s; ++m_set; }
    const istring& subtype() const    { return m_subtype; }
    void quiet(bool b)        { m_quiet = b; }
    bool quiet() const        { return m_quiet; }
private:
    istring m_type, m_subtype;
    bool m_quiet, m_set;
} mpr;

void die(bool b, const string& msg)
{
    if(b)
    {
        cerr << "Error: " << msg << endl << endl;
        usage();
    }
}

string get_filename(const MimeEntity& me)
{
    if(me.hasField(ContentDisposition::label))
    {
        const ContentDisposition& cd = me.header().contentDisposition();
        if(cd.param("filename").length())
            return cd.param("filename");
    } else if (me.hasField(ContentType::label)) {
        const ContentType& ct = me.header().contentType();
        return string("unnamed_" + ct.type() + "." + ct.subtype());
    } 
    return "unknown_attachment";
}

static bool is_known_mechanism(const string& mechanism)
{
    istring m = mechanism;

    if(m == "base64" || m == "quoted-printable" || m == "binary" || 
       m == "7bit" || m == "8bit")
        return true;

    return false;
}

void decode_bin(const MimeEntity& me)
{
    const ContentTransferEncoding& cte = me.header().contentTransferEncoding();
    // const ContentDisposition& cd = me.header().contentDisposition();

    if(is_known_mechanism(cte.mechanism()))
    {
        string filename = get_filename(me);
        if(File::exists(filename))
        {
            int t;
            for(t = 0; File::exists(utils::int2str(t)+"-"+filename);t++)
                ;
            filename = utils::int2str(t) + "-" + filename;    
        }
        const ContentType& ct = me.header().contentType();
        if(!mpr.quiet())
            cout << "\tdecoding " << filename
                << " (" << ct.type() << "/" << ct.subtype() << ")"
                << endl;
        ofstream of(filename.c_str());    
        if(!of.is_open())
        {
            cerr << "ERR: unable to write to " << filename << endl;
            return;
        }
        ostreambuf_iterator<char> oi(of);
        istring enc_algo = cte.mechanism();
        if(enc_algo == ContentTransferEncoding::base64)
        {
            Base64::Decoder b64;
            g_decoded++;
            decode(me.body().begin(), me.body().end(), b64 ,oi);
        } else if (enc_algo == ContentTransferEncoding::quoted_printable) {
            QP::Decoder qp;
            g_decoded++;
            decode(me.body().begin(), me.body().end(), qp, oi);
        } else if (enc_algo == ContentTransferEncoding::eightbit || 
                enc_algo == ContentTransferEncoding::sevenbit || 
                enc_algo == ContentTransferEncoding::binary) {
            copy(me.body().begin(), me.body().end(), oi);
        } else {
            cerr << "ERR: unknown encoding algorithm "
                << enc_algo << endl;
        }
    }
}

void parsePart(const MimeEntity& me, string& fqn)
{
    g_parts++;
    if(mpr(&me))
    {
        if(!mpr.quiet() && fqn.length() > 0)
        {
            cout << fqn << endl;
            fqn.clear();
        }
        decode_bin(me);
    }
    MimeEntityList::const_iterator mbit, meit;
    mbit = me.body().parts().begin(), meit = me.body().parts().end();
    for(; mbit != meit; ++mbit)
        parsePart(**mbit, fqn);
}

inline int isnl(char c)
{
    return c == '\n' || c == '\r';
}

template<typename Iterator>
void parseMboxFile(Iterator bit, Iterator eit, string& fqn)
{
    char prev;
    Iterator it = bit;
    for(; bit != eit; )
    {
        for(;;)
        {
            it = utils::find_bm(it, eit, "From ");
            //it = find_n(it, eit, "From ");
            prev = *(it-1); // previous char (must be a newline)
            if(it == eit || isnl(prev) )
                break;
            else
                ++it; // From in the middle of a line
        } 
        g_messages++;
        MimeEntity me(bit, it);
        parsePart(me, fqn);
        if(it == eit)
            return;
        bit = it;
        ++it; // skip the current From 
    }
}

template<typename Iterator>
void parse(Iterator bit, Iterator eit, string& fqn)
{
    string sep = "From ";
    Iterator it = utils::find_bm(bit, bit + sep.length(), sep);
    if(it == bit)
    {
        parseMboxFile(bit, eit, fqn);
    } else {
        g_messages++;
        MimeEntity me(bit, eit);
        parsePart(me, fqn);
    }
}


int main(int argc, char** argv)
{
    std::ios_base::sync_with_stdio(false);

    // handle command line parameters
    int p = 1;
    while(p < argc)
    {
        string param = argv[p];
        if(param == "-h")
            usage();
        else if (param == "-q") 
            mpr.quiet(true);
        else if (param == "-t") {
            die( ++p == argc, param + " requires an argument");
            ContentType ct(argv[p]);
            die(mpr.type().length() != 0, "just one -t allowed");
            mpr.type(ct.type());
            mpr.subtype(ct.subtype());
        } else {
            // filename list starts here
            // first filename: argv[p]
            break;
        }
        ++p;
    }

    string fqn;
    if(argc == p)
    { // read from stdin
        fqn = "stdin";
        string buf;
        enum { page_sz = 4096 };
        char page[page_sz];
        int count;
        while((count = cin.rdbuf()->sgetn(page, page_sz)) > 0)
            buf.append(page);
        parse(buf.begin(), buf.end(), fqn);
        g_files++;
    } else {
        for(int fc = p; fc < argc; ++fc)
        {
            fqn = argv[fc];
            File in(fqn);
            if(!in)
            {
                cerr << "ERR: unable to open file " << argv[fc]
                     << endl;
                continue;
            }
            g_files++;
            parse(in.begin(), in.end(), fqn);
        }
    }
    if(!mpr.quiet())
        cout << g_files << " file(s) analyzed, " <<
            g_messages << " message(s) and " <<
            g_parts << " entitie(s) parsed, " <<
            g_decoded << " attachment(s) extracted." << endl;
    return g_decoded;
}

