// https://www.cryptopals.com/sets/1/challenges/1
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "base64_conversion.h"
#include "utils.h"
#include "fixed_xor.h"


int main(int argc, char *argv[])
{
	const char input[] = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
	char out_num[128];

	printf("-----Set1-1: Convert hex to base64-----\n");

	init_byte_array(out_num, sizeof(out_num));
	ascii_hex_to_base64(&input[0], get_length(&input[0]), out_num,
						sizeof(out_num));

	printf("Hex: ");
	print_string(input);
	printf("\n");
	printf("Base64: ");
	print_string(out_num);
	printf("\n");

	printf("-----Set1-2: Fixed XOR-----\n");
	const char in1[] = "\x1c\x01\x11\x00\x1f\x01\x01\x00\x06\x1a\x02\x4b\x53\x53\x50\x09\x18\x1c";
	const char in2[] = "\x68\x69\x74\x20\x74\x68\x65\x20\x62\x75\x6c\x6c\x27\x73\x20\x65\x79\x65";
	char res[sizeof(in1)];

	fixed_xor(in1, in2, res, sizeof(in1));

	printf("In 1:\t");
	print_string_hex(in1, sizeof(in1));
	printf("\n");
	printf("In 2:\t");
	print_string_hex(in2, sizeof(in2));
	printf("\n");
	printf("XORed:\t");
	print_string_hex(res, sizeof(res));
	printf("\n");

	printf("-----Set1-3: Single byte XOR cipher-----\n");

	const char msg[] = "\x1b\x37\x37\x33\x31\x36\x3f\x78\x15\x1b\x7f\x2b\x78\x34\x31\x33\x3d\x78\x39\x78\x28\x37\x2d\x36\x3c\x78\x37\x3e\x78\x3a\x39\x3b\x37\x36";

	int masterkey = score_text(msg, sizeof(msg));

	/* decrypt */
	char letter = 0;
	printf("Masterkey used: %d\n", masterkey);
	for(int i = 0; i < sizeof(msg) - 1; ++i) // do not decrypt terminating '\0'
	{
		letter = msg[i] ^ masterkey;
		printf("%c", letter);
	}
	printf("\n");

	return 0;
}
