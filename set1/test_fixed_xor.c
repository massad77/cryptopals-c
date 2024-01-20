#include "fixed_xor.h"
#include "string.h"
#include "utils.h"

#define ARRAY_SIZE 3

static void print_test_header(const char *in[ARRAY_SIZE])
{
    printf("In1:\t\t\t\t0x");
    for(int i = 0; *(in[0]+i) != '\0'; ++i) printf("%02x", *(in[0]+i));
    printf("\nIn2:\t\t\t\t0x");
    for(int i = 0; *(in[1]+i) != '\0'; ++i) printf("%02x", *(in[1]+i));
    printf("\nExpected output:\t0x");
    for(int i = 0; *(in[2]+i) != '\0'; ++i) printf("%02x", *(in[2]+i));
    printf("\n");
}

int main(int argc, char *argv[])
{
    /* fixed_xor(...) */
    const char *test_strings[][ARRAY_SIZE]= {
        {"\x55", "\x00", "\x55"},
        {"\x55", "\x00", "\x55"},
        {"\x55", "\xaa", "\xff"},
        {"\x55\x55", "\x55\x55", "\x00\x00"},
        {"\xff\xff", "\x00\x00", "\xff\xff"}
    };

    char res[16];
    const int num_tests = sizeof(test_strings)/sizeof(test_strings[0]);

    for(int i = 0; i < num_tests; ++i)
    {

        init_byte_array(res, sizeof(res));
        fixed_xor(test_strings[i][0], test_strings[i][1], res,
                  sizeof(test_strings[i][0]));

        if(strcmp(res, test_strings[i][2]))
        {
            printf("Test failed!\n");
            print_test_header(test_strings[i]);
            printf("Res:\t\t\t\t0x");
            for(int i = 0; res[i] != '\0'; ++i) printf("%02x", res[i]);
            printf("\n");
            return -1;
        }
    }

    /* singlebyte_xor(...) */
    char *test_bytes[][ARRAY_SIZE]= {
        {"\x55", "\x00", "\x55"},
        {"\x55", "\x00", "\x55"},
        {"\x55", "\xaa", "\xff"}
    };

    for(int i = 0; i < COUNT_OF(test_bytes); ++i)
    {

        printf("%x\n", *test_bytes[i][0]);
        printf("%d\n", get_length(test_bytes[i][0]));
        printf("%02x\n", *((unsigned char *)test_bytes[i][1]));
        /* singlebyte_xor(test_bytes[i][0], get_length(test_bytes[i][0]), */
        /*           *((unsigned char *)test_bytes[i][1])); */

        char *buf = "\xa5";
        printf("%x\n", *buf);
        singlebyte_xor(buf, 1, 0xaa);
        printf("%x\n", *buf);
        /* test_bytes[i][0] now contains XORed value */
        if(strcmp(test_bytes[i][0], test_strings[i][2]))
        {
            printf("Singelbyte XOR test failed!\n");
            printf("Should: %x, Is: %x\n", *test_bytes[i][0], *test_bytes[i][2]);
            return -1;
        }
    }

    /* score_text(...) */
    typedef struct {
        char *txt;
        int score;
    } text_score_t;

    const text_score_t test_text[] = {
        {"etaoinsrhldu", 80},
        {"EtaOinsrHldu", 80},
        {"eee", 36},
        {"EEE", 36},
        {"uab", 11},
        {"abc", 8},
        {"xYz", 0}
    };

    for(int i = 0; i < COUNT_OF(test_text); ++i)
    {
        int score = score_text2(test_text[i].txt, get_length(test_text[i].txt));
        if(score != test_text[i].score)
        {
            printf("Score test failed!\n");
            printf("Test string: %s\n", test_text[i].txt);
            printf("Should: %d, Is: %d\n", test_text[i].score, score);
            return -1;
        }
    }

    printf("All tests passed! (%i)\n", num_tests);

    return 0;
}
