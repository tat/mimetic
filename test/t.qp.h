#ifndef _T_QP_H_
#define _T_QP_H_
#include <sstream>
#include <iterator>
#include <string>
#include "cutee.h"
#include <mimetic/codec/codec.h>

namespace mimetic 
{

class TEST_CLASS( test_qp )
{
    static const char* test[][3];
    static const char* test_decode_malformed[][2];
public:
    // calls qp.encode(char,out)
    void TEST_FUNCTION( testEncode )
    {
        int i = 0;
        while(test[i][0] != 0)
        {
            std::string src = test[i][0];
            std::string exp = test[i][1];
            std::string got;
            QP::Encoder qp;
            qp.maxlen(12);
            encode(src.begin(), src.end(), qp,std::back_inserter<std::string>(got));
            TEST_ASSERT_EQUALS_P( exp, got );
            i++;
        }
    }
    // calls qp.encode(InIt, InIt, ,out)
    void TEST_FUNCTION( testEncodeBlock )
    {
        int i = 0;
        while(test[i][0] != 0)
        {
            std::string src = test[i][0];
            std::string exp = test[i][1];
            std::string got;
            QP::Encoder qp;
            qp.maxlen(12);
            qp.process(src.begin(), src.end(), std::back_inserter<std::string>(got));
            TEST_ASSERT_EQUALS_P( exp, got);
            i++;
        }
    }
    void TEST_FUNCTION( testBinaryEncode )
    {
        int i = 0;
        while(test[i][0] != 0)
        {
            std::string src = test[i][0];
            std::string exp = (0 == test[i][2] ?test[i][1] : test[i][2]);
            std::string got;
            QP::Encoder qp(true);
            qp.maxlen(12);
            encode(src.begin(), src.end(),qp,std::back_inserter<std::string>(got));
            TEST_ASSERT_EQUALS_P( exp, got);
            i++;
        }
    }
    void TEST_FUNCTION( testBinaryEncodeBlock )
    {
        int i = 0;
        while(test[i][0] != 0)
        {
            std::string src = test[i][0];
            std::string exp = (0 == test[i][2] ?test[i][1] : test[i][2]);
            std::string got;
            QP::Encoder qp(true);
            qp.maxlen(12);
            qp.process(src.begin(), src.end(),std::back_inserter<std::string>(got));
            TEST_ASSERT_EQUALS_P( exp, got);
            i++;
        }
    }
    void TEST_FUNCTION( testDecode )
    {
        int i = 0;
        while(test[i][0] != 0)
        {
            std::string src = test[i][1];
            std::string exp = test[i][0];
            std::string got;
            QP::Decoder qp;
            decode(src.begin(), src.end(),qp,std::back_inserter<std::string>(got));
            TEST_ASSERT_EQUALS_P( exp, got);
            i++;
        }
    }
    void TEST_FUNCTION( testDecodeBlock )
    {
        int i = 0;
        while(test[i][0] != 0)
        {
            std::string src = test[i][1];
            std::string exp = test[i][0];
            std::string got;
            QP::Decoder qp;
            qp.process(src.begin(), src.end(),std::back_inserter<std::string>(got));
            TEST_ASSERT_EQUALS_P( exp, got);
            i++;
        }
    }
    void TEST_FUNCTION( testBinaryInputDecode )
    {
        int i = 0;
        while(test[i][2] != 0)
        {
            std::string src = test[i][2];
            std::string exp = test[i][0];
            std::string got;
            QP::Decoder qp;
            decode(src.begin(), src.end(),qp,std::back_inserter<std::string>(got));
            TEST_ASSERT_EQUALS_P( exp, got);
            i++;
        }
    }
    void TEST_FUNCTION( testBinaryInputDecodeBlock )
    {
        int i = 0;
        while(test[i][2] != 0)
        {
            std::string src = test[i][2];
            std::string exp = test[i][0];
            std::string got;
            QP::Decoder qp;
            qp.process(src.begin(), src.end(),std::back_inserter<std::string>(got));
            TEST_ASSERT_EQUALS_P( exp, got);
            i++;
        }
    }
    void TEST_FUNCTION( testMalformedInputDecode )
    {
        int i = 0;
        while(test_decode_malformed[i][0] != 0)
        {
            std::string src = test_decode_malformed[i][1];
            std::string exp = test_decode_malformed[i][0];
            std::string got;
            QP::Decoder qp;
            decode(src.begin(), src.end(),qp,std::back_inserter<std::string>(got));
            TEST_ASSERT_EQUALS_P( exp, got);
            i++;
        }
    }
    void TEST_FUNCTION( testMalformedInputDecodeBlock )
    {
        int i = 0;
        while(test_decode_malformed[i][0] != 0)
        {
            std::string src = test_decode_malformed[i][1];
            std::string exp = test_decode_malformed[i][0];
            std::string got;
            QP::Decoder qp;
            qp.process(src.begin(), src.end(),std::back_inserter<std::string>(got));
            TEST_ASSERT_EQUALS_P( exp, got);
            i++;
        }
    }
    // check correctness of static array QP::tb[]
    void TEST_FUNCTION( testTbValues )
    {
        short tb[256];
        for(int c = 0; c < 256; c++)
        {
            if (c != '=' && c > 32 && c < 127)
                tb[c] = QP::printable;
            else 
                tb[c] = QP::binary;
        }
        tb['\t'] = QP::tab; 
        tb[' '] = QP::sp;
        tb['='] = QP::unsafe;
        tb[QP::CR] = tb[QP::LF] = QP::newline;
        const char* unsafe = "!\"#$@[]\\^`{}|~";
        while(*unsafe != 0)
            tb[*unsafe++] = QP::unsafe;
        for(int i = 0; i < 256; i++)
        {
            TEST_ASSERT(tb[i] == QP::sTb[i]);
            //TEST_ASSERT("Correct struct: \n"+tbInitCode(tb), tb[i] == QP::sTb[i]);
        }
    }
private:
    std::string tbInitCode(short* tb)
    {
        std::ostringstream out;
        std::string comment, values;
        bool pcom = false;
        for(int i =0 ; i < 256; i++)
        {
            if(tb[i] == QP::printable)
            {
                comment += i;
                comment += ", ";
                pcom=true;
            } else {
                comment += "   ";
            }
            std::ostringstream oss;
            oss << tb[i] << ", ";
            values += oss.str();
            if((i+1) % 10 == 0)
            {
                if(pcom)
                    out << std::endl<< "// " << comment << std::endl;
                pcom = false;
                out << "   " << values << std::endl;
                comment = "";
                values = "";
            }
        }
        if(pcom)
             out << std::endl<< "// " << comment << std::endl;
        out << "   " << values << std::endl;
        out << std::endl;
        return out.str();
    }

};

}

#endif
