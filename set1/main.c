// https://www.cryptopals.com/sets/1/challenges/1
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

static int get_length (char *str)
{
	int len = 0;
	for(int i = 0; str[i] != '\0'; ++i)
	{
		++len;
	}
	return len;
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
	if(argc > 1)
	{
		char *num_str = argv[1];
		char out_num[128];
		int len = 0;
		for(int i = 0; i < 128; ++i)
			out_num[i] = 0;
		len = hex_to_base64(num_str, get_length(num_str), out_num, sizeof(out_num));
		if(len < 0)
		{
			printf("FAIL\n");
			return -1;
		}

		for(int i = len; i >= 0; --i)
		{
			printf("%c", out_num[i]);
		}
	}
	else
	{
		printf("Pass hex number e.g 'a1'...");
	}

	return 0;
}
