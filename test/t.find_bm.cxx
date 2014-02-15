#include "t.find_bm.h"
using namespace mimetic;
/*
 * test fields:
 * input:    text, pattern, offset returned or strlen(text) if not found
 * (i.e. return end iterator-->the first char after the end)
 *
 */
const struct test_find_bm::test_s test_find_bm::test[] = {
{ "a",         "a",    0    },
{ "a",         "b",    1    },
{ "a",         "ab",    1    },
{ "ab",     "a",    0    },
{ "ab",     "b",    1    },
{ "ab",     "c",    2    },
{ "abc",     "abc",    0    },
{ "abc",     "ab",    0    },
{ "abc",     "bc",    1    },
{ "abc",     "d",    3    },
{ "abc",     "ddd",    3    },
{ "abc",     "dddd",    3    },
{ "abcd",     "d",    3    },
{ "abcd",     "cd",    2    },
{ 0, 0, 0 }
};
