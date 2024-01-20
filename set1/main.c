// https://www.cryptopals.com/sets/1/challenges/1
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_TESTS 8

static int get_length (char *str)
{
	int len = 0;
	for(int i = 0; str[i] != '\0'; ++i)
	{
		++len;
	}
	return len;
}

static int print_test_header(char *input, char *expected_output)
{
	printf("Input: ");
	for(int i = 0; input[i] != '\0'; ++i) printf("%c", input[i]);
	printf(" Expected output: ");
	for(int i = 0; input[i] != '\0'; ++i) printf("%c", expected_output[i]);
	printf("\n");
	return 0;
}

static int reverse_array(char *array, int length)
{
	char tmp;
	for(int i = 0; i < length / 2; ++i)
	{
		tmp = array[i];
		array[i] = array[length - 1 - i];
		array[length - 1 - i] = tmp;
	}
	return 0;
}

/* input: value 
   output: ascii encoded representation of value
*/
static char base64_to_ascii(char num)
{
	char base64_digit;
	if(num >= 0 && num <= 0x19 /* 25 */) base64_digit = num + 'A';
	if(num >= 26 && num <= 0x33 /* 51 */) base64_digit = num + 'a' - 26;
	if(num >= 52 && num <= 0x3D /* 61 */) base64_digit = num + '0' - 52;
	if(num == 0x3E /* 62 */) base64_digit = '+';
	if(num == 0x3F /* 63 */) base64_digit = '/';
	return base64_digit;
}

/* input: ascii encoded hex digit
   output: value
*/
static char ascii_to_base16(char ascii)
{
	char value;
	if(ascii >= '0' && ascii <= '9') value = ascii - '0';
	if(ascii >= 'a' && ascii <= 'f') value = ascii - 'a' + 10;
	if(ascii >= 'A' && ascii <= 'F') value = ascii - 'A' + 10;
	return value;
}

/* input: char * -> number as hex digits, ascii coded
 * output: char * -> number as base_radix digits, ascii coded
 */
static int hex_to_base64(char *in_num, int in_len, char *out_num, int out_len)
{
	if(in_num == NULL || out_num == NULL) return -1;

	int i = 0;
	int j = 0;
	int k = 0;
	char nipple1 = 0x00;
	char nipple2 = 0x00;
	char byte = 0x00;
	for(i = (in_len - 1); i >= 0; --i)
	{
		switch(j)
		{
		case 0:
			nipple1 = ascii_to_base16(in_num[i]);
			++j;
			break;
		case 1:
			nipple2 = ascii_to_base16(in_num[i]);
			byte = (0x3f & ((nipple2 << 4) | (nipple1)));
			out_num[k++] = base64_to_ascii(byte);
			++j;
			break;
		case 2:
			nipple1 = ascii_to_base16(in_num[i]);
			byte = (nipple1 << 2) | ((0x0c & nipple2) >> 2); 
			out_num[k++] = base64_to_ascii(byte);
			j = 0;
			break;
		default:
			return -1;
		}

	}
	/* one extra nipple */
	if(j ==1)
	{
		out_num[k++] = base64_to_ascii(nipple1);
	}
	if(j == 2)
	{
		out_num[k++] = base64_to_ascii((0x0f & nipple2 >> 2));
	}

	/* delete leading 0 -> 'A'(base64) and reduce size of array by one */
	if((k > 1) & (out_num[k-1] == 'A')) out_num[--k] = 0x00;


	/* Put MSB at the beginning of the array and LSB at the end */
	reverse_array(out_num, k);
	return k;
}

/* input: char * -> number as base_radix digits, ascii coded
 * output: char * -> number as hex digits, ascii coded
 */
static int base64_to_hex(char *in_num, int in_len, char *out_num, int out_len)
{
	return 0;
}

int main(int argc, char *argv[])
{
	char out_num[128];
	for(int i = 0; i < 128; ++i)
		out_num[i] = 0;

	char *test_strings[NUM_TESTS][NUM_TESTS] = {
		{ "0", "A" },
		{ "1", "B" },
		{ "a", "K" },
		{ "A", "K" },
		{ "f", "P" },
		{ "3f", "/" },
		{ "40", "BA" },
		{ "81", "CB" }
	};

	for(int i = 0; i < NUM_TESTS; ++i)
	{
		print_test_header(test_strings[i][0], test_strings[i][1]);
		hex_to_base64(test_strings[i][0],
				    get_length(test_strings[i][0]), out_num,
				    sizeof(out_num));

		if(strcmp(out_num, test_strings[i][1]))
		{
			printf("Test failed!\n");
			return -1;
		}
	}
	printf("All tests passed!");

	return 0;
}
