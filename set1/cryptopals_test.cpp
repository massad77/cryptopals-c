#include <gtest/gtest.h>
extern "C" {
#include "cryptopals.h"
#include "utils.h"
}

TEST(HammingDistance, test)
{
    char a[] = "this is a test";
    char b[] = "wokka wokka!!!";
    int is = 0;
    int want = 37;

    is = hamming_distance(a, b, sizeof(a));

    EXPECT_EQ(is, want);
}

TEST(BreakXOR, keysize)
{
    char in[] = "\x0e\x36\x47\xe8\x59\x2d\x35\x51\x4a\x08\x12\x43\x58\x25\x36\xed\x3d\xe6\x73\x40\x59\x00\x1e\x3f\x53\x5c\xe6\x27\x10\x32";
    float want = 3.0;

    float is = estimate_keysize(in, COUNT_OF(in));
    EXPECT_EQ(is, want);
}

TEST(BreakXOR, Set1_6)
{
    FILE *fp;

    fp = fopen("../6.txt", "r");
    if(fp == 0)
    {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    free(fp);
}
