#include <ctype.h>
#include "utils.h"

typedef struct {
	char letter;
	int freq;
} letter_freq_t;

/* Reference: http://pi.math.cornell.edu/~mec/2003-2004/cryptography/subs/frequencies.html */
static const letter_freq_t ENG_COMMON_LETTER[12] = {
	{'e', 12},
	{'t', 9},
	{'a', 8},
	{'o', 8},
	{'i', 7},
	{'n', 7},
	{'s', 6},
	{'r', 6},
	{'h', 6},
	{'d', 4},
	{'l', 4},
	{'u', 3}
};

static int letter_freq(char c)
{
	for(int i = 0; i < 12; ++i)
	{
		if(ENG_COMMON_LETTER[i].letter == c) return ENG_COMMON_LETTER[i].freq;
	}
	return 0;
}

void singlebyte_xor(char *buf, int len, const char key)
{
	for(int i = 0; i < len; ++i)
	{
		*(buf+i) ^= key;
	}
}

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

/*
 * input: text, size
 * Score text based on most common chars in english
 * output: score
 */
char score_text2(const char txt[], int size)
{
	int score = 0;

	for(int i = 0; i < size; ++i)
	{
		score += letter_freq(tolower(txt[i]));
	}
	return score;
}

