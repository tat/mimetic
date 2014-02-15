#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <streambuf>
#include <mimetic/mimetic.h>
#include <mimetic/utils.h>
#include <openssl/bio.h>
#include <openssl/pkcs7.h>
#include <openssl/x509.h>
#include <openssl/err.h>
#include <openssl/ssl.h>

using namespace std;
using namespace mimetic;

void printMimeStructure(MimeEntity* pMe, int tabcount = 0);

int g_verbose; // verbose mode on/off
int g_quiet; // quiet mode
int g_entityCount; // num of entities found

void printTabs(int c)
{
    while(c--)
        cout << "    ";
}

int bca_pkcs7_verify(PKCS7 *p7)
{
    return ~0;
}

int pkcs7MimeHandle(MimeEntity* pMe, int tabcount)
{
    BIO *in = NULL, *content = NULL;
    PKCS7 *p7 = NULL;
    long sz;
    char *pdata = NULL;
    int rc = ~0, vfy, iMask = imPreamble | imEpilogue;
    string me_str;
    Header& h = pMe->header();
    ContentType ct = h.contentType();

    {
    stringstream ss;
    ss << *pMe;
    me_str = ss.str();
    }

    in = BIO_new_mem_buf((void*)me_str.c_str(), me_str.length());
    if(in == NULL)
        goto err;
    BIO_set_mem_eof_return(in, 0);

    p7 = SMIME_read_PKCS7(in, &content);
    if(p7 == NULL)
        goto err;

    vfy = bca_pkcs7_verify(p7);

    printTabs(tabcount);
    cout << ct.type() << "/" << ct.subtype() 
        << " (verify " << (vfy == 0 ? "ok" : "failed") << ")"
        << endl;

    if(content == NULL)
    { 
        content = PKCS7_dataInit(p7, NULL);
        if(content == NULL)
            goto err;
    } else {
        ; /* multipart/signed, content is the cleartext message part */
        goto err;
    }
    
    sz = BIO_get_mem_data(content, &pdata);
    if(sz > 0 && pdata)
    {
        MimeEntity me(pdata, pdata + sz, iMask);
        printMimeStructure(&me, 1 + tabcount);
    }

    /* success */
    rc = 0;
err:
    if(in)
        BIO_free(in);
    if(content)
        BIO_free(content);
    if(p7)
        PKCS7_free(p7);
    if(rc)
        ERR_print_errors_fp(stderr);
    return rc;
}


void printMimeStructure(MimeEntity* pMe, int tabcount)
{
    Header& h = pMe->header();
    ContentType ct = h.contentType();
    ContentId ci = h.contentId();

    ++g_entityCount;

    if(ct.isMultipart() || 
        (ct.type() == "message" && ct.subtype() == "rfc822"))
    {
        if(ct.subtype() == "signed")
        {
            cerr << "S/MIME multipart/signed not yet supported!" << endl;
            exit(1);
            #if 0
            if(pMe->body().parts().size() != 2)
                cerr << "bad message!" << endl;
            if(smimeVerify(pMe))
                cerr << "S/MIME verification failed" << endl;
            #endif
        } else {
            printTabs(tabcount);
            cout << ct.type() << "/" << ct.subtype() << endl;

            MimeEntityList::iterator mbit = pMe->body().parts().begin(),
                            meit = pMe->body().parts().end();
            for(; mbit != meit; ++mbit)
                printMimeStructure(*mbit, 1 + tabcount);
        }
    } else {
        if(ct.type() == "application" && 
            (ct.subtype() == "x-pkcs7-mime" || ct.subtype() == "pkcs7-mime"))
        {
            pkcs7MimeHandle(pMe, tabcount);
        } else {
            printTabs(tabcount);
            cout << ct.type() << "/" << ct.subtype() << endl;
        }
    }
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
    //int iMask = imBody | imPreamble | imEpilogue;
    int iMask = imPreamble | imEpilogue;

    /* Initialize the OpenSSL engine.  */
    CRYPTO_mem_ctrl(CRYPTO_MEM_CHECK_ON);
    OpenSSL_add_all_algorithms();
    ERR_load_crypto_strings();

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

