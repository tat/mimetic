#include "t.composite.h"
using namespace mimetic;
/*
 * test fields:
 * input        output in binary mode
 */
const char* test_composite::test[][2] = {
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
{ 0, 0 }
};
