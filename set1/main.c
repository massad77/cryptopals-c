// https://www.cryptopals.com/sets/1/challenges/1
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "base64_conversion.h"
#include "utils.h"
#include "fixed_xor.h"

const char input[] = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";

int main(int argc, char *argv[])
{
	char out_num[128];

	init_byte_array(out_num, sizeof(out_num));
	ascii_hex_to_base64(&input[0], get_length(&input[0]), out_num,
						sizeof(out_num));
	print_string(out_num);
	printf("\n");

	return 0;
}
