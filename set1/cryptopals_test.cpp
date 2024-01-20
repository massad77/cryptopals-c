#include <gtest/gtest.h>
#include <stdio.h>
extern "C" {
#include "base64_conversion.h"
#include "cryptopals.h"
#include "fixed_xor.h"
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

TEST(BreakXOR, Set1_6)
{
    FILE *fp;
    int fsize = 0;

    fp = fopen("../6.txt", "r");
    if(fp == 0)
    {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    /* get size */
    fseek(fp, 0, SEEK_END);
    fsize = ftell(fp);
    if(fsize < 0)
    {
        perror("ftell");
        exit(EXIT_FAILURE);
    }

    rewind(fp);

    char *buf = (char *)calloc(fsize+1, sizeof(char));
    char *decoded = NULL;

    int c = 0, i = 0;
    do {
        c = fgetc(fp);
        if(c != '\n')
            buf[i++] = c;
    } while(c != EOF);
    buf[i-1] = '\0'; // clear the EOF in the buffer

    int out_len;
    decoded = decode_base64(buf, fsize, &out_len);

    int keylen = estimate_keysize(decoded, out_len);
    //printf("Key length: %d, data length: %d\n", keylen, out_len);
    EXPECT_EQ(keylen, 29);

    char **blocks = NULL;
    blocks = (char **)malloc((out_len/keylen) * sizeof(char *));
    char *ptr = decoded;
    for(int i = 0; i < (out_len/keylen); ++i)
    {
        blocks[i] = (char *)malloc((keylen + 1) * sizeof(char));
        memcpy(blocks[i], ptr, keylen);
        blocks[i][keylen] = '\0';
        ptr += keylen;
    }

    EXPECT_EQ(blocks[0][keylen-1], decoded[keylen-1]);
    EXPECT_EQ(blocks[1][keylen-1], decoded[keylen+(keylen-1)]);
    EXPECT_EQ(blocks[3][keylen-1], decoded[(3*keylen)+(keylen-1)]);

    /* transpose */
    char **transposed = NULL;
    transposed = (char **)malloc(keylen * sizeof(char *));
    for(int i = 0; i < keylen; ++i)
    {
        transposed[i] = (char *)malloc((out_len/keylen) * sizeof(char));
        for (int j = 0; j < (out_len/keylen); ++j) {
            transposed[i][j] = blocks[j][i];
        }
    }

    EXPECT_EQ(transposed[0][1], blocks[1][0]);
    EXPECT_EQ(transposed[0][out_len/keylen - 1], blocks[out_len/keylen - 1][0]);
    EXPECT_EQ(transposed[keylen-1][0], decoded[keylen-1]);
    EXPECT_EQ(transposed[keylen-1][1], decoded[keylen+(keylen-1)]);
    EXPECT_EQ(transposed[keylen-1][3], decoded[(3*keylen)+(keylen-1)]);

    /* break singlebyte xor */
    typedef struct{
        int key;
        float score;
        char *res;
    } keyscore_t;

    keyscore_t keyscore;
    keyscore.res = (char *)malloc((out_len/keylen + 1) * sizeof(char));
    keyscore.res[out_len/keylen] = '\0';

    char *key = (char *)malloc((keylen + 1) * sizeof(char));
    key[keylen] = '\0';

    for(int i = 0; i < keylen; ++i)
    {
        float local_max = 0;
        int local_key = 0;
        float score = 0;
        for(unsigned char c = 255; c > 0; --c)
        {
            singlebyte_xor(transposed[i], keyscore.res, out_len/keylen, c);
            score = score_text(keyscore.res, out_len/keylen);
            if(score > local_max)
            {
                local_max = score;
                local_key = c;
            }
        }
        key[i] = local_key;
        keyscore.key = local_key;
        keyscore.score = local_max;
        //printf("Block %d, Key 0x%x, score %f\n", i, local_key, local_max);
    }

    char *result = XORencode(decoded, out_len, key, keylen);
    EXPECT_STREQ(key, "Terminator X: Bring the noise");
    //printf("Key: %s\n", key);
    printf("%s\n", result);

    free(keyscore.res);
    free(key);
    /* clean up */
    free(decoded);
    free(buf);
    for (int i = 0; i < out_len/keylen; i++)
    {
        free(blocks[i]);
    }
    free(blocks);
    free(result);

    for (int i = 0; i < keylen; i++)
    {
        free(transposed[i]);
    }
    free(transposed);

    fclose(fp);
}
