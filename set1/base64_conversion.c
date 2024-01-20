#include <stdio.h>
#include <stdlib.h>

#include "base64_conversion.h"

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
	char num = 0;
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

/* input: hex-encoded string with ASCII characters
 * output: binary data
 */
char *decode_base16(char const * const in, const int in_len, int *out_len)
{
	if(in == NULL || in_len == 0) return NULL;

	*out_len = in_len / 2 + 2;
	char *out = calloc(*out_len, sizeof(char));
	if(out == NULL) return NULL;

	if(in_len == 1)
	{
		out[0] = decode_ascii_base16(in[0]);
		out[(*out_len) - 1] = '\0';
		return out;
	}

	for(int i = 0; i < in_len / 2; i++)
	{
		out[i] |= decode_ascii_base16(in[i*2]) << 4;
		out[i] |= decode_ascii_base16(in[i*2+1]);
	}

	/* if odd number of ascii chars, consider the last one */
	if(in_len % 2 != 0)
	{
		out[(in_len - 1)/ 2] = decode_ascii_base16(in[in_len-1]) << 4;
	}
	return out;
}

/* input: base64-encoded string with ASCII characters
 * output: binary data
 */
char *decode_base64(char const * const in, const int in_len, int *out_len)
{
	*out_len = (in_len * 6) / 8 + 2;
	char *out = calloc(*out_len, sizeof(char));

	int j = 0;
	for(int i = 0; i < in_len; ++i)
	{
		switch(i%4)
		{
			case 0:
				out[j] = decode_ascii_base64(in[i]) << 2;
				j++;
				break;
			case 1:
				out[j-1] |= (decode_ascii_base64(in[i]) >> 4) & 0x03;
				out[j] = (decode_ascii_base64(in[i]) << 4);
				j++;
				break;
			case 2:
				out[j-1] |= (decode_ascii_base64(in[i]) >> 2) & 0x0F;
				out[j] = (decode_ascii_base64(in[i]) << 6);
				j++;
				break;
			case 3:
				out[j-1] |= decode_ascii_base64(in[i]) & 0x3F;
				/* do not increment 'j' here! */
				break;
			default:
				break;
		}
	}

	return out;
}
