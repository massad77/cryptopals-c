#include "utils.h"

int fixed_xor(const char *in1, const char *in2, char *result, int length)
{
	for(int i = 0; i < length; ++i)
	{
		result[i] = in1[i] ^ in2[i];
	}
	return 0;
}

/*
 * input: text
 * Decrypt text with all possible chars and assign a score based on character frequency
 * output: key with best score
  */
char score_text(const char txt[], int size)
{
	char scores[255];
	char letter = 0;
	init_byte_array(scores, sizeof(scores));

	for(int key = 0; key < sizeof(scores); ++key)
	{
		for(int i = 0; i < size; ++i)
		{
			letter = txt[i] ^ key;
			if(letter == 'a' || letter == 'e' || letter == 'i' || letter == 'o' || letter == 'u')
				scores[key] += 1;
		}
	}

	int masterkey = 0;
	for(int i = 0, max = 0; i < sizeof(scores); ++i)
	{
		if(scores[i] > max)
		{
			masterkey = i;
			max = scores[i];
		}
	}
	return masterkey;
}
