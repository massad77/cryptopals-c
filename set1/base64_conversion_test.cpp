#include <gtest/gtest.h>
extern "C" {
#include "base64_conversion.h"
#include "utils.h"
}

#define NUM_TESTS 12

TEST(DecodeBase16, GoodCase)
{
    char *out = NULL;
    int len = 0;
    /* single digit conversion */
    out = decode_base16("A", 1, &len);
    EXPECT_EQ(out[0], '\x0a');
    free(out);
    out = decode_base16("f", 1, &len);
    EXPECT_EQ(out[0], '\x0f');
    free(out);

    /* two digit conversion */
    out = decode_base16("AA", 2, &len);
    EXPECT_EQ(out[0], '\xaa');
    free(out);
    out = decode_base16("F0", 2, &len);
    EXPECT_EQ(out[0], '\xf0');
    free(out);
    out = decode_base16("AF", 2, &len);
    EXPECT_EQ(out[0], '\xaf');
    free(out);

    /* five digit conversion */
    out = decode_base16("AA55E", 5, &len);
    EXPECT_EQ(out[0], '\xaa');
    EXPECT_EQ(out[1], '\x55');
    EXPECT_EQ(out[2], '\xe0');
    free(out);

    out = decode_base16("fa0ce", 5, &len);
    EXPECT_EQ(out[0], '\xfa');
    EXPECT_EQ(out[1], '\x0c');
    EXPECT_EQ(out[2], '\xe0');
    free(out);
}

TEST(DecodeBase16, EdgeCases)
{
    char *out = NULL;
    int len = 0;
    out = decode_base16(NULL, 0, &len);
    EXPECT_EQ(out, nullptr);
    free(out);
    out = decode_base16(NULL, 3, &len);
    EXPECT_EQ(out, nullptr);
    free(out);
    out = decode_base16("AA", 0, &len);
    EXPECT_EQ(out, nullptr);
    free(out);
}

TEST(DecodeBase64, Simple)
{
    char in[] = "TWFuTWFu";
    char want[] = "ManMan";
    char *out = NULL;
    int len = 0;

    out = decode_base64(in, COUNT_OF(in), &len);
    EXPECT_STREQ(out, want);

    free(out);
}

TEST(DecodeBase64, Padding)
{
    char in[] = "bGlnaHQgdw==";
    char want[] = "light w";
    char *out = NULL;
    int len = 0;

    out = decode_base64(in, COUNT_OF(in), &len);
    EXPECT_STREQ(out, want);
    free(out);

    char in1[] = "bGlnaHQgd28=";
    char want1[] = "light wo";
    out = NULL;

    out = decode_base64(in1, COUNT_OF(in1), &len);
    EXPECT_STREQ(out, want1);
    free(out);
}

TEST(DecodeBase64, Set1)
{
    char in[] = "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t";
    char want[] = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
    char *plaintext = NULL;
    char *out = NULL;
    int len = 0;
    int out_len = 0;

    plaintext = decode_base16(want, sizeof(want) - 1, &out_len);
    out = decode_base64(in, COUNT_OF(in), &len);
    EXPECT_STREQ(out, plaintext);

    free(out);
    free(plaintext);
}
