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

TEST(FixedXor, Set1_3)
{
	const char msg[] = "\x1b\x37\x37\x33\x31\x36\x3f\x78\x15\x1b\x7f\x2b\x78\x34\x31\x33\x3d\x78\x39\x78\x28\x37\x2d\x36\x3c\x78\x37\x3e\x78\x3a\x39\x3b\x37\x36";

    class item_c{
        public:
            int score;
            unsigned char key;
            char plaintext[COUNT_OF(msg) + 1];
    };

    item_c items[256];
    item_c res = {0};

    /* try all possible keys and score result */
    for(int key = 0, max = 0; key < COUNT_OF(items); ++key)
    {
        char new_msg[COUNT_OF(msg)] = {0};
        for(int i = 0; i < COUNT_OF(new_msg); ++i)
        {
            items[key].plaintext[i] = msg[i] ^ key;
        }
        items[key].score = score_text(items[key].plaintext, \
                                       COUNT_OF(items[key].plaintext));
        items[key].key = key;

        if(items[key].score > max)
        {
            max = items[key].score;
            res = items[key];
        }
    }

    EXPECT_STREQ(res.plaintext, "Cooking MC's like a pound of baconX");
}

