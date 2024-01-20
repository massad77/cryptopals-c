#include <stdio.h>
#include <string.h>
#include "base64_conversion.h"
#include "utils.h"

#define NUM_TESTS 12

static int print_test_header(char *input, char *expected_output)
{
    printf("Input: ");
    for(int i = 0; input[i] != '\0'; ++i) printf("%c", input[i]);
    printf(" Expected output: ");
    for(int i = 0; expected_output[i] != '\0'; ++i) printf("%c", expected_output[i]);
    printf("\n");
    return 0;
}

int main(int argc, char *argv[])
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
        print_test_header(test_strings[i][0], test_strings[i][1]);
        int len = ascii_hex_to_base64(test_strings[i][0],
                    get_length(test_strings[i][0]), out_num,
                    sizeof(out_num));

        if(strcmp(out_num, test_strings[i][1]))
        {
            printf("Test failed!\n");
            if(len > 0) print_string(out_num);
            return -1;
        }
    }
    printf("All tests passed!\n");

    for(int i = 0; i < NUM_TESTS; ++i)
    {
        init_byte_array(out_num, sizeof(out_num));
        print_test_header(test_strings[i][1], test_strings[i][0]);
        int len = ascii_base64_to_hex(test_strings[i][1],
                    get_length(test_strings[i][1]), out_num,
                    sizeof(out_num));

        if(strcmp(out_num, test_strings[i][0]))
        {
            printf("Test failed!\n");
            if(len > 0) print_string(out_num);
            return -1;
        }
    }
    printf("All tests passed!\n");

    if(argc > 1)
    {
        init_byte_array(out_num, sizeof(out_num));
        ascii_hex_to_base64(argv[1], get_length(argv[1]), out_num,
                  sizeof(out_num));
        print_string(out_num);
        printf("\n");

        char hex_num[128];
        init_byte_array(hex_num, sizeof(hex_num));
        ascii_base64_to_hex(out_num, get_length(out_num), hex_num,
                  sizeof(hex_num));
        print_string(hex_num);
    }

    return 0;
}
