#include "t.qp.h"
using namespace mimetic;


/*
 * test field:
 * input        output in text mode        output in binary mode
 *
 * note: maxlen == 12 during the following tests
 */
const char* test_qp::test[][3] = {
{ "test",         "test",                0 },
{ "te st",         "te st",            "te=20st" },
{ " test",         " test",            "=20test" },
{ "\ttest",         "\ttest",            "=09test" },
{ "tes\tt",         "tes\tt",            "tes=09t" },
{ "equal=",         "equal=3D",            0 },
{ "Fantastic",         "Fantastic",            0 },
{ "Fromage",         "Fromage",            0 },
{ "From ",         "=46rom=20",            0 },
{ "From more",         "=46rom more",            "=46rom=20mo=\nre" },
{ "From age",         "=46rom age",            "=46rom=20age" },
{ "te\xfest",         "te=FEst",            0 },
{ "te\1st",         "te=01st",            0 },
{ ".test",         ".test",            0 },
{ ".",             "=2E",                0 },
// unsafe chars
{ "unsafe!#$",         "unsafe=21=\n=23=24",        0 },
// maxline test
{ "aaaabbbbcccc",     "aaaabbbbcccc",            0 },
{ "aaaabbbbccccd",     "aaaabbbbccc=\ncd",        0 },
{ "aaaabbbbccccdddd",     "aaaabbbbccc=\ncdddd",    0 },
{ "aaaabbbbcc##",     "aaaabbbbcc=\n=23=23",    0 },
{ "aaaabbbbccc##",     "aaaabbbbccc=\n=23=23",    0 },
{ "aaaabbbbcccc##",     "aaaabbbbccc=\nc=23=23",    0 },
// trailing blank (sp && tab)
{ "abcde    f",     "abcde    f",        "abcde=20=20=\n=20=20f" },
{ "abcde    ",      "abcde   =20",        "abcde=20=20=\n=20=20" },
{ "Abcde     ",     "Abcde    =20",        "Abcde=20=20=\n=20=20=20" },
{ "ABcde      ",     "ABcde     =\n=20",    "ABcde=20=20=\n=20=20=20=20"},
{ "abcde\t \t a",     "abcde\t \t a",        "abcde=09=20=\n=09=20a" },
// newline testing
{ "abcde\nf",     "abcde\nf",        "abcde=0Af" },
{ "abcdef\n\ng",     "abcdef\n\ng",    "abcdef=0A=\n=0Ag" },
{ "\nabc\n\ndefgh","\nabc\n\ndefgh","=0Aabc=0A=\n=0Adefgh" },
//
{ 0, 0, 0 }
};


/*
 * expected decoded string    malformed encoded string
 */
const char* test_qp::test_decode_malformed[][2] = {
// trailing spaces
{"abcdefghilmnopqrst",    "abcdefghilm= \r\nnopqrst"},
{"abcdefghilmnopqrst",    "abcdefghilm=  \r\nnopqrst"},
{"abcdefghilmnopqrst",    "abcdefghilm=   \r\nnopqrst"},
{"abcdefghilmnopqrst",    "abcdefghilm=    \r\nnopqrst"},
{"abcdefghilm=     X\nnopqrst","abcdefghilm=     X\r\nnopqrst"},
// rfc2045 6.7 note 1
{"ab=cde","ab=3dcde"},
{"ab\xfe cde","ab=fE cde"},
// rfc2045 6.7 note 2
{"abcdefgh=ilmnopqrst","abcdefgh=ilmno=\r\npqrst"},
{"abcd=Xefghilmnopqrst","abcd=Xefghi=\r\nlmnopqrst"},
{"abcd= efghilmnopqrst","abcd= efghi=\r\nlmnopqrst"},
{"abcdefghilm=nopqrst",    "abcdefghilm=\r\n=nopqrst"},
// rfc2045 6.7 note 3
{"abcd=","abcd="},
{"abcd=A","abcd=A"},
{"abcd=X","abcd=X"},
// rfc2045 6.7 note 4
{"abcde","ab\1cde"},
{"abcde","ab\2c\3de"},
{"abcdefghi","\4abcd\5efghi"},
// rfc2045 6.7 note 4 (max line == 12)
{"abcdefghilmnopqrstuvz","abcdefghilmnopqrstuvz"},
{0,0}
};




