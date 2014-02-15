#ifndef _T_FILE_H_
#define _T_FILE_H_
#include <sstream>
#include <iterator>
#include <algorithm>
#include <fstream>
#include <string>
#include <cctype>
#include "cutee.h"
#include <mimetic/os/os.h>


namespace mimetic 
{

class TEST_CLASS( test_file )
{
    std::string m_text_fqn, m_bin_fqn;
public:
    test_file()
    : m_text_fqn("t.file.h"), m_bin_fqn("file.bin")
    {
    }
    void TEST_FUNCTION( testFileStdFile )
    {
        File a(m_text_fqn);
        File::iterator beg = a.begin(), end = a.end();
        StdFile in(m_text_fqn);
        StdFile::iterator _fbeg = in.begin(), _fend = in.end();
        StdFile::iterator fbeg = _fbeg, fend = _fend;
        for(; beg != end; ++beg, ++fbeg)
        {
            if(*beg != *fbeg)
            { // I don't want all bytes to be counted as test..
                TEST_ASSERT_EQUALS_P(*beg, *fbeg);
            }
        }
        TEST_ASSERT(beg == end && fbeg == fend);
    }
    void TEST_FUNCTION( testBinFileStdFile )
    {
        File a(m_bin_fqn);
        File::iterator beg = a.begin(), end = a.end();
        StdFile in(m_bin_fqn);
        StdFile::iterator fbeg = in.begin(), fend = in.end();
        for(; beg != end; ++beg, ++fbeg)
        {
            if(*beg != *fbeg)
            { // I don't want all bytes to be counted as test..
                TEST_ASSERT_EQUALS_P(*beg, *fbeg);
            }
        }
        TEST_ASSERT(beg == end && fbeg == fend);
    }
    void TEST_FUNCTION( testStdFile )
    {
        std::ifstream ifs(m_text_fqn.c_str());
        std::istreambuf_iterator<char> beg(ifs), end;
        StdFile in(m_text_fqn);
        StdFile::iterator fbeg = in.begin(), fend = in.end();
        for(; beg != end; ++beg, ++fbeg)
        {
            if(*beg != *fbeg)
            { // I don't want all bytes to be counted as test..
                TEST_ASSERT_EQUALS_P(*beg, *fbeg);
            }
        }
        TEST_ASSERT(beg == end && fbeg == fend);
    }
    void TEST_FUNCTION( testFileIterator )
    {
        std::ifstream ifs(m_text_fqn.c_str());
        std::istreambuf_iterator<char> beg(ifs), end;
        File in(m_text_fqn);
        File::iterator fbeg = in.begin(), fend = in.end();
        for(; beg != end; ++beg, ++fbeg)
        {
            if(*beg != *fbeg)
            { // I don't want all bytes to be counted as test..
                TEST_ASSERT_EQUALS_P(*beg, *fbeg);
            }
        }
        TEST_ASSERT(beg == end && fbeg == fend);
    }
    void TEST_FUNCTION( testFileConstIterator )
    {
        std::ifstream ifs(m_text_fqn.c_str());
        std::istreambuf_iterator<char> beg(ifs), end;
        const File in(m_text_fqn);
        File::const_iterator fbeg = in.begin(), fend = in.end();
        for(; beg != end; ++beg, ++fbeg)
        {
            if(*beg != *fbeg)
            { // I don't want all bytes to be counted as test..
                TEST_ASSERT_EQUALS_P(*beg, *fbeg);
            }
        }
        TEST_ASSERT(beg == end && fbeg == fend);
    }
    void TEST_FUNCTION( testBinFileIterator )
    {
        std::ifstream ifs(m_bin_fqn.c_str());
        std::istreambuf_iterator<char> beg(ifs), end;
        File in(m_bin_fqn);
        File::iterator fbeg = in.begin(), fend = in.end();
        for(; beg != end; ++beg, ++fbeg)
        {
            if(*beg != *fbeg)
            { // I don't want all bytes to be counted as test..
                TEST_ASSERT_EQUALS_P(*beg, *fbeg);
            }
        }
        TEST_ASSERT(beg == end && fbeg == fend);
    }
    void TEST_FUNCTION( testBinFileConstIterator )
    {
        std::ifstream ifs(m_bin_fqn.c_str());
        std::istreambuf_iterator<char> beg(ifs), end;
        const File in(m_bin_fqn);
        File::const_iterator fbeg = in.begin(), fend = in.end();
        for(; beg != end; ++beg, ++fbeg)
        {
            if(*beg != *fbeg)
            { // I don't want all bytes to be counted as test..
                TEST_ASSERT_EQUALS_P(*beg, *fbeg);
            }
        }
        TEST_ASSERT(beg == end && fbeg == fend);
    }
};

}

#endif
