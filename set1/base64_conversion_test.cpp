#include <gtest/gtest.h>
extern "C" {
#include "base64_conversion.h"
#include "utils.h"
}

#define NUM_TESTS 12

TEST(DecodeBase16, GoodCase)
{
    char out[8];
    init_byte_array(out, sizeof(out));
    /* single digit conversion */
    decode_base16("A", 1, out, sizeof(out));
    EXPECT_EQ(out[0], '\x0a');
    decode_base16("f", 1, out, sizeof(out));
    EXPECT_EQ(out[0], '\x0f');

    /* two digit conversion */
    init_byte_array(out, sizeof(out));
    decode_base16("AA", 2, out, sizeof(out));
    EXPECT_EQ(out[0], '\xaa');
    init_byte_array(out, sizeof(out));
    decode_base16("F0", 2, out, sizeof(out));
    EXPECT_EQ(out[0], '\xf0');
    init_byte_array(out, sizeof(out));
    decode_base16("AF", 2, out, sizeof(out));
    EXPECT_EQ(out[0], '\xaf');

    /* five digit conversion */
    init_byte_array(out, sizeof(out));
    decode_base16("AA55E", 5, out, sizeof(out));
    EXPECT_EQ(out[0], '\xaa');
    EXPECT_EQ(out[1], '\x55');
    EXPECT_EQ(out[2], '\xe0');

    init_byte_array(out, sizeof(out));
    decode_base16("fa0ce", 5, out, sizeof(out));
    EXPECT_EQ(out[0], '\xfa');
    EXPECT_EQ(out[1], '\x0c');
    EXPECT_EQ(out[2], '\xe0');
}

TEST(DecodeBase16, EdgeCases)
{
    char out[8];
    int ret = 0;
    ret = decode_base16("AA", 2, out, 0);
    EXPECT_EQ(ret, -1);
    ret = decode_base16(NULL, 0, out, 2);
    EXPECT_EQ(ret, -1);
    ret = decode_base16("A", 1, NULL, 2);
    EXPECT_EQ(ret, -1);
}
/*
TEST(Base64Test, HexToBase64)
{
    char out_num[128];

    char *test_strings[NUM_TESTS][NUM_TESTS] = {
        { (char*)"0", (char*)"A" },
        { (char*)"1", (char*)"B" },
        { (char*)"a", (char*)"K" },
        { (char*)"f", (char*)"P" },
        { (char*)"3f", (char*)"/" },
        { (char*)"40", (char*)"BA" },
        { (char*)"81", (char*)"CB" },
        { (char*)"a5a", (char*)"pa" },
        { (char*)"5a5a", (char*)"Fpa" },
        { (char*)"10000", (char*)"QAA" },
        { (char*)"200000", (char*)"IAAA" },
        { (char*)"1000000", (char*)"BAAAA" }
    };

    for(int i = 0; i < NUM_TESTS; ++i)
    {
        init_byte_array(out_num, sizeof(out_num));
        int len = ascii_hex_to_base64(test_strings[i][0],
                    get_length(test_strings[i][0]), out_num,
                    sizeof(out_num));
        EXPECT_EQ(out_num, test_strings[i][1]);
    }
}
*/
/*
TEST(Base64Test, Base64ToHex)
{
    char out_num[128];

    char *test_strings[NUM_TESTS][NUM_TESTS] = {
        { "0", "A" },
        { "1", "B" },
        { "a", "K" },
        { "f", "P" },
        { "3f", "/" },
        { "40", "BA" },
        { "81", "CB" },
        { "a5a", "pa" },
        { "5a5a", "Fpa" },
        { "10000", "QAA" },
        { "200000", "IAAA" },
        { "1000000", "BAAAA" }
    };

    for(int i = 0; i < NUM_TESTS; ++i)
    {
        init_byte_array(out_num, sizeof(out_num));
        int len = ascii_base64_to_hex(test_strings[i][1],
                    get_length(test_strings[i][1]), out_num,
                    sizeof(out_num));
        EXPECT_EQ(out_num, test_strings[i][0]);
    }
}
*/