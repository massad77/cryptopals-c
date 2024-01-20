#include <gtest/gtest.h>
extern "C" {
#include "base64_conversion.h"
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
            float score;
            unsigned char key;
            char plaintext[COUNT_OF(msg) + 1];
    };

    item_c items[256];
    item_c res = {0};
    float max = 0;

    /* try all possible keys and score result */
    for(unsigned long key = 0; key < COUNT_OF(items); ++key)
    {
        char new_msg[COUNT_OF(msg)] = {0};
        for(unsigned long i = 0; i < COUNT_OF(new_msg); ++i)
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

TEST(Score, score)
{
    const char txt[] = "etaoinsrhdlu";
    EXPECT_FLOAT_EQ(score_text(txt, COUNT_OF(txt)), 80.58);
    const char txt1[] = "              ";
    EXPECT_FLOAT_EQ(score_text(txt1, COUNT_OF(txt1)), 56.0);
    const char txt2[] = "hello";
    EXPECT_FLOAT_EQ(score_text(txt2, COUNT_OF(txt2)), 33.58);
}

TEST(FixedXor, Set1_4)
{
    FILE *fp;

    fp = fopen("../4.txt", "r");
    if(fp == 0)
    {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    typedef struct {
        int key;
        float score;
        char res[128];
    } keyscore_t;

    keyscore_t linescore[400];
    char *plaintext = NULL;

    size_t line_size = 0;
    ssize_t nread = 0;
    char *line = NULL;
    int len = 0;
    int out_len = 0;
    int line_count = 0;
    float max = 0;
    int solution = 0;

    do {
        nread = getline(&line, &line_size, fp);
        len = nread - 1; // do not care about '\n' and '\0'
        plaintext = decode_base16(line, len, &out_len);

        float local_max = 0;
        int local_key = 0;
        float score = 0;
        for(unsigned char c = 0; c < 255; ++c)
        {
            singlebyte_xor(plaintext, linescore[line_count].res, out_len, c);
            score = score_text(linescore[line_count].res, out_len);
            if(score > local_max)
            {
                local_max = score;
                local_key = c;
            }
        }
        linescore[line_count].key = local_key;
        linescore[line_count].score = local_max;
        singlebyte_xor(plaintext, linescore[line_count].res, out_len, local_key);

        if(local_max > max)
        {
            max = local_max;
            solution = line_count;
        }

        ++line_count;
    } while (nread > 0);

    /* cut string */
    for(unsigned long i = 0; i < COUNT_OF(linescore[solution].res); ++i)
    {
        if(linescore[solution].res[i] == '\n')
        {
            linescore[solution].res[i] = '\0';
            break;
        }
    }
    EXPECT_STREQ(linescore[solution].res, "Now that the party is jumping");

    free(line);
    fclose(fp);
}

TEST(FixedXor, Set1_5)
{
    char in[] = "Burning 'em, if you ain't quick and nimble\nI go crazy when I hear a cymbal";
    char want[] = "\x0b\x36\x37\x27\x2a\x2b\x2e\x63\x62\x2c\x2e\x69\x69\x2a\x23\x69\x3a\x2a\x3c\x63\x24\x20\x2d\x62\x3d\x63\x34\x3c\x2a\x26\x22\x63\x24\x27\x27\x65\x27\x2a\x28\x2b\x2f\x20\x43\x0a\x65\x2e\x2c\x65\x2a\x31\x24\x33\x3a\x65\x3e\x2b\x20\x27\x63\x0c\x69\x2b\x20\x28\x31\x65\x28\x63\x26\x30\x2e\x27\x28\x2f";
    char *res = NULL;
    char key[] = "ICE";

    res = XORencode(in, COUNT_OF(in)-1, key, get_length(key)); // drop trailing '\0' from in

    EXPECT_STREQ(res, want);
}
