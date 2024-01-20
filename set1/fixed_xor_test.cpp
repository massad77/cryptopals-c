#include <gtest/gtest.h>
extern "C" {
#include "fixed_xor.h"
#include "utils.h"
}

TEST(FixedXor, Set1_2)
{
    /* Challegen 1.2 */
    const char in[] = "\x1c\x01\x11\x00\x1f\x01\x01\x00\x06\x1a\x02\x4b\x53\x53\x50\x09\x18\x1c";
    const char key[] = "\x68\x69\x74\x20\x74\x68\x65\x20\x62\x75\x6c\x6c\x27\x73\x20\x65\x79\x65";
    const char want[] = "the kid don't play";
    char res[sizeof(in)];

    init_byte_array(res, sizeof(res));

    fixed_xor(in, key, res, sizeof(res));
    EXPECT_STREQ(res, want);
}
