#include <gtest/gtest.h>
extern "C" {
#include "cryptopals.h"
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
