#include <stdio.h>

#include "base64_conversion.h"

/* swap the last byte in the array with the first one etc.
 */
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
static char encode_ascii_base64(char num)
{
	char base64_digit = '\0';
	if(num >= 0 && num <= 0x19 /* 25 */) base64_digit = num + 'A';
	if(num >= 26 && num <= 0x33 /* 51 */) base64_digit = num + 'a' - 26;
	if(num >= 52 && num <= 0x3D /* 61 */) base64_digit = num + '0' - 52;
	if(num == 0x3E /* 62 */) base64_digit = '+';
	if(num == 0x3F /* 63 */) base64_digit = '/';
	return base64_digit;
}

/* input: ascii encoded representation of num (base64)
   output: num
*/
static char decode_ascii_base64(char base64_digit)
{
	char num = -1;
	if(base64_digit >= 'A' && base64_digit <= 'Z' /* 25 */) num = base64_digit - 'A';
	if(base64_digit >= 'a' && base64_digit <= 'z' /* 51 */) num = base64_digit - 'a' + 26;
	if(base64_digit >= '0' && base64_digit <= '9' /* 61 */) num = base64_digit - '0' + 52;
	if(base64_digit == '+' /* 62 */) num = 0x3E;
	if(base64_digit == '/' /* 63 */) num = 0x3F;
	return num;
}

/* input: ascii encoded hex digit
   output: value
*/
static char decode_ascii_base16(char ascii)
{
	char value = -1;
	if(ascii >= '0' && ascii <= '9') value = ascii - '0';
	if(ascii >= 'a' && ascii <= 'f') value = ascii - 'a' + 10;
	if(ascii >= 'A' && ascii <= 'F') value = ascii - 'A' + 10;
	return value;
}

/* input: 4 bits
   output: ascii encoded value
*/
static char encode_ascii_base16(unsigned char value)
{
	char ascii = '\0';
	if(value >= 0 && value <= 9) ascii = value + '0';
	if(value >= 10 && value <= 15) ascii = value + 'a' - 10;
	return ascii;
}

/* input: char * -> number as hex digits, ascii coded
 * output: char * -> number as base_radix digits, ascii coded
 */
int ascii_hex_to_base64(char *in_num, int in_len, char *out_num, int out_len)
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
			nipple1 = decode_ascii_base16(in_num[i]);
			++j;
			break;
		case 1:
			nipple2 = decode_ascii_base16(in_num[i]);
			byte = (0x3f & ((nipple2 << 4) | (nipple1)));
			out_num[k++] = encode_ascii_base64(byte);
			++j;
			break;
		case 2:
			nipple1 = decode_ascii_base16(in_num[i]);
			byte = (nipple1 << 2) | ((0x0c & nipple2) >> 2); 
			out_num[k++] = encode_ascii_base64(byte);
			j = 0;
			break;
		default:
			return -1;
		}

	}
	/* one extra nipple */
	if(j == 1)
	{
		out_num[k++] = encode_ascii_base64(nipple1);
	}
	if(j == 2)
	{
		out_num[k++] = encode_ascii_base64((0x0f & nipple2 >> 2));
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
int ascii_base64_to_hex(char *in_num, int in_len, char *out_num, int out_len)
{
	if(in_num == NULL || out_num == NULL) return -1;

	int i = 0;
	int j = 0;
	int k = 0;
	char part0 = 0x00;
	char part1 = 0x00;
	char part2 = 0x00;
	char part3 = 0x00;
	char byte0 = 0x00;
	char byte1 = 0x00;
	char byte2 = 0x00;
	for(i = (in_len - 1); i >= 0; --i)
	{
		switch(j)
		{
		case 0:
			part0 = decode_ascii_base64(in_num[i]); /* 6 bits */
			++j;
			break;
		case 1:
			part1 = decode_ascii_base64(in_num[i]);
			byte0 = (((0x03 & part1) << 6) | part0);
			/* two chars are necessary to represent byte in ascii */
			out_num[k++] = encode_ascii_base16(byte0 & 0x0f);
			out_num[k++] = encode_ascii_base16((byte0 & 0xf0) >> 4);
			++j;
			break;
		case 2:
			part2 = decode_ascii_base64(in_num[i]);
			byte1 = (((0x0f & part2) << 4) | (0x3c & part1) >> 2); 
			/* two chars are necessary to represent byte in ascii */
			out_num[k++] = encode_ascii_base16(byte1 & 0x0f);
			out_num[k++] = encode_ascii_base16((byte1 & 0xf0) >> 4);
			++j;
			break;
		case 3:
			part3 = decode_ascii_base64(in_num[i]);
			byte2 = ((part3 << 2) | (0x30 & part2) >> 4);
			/* two chars are necessary to represent byte in ascii */
			out_num[k++] = encode_ascii_base16(byte2 & 0x0f);
			out_num[k++] = encode_ascii_base16((byte2 & 0xf0) >> 4);
			j = 0;
			break;
		default:
			return -1;
		}

	}
	switch(j)
	{
	case 1:
		out_num[k++] = encode_ascii_base16(part0 & 0x0f);
		out_num[k++] = encode_ascii_base16((part0 & 0xf0) >> 4);
		break;
	case 2:
		part1 = (0xfc & part1) >> 2;
		if(part1 == 0x00) break;
		out_num[k++] = encode_ascii_base16(part1 & 0x0f);
		out_num[k++] = encode_ascii_base16((part1 & 0xf0) >> 4);
		break;
	case 3:
		part2 = (0xf0 & part2) >> 4;
		if(part2 == 0x00) break;
		out_num[k++] = encode_ascii_base16(part2 & 0x0f);
		out_num[k++] = encode_ascii_base16((part2 & 0xf0) >> 4);
		break;
	default:
		break;
	}

	/* delete leading 0 -> '0'(base16) and reduce size of array by one */
	if((k > 1) && (out_num[k-1] == '0')) out_num[--k] = '\0';

	/* Put MSB at the beginning of the array and LSB at the end */
	reverse_array(out_num, k);

	return k;
}
