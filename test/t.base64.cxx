#include "t.base64.h"
using namespace mimetic;
/*
 * test fields:
 * input        output in binary mode
 *
 */
const char* test_base64::test[][2] = {
{ "a",         "YQ=="        },
{ "ab",     "YWI="        },
{ "abc",     "YWJj"        },
{ "abcd",     "YWJjZA=="    },
{ "abcde",     "YWJjZGU="    },
{ "abcdef",     "YWJjZGVm"    },
{ "abcdefg",     "YWJjZGVmZw=="    },
{ "abcdefgh",     "YWJjZGVmZ2g="    },
{ "abcdefghi",     "YWJjZGVmZ2hp"    },
{ "abcdefghil",    "YWJjZGVmZ2hpbA=="    },
{ "abcdefghilm",    "YWJjZGVmZ2hpbG0="    },
{ "abcdefghilmn",    "YWJjZGVmZ2hpbG1u"    },
{ "abcdefghilmno",    "YWJjZGVmZ2hpbG1ubw=="    },
{ "abcdefghilmnop",    "YWJjZGVmZ2hpbG1ub3A="    },
{ "abcdefghilmnopq",    "YWJjZGVmZ2hpbG1ub3Bx"    },
{ "abcdefghilmnopqr",    "YWJjZGVmZ2hpbG1ub3Bxcg=="    },
{ "abcdefghilmnopqrs",    "YWJjZGVmZ2hpbG1ub3BxcnM="    },
{ "abcdefghilmnopqrst",    "YWJjZGVmZ2hpbG1ub3BxcnN0"    },
{ "abcdefghilmnopqrstu","YWJjZGVmZ2hpbG1ub3BxcnN0dQ=="    },
{ "abcdefghilmnopqrstuv","YWJjZGVmZ2hpbG1ub3BxcnN0dXY="    },
{ "abcdefghilmnopqrstuvz","YWJjZGVmZ2hpbG1ub3BxcnN0dXZ6"    },
// binary
{ "\1","AQ=="    },
{ "\2","Ag=="    },
{ "\1\2","AQI="    },
{ "\1\2\3","AQID"    },
{ "abcde\1fghi\2lmno\3pqr\4stu\5vz","YWJjZGUBZmdoaQJsbW5vA3BxcgRzdHUFdno="    },
{ 0, 0 }
};

/*
 * only test decoder, invalid chars should be skipped
 *
 * test fields:
 * expected decoded data        base64 encoded data including invalid chars (e.g. whitespace)
 *
 */
const char* test_base64::test_invalid_input[][2] = {
{ "abcdefghilmnopqrstuvz","Y WJ#jZGVm:Z2h&pb;G1ub3-Bxcn\nN0d$XZ6"    },
// | is 124 (dec) which is exactly the size of Base64::sDecTableSz
{ "abcdefghilmnopqrstuvz","YWJjZGVmZ2|hpbG1ub3BxcnN0dXZ6"    },
{ 0, 0 }
};