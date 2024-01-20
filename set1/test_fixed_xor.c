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
    printf("All tests passed! (%i)\n", num_tests);

    return 0;
}
