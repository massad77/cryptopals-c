#include <gtest/gtest.h>
#include <stdio.h>
#include <string.h>
extern "C" {
#include <openssl/crypto.h>
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
    char *res = (char *)malloc((out_len/keylen + 1) * sizeof(char));
    res[out_len/keylen] = '\0';

    char *key = (char *)malloc((keylen + 1) * sizeof(char));
    key[keylen] = '\0';

    for(int i = 0; i < keylen; ++i)
    {
        key[i] = break_singlebyte_xor(transposed[i], res, out_len/keylen);
    }

    char *result = XORencode(decoded, out_len, key, keylen);
    EXPECT_STREQ(key, "Terminator X: Bring the noise");
    //printf("Key: %s\n", key);
    //printf("%s\n", result);

    free(res);
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

TEST(OpenSSL, EnDecrypt)
{
    //printf("OpenSSL version: %s\n", OpenSSL_version(OPENSSL_VERSION_STRING));
    unsigned char key[] = { 0x59, 0x45, 0x4c, 0x4c, 0x4f, 0x57, 0x20, 0x53,
                            0x55, 0x42, 0x4d, 0x41, 0x52, 0x49, 0x4e, 0x45 }; // YELLOW SUBMARINE

    unsigned char iv[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                           0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

    int ciphertext_len, decryptedtext_len;

    /* Load config file, and other important initialisation */
    OPENSSL_init_crypto(OPENSSL_INIT_ADD_ALL_CIPHERS, NULL);

    const unsigned char *plaintext = (unsigned char const *)"A quick nimble.";
    unsigned char ciphertext[128] = {0};
    unsigned char decryptedtext[128] = {0};

    ciphertext_len = aes128_ecb_encrypt(plaintext, strlen((char *)plaintext), ciphertext, key, iv);

    decryptedtext_len = aes128_ecb_decrypt(ciphertext, ciphertext_len, decryptedtext, key, iv);

    EXPECT_STREQ((const char *)plaintext, (const char *)decryptedtext);
}

TEST(OpenSSL, Test1_7)
{
    FILE *fp;
    int fsize = 0;

    fp = fopen("../7.txt", "r");
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
        else if (c == '\n')
            fsize--;
    } while(c != EOF);
    buf[i-1] = '\0'; // clear the EOF in the buffer

    int out_len;
    decoded = decode_base64(buf, fsize, &out_len);

    unsigned char key[] = { 0x59, 0x45, 0x4c, 0x4c, 0x4f, 0x57, 0x20, 0x53,
                            0x55, 0x42, 0x4d, 0x41, 0x52, 0x49, 0x4e, 0x45 }; // YELLOW SUBMARINE

    int decryptedtext_len = 0;
    unsigned char *decryptedtext = (unsigned char *)malloc((out_len+1) * sizeof(char));

    /* Load config file, and other important initialisation */
    OPENSSL_init_crypto(OPENSSL_INIT_ADD_ALL_CIPHERS, NULL);

    decryptedtext_len = aes128_ecb_decrypt((const unsigned char *)decoded, out_len, decryptedtext, key, NULL);
    decryptedtext[decryptedtext_len] = '\0';
    //printf("%s\n", decryptedtext);

    free(buf);
    free(decoded);
    free(decryptedtext);
}

TEST(OpenSSL, Set1_8)
{
    const int CHUNKSIZE = 16;
    FILE *fp;

    fp = fopen("../8.txt", "r");
    if(fp == 0)
    {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    size_t line_size = 0;
    ssize_t nread = 0;
    char *line = NULL;
    int len = 0;
    int out_len = 0;
    char *plaintext = NULL;
    int linescore[400];
    int line_count = 0;
    int max = 0;
    int line_max = 0;

    do {
        nread = getline(&line, &line_size, fp);
        len = nread - 1; // do not care about '\n' and '\0'
        plaintext = decode_base16(line, len, &out_len);

        for(int i = 0; i < (out_len / CHUNKSIZE) - 1; ++i)
        {
            for(int j = i + 1; j < (out_len / CHUNKSIZE); ++j)
            {
                if(0 == memcmp(&plaintext[i*CHUNKSIZE], &plaintext[j * CHUNKSIZE], CHUNKSIZE))
                    ++linescore[line_count];
            }
        }

        if(linescore[line_count] > max)
        {
            line_max = line_count;
            max = linescore[line_count];
        }

        ++line_count;
        free(plaintext);
    } while(nread > 0);

    printf("max: %d, line: %d\n", linescore[line_max], line_max);
    printf("Line %d is probably ECB encoded\n", line_max+1);
    free(line);
    fclose(fp);
}
