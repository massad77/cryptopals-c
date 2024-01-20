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

	printf("-----Set1-4: Detect single-character XOR -----\n");
	FILE *fp;
	char buff[255];
	char *line = NULL;
	int line_count = 0;
	int len = 0;
	size_t line_size = 0;
	ssize_t nread = 0;
	int score = 0;
	int max = 0;
	int key = 0;

	typedef struct {
		int key;
		int score;
	} keyscore_t;

	keyscore_t linescore[400];

	fp = fopen("4.txt", "r");
	if(fp == 0)
	{
		perror("fopen");
		exit(EXIT_FAILURE);
	}

	do {
		int local_max = 0;
		int local_key = 0;
		nread = getline(&line, &line_size, fp);
		len = nread - 1; // do not care about '\n' and '\0'
		/* printf("line_size: %ld\n", line_size); */
		/* printf("nread: %ld\n", nread); */
		for(int i = 0; i < len; ++i)
		{
			buff[i] = decode_ascii_base16(line[i]);
			//printf("%x", buff[i]);
		}
		//printf("\n");

		for(unsigned char c = 0; c < 255; ++c)
		{
			//score = score_text2(buff, len, c);
			if(score > local_max)
			{
				local_max = score;
				local_key = c;
			}
		}
		//printf("Line: %d, Key: %d, score: %d\n", line_count, local_key, local_max);
		linescore[line_count].key = local_key;
		linescore[line_count].score = local_max;

		if(local_max > max)
		{
			max = local_max;
			key = local_key;
		}

		if(local_max >= 125)
		{
			printf("Line: %d, Key: %d, score: %d\n", line_count, local_key, local_max);
			//printf("%s", line);
			/* decrypt */
			for(int i = 0; i < len; ++i)
			{
				buff[i] = buff[i] ^ key;
				printf("%c", buff[i]);
			}
			printf("\n");
			//exit(1);
			printf("DONE\n");
		}
		++line_count;
	} while (nread > 0);

	printf("Line: %d, maxKey: %d, maxscore: %d\n", line_count, key, max);

	//printf("Line: %d, Key: %d, score: %d\n", 0, linescore[0].key, linescore[0].score);
	for(int i = 0; i < 255; ++i)
	{
		if(linescore[i].score > 200)
		{
			printf("Line: %d, Key: %d, score: %d\n", i, linescore[i].key, linescore[i].score);
		}
	}

	free(line);
	fclose(fp);
	return 0;
}
