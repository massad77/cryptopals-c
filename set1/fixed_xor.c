#include <ctype.h>
#include <stdlib.h>
#include "utils.h"

typedef struct {
	char letter;
	float freq;
} letter_freq_t;

/* Reference: http://pi.math.cornell.edu/~mec/2003-2004/cryptography/subs/frequencies.html */
static const letter_freq_t ENG_COMMON_LETTER[] = {
	{'e', 12.02},
	{'t', 9.10},
	{'a', 8.12},
	{'o', 7.68},
	{'i', 7.31},
	{'n', 6.95},
	{'s', 6.28},
	{'r', 6.02},
	{'h', 5.92},
	{'d', 4.32},
	{'l', 3.98},
	{'u', 2.88},
	{'c', 2.71},
	{'m', 2.61},
	{'f', 2.30},
	{'y', 2.11},
	{'w', 2.09},
	{'g', 2.03},
	{'p', 1.82},
	{'b', 1.49},
	{'v', 1.11},
	{'k', 0.69},
	{'x', 0.17},
	{'q', 0.11},
	{'j', 0.10},
	{'z', 0.10},
	{' ', 4.0}
};

static float letter_freq(char c)
{
	for(unsigned long i = 0; i < COUNT_OF(ENG_COMMON_LETTER); ++i)
	{
		if(ENG_COMMON_LETTER[i].letter == c) return ENG_COMMON_LETTER[i].freq;
	}
	return 0;
}

void singlebyte_xor(char const * const buf, char *res, const int len, const char key)
{
	if(buf == NULL) return;

	for(int i = 0; i < len; ++i)
	{
		res[i] = buf[i] ^ key;
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
 * input: text, size
 * Score text based on most common chars in english
 * output: score
 */
float score_text(const char txt[], int size)
{
	float score = 0;

	/* count english letters */
	for(int i = 0; i < size; ++i)
		score += letter_freq(tolower(txt[i]));

	/* LEARNINGS
	 * Counting the character frequency and multiplying with the desired
	 * frequency leads to good results and solves 1.3 but only if you include
	 * spaces.
	 */

	return score;
}

char *XORencode(char const * const in, int len, char const * const key, int key_len)
{
	char *out = calloc(len, sizeof(char));

	/* ignore '\0' at the end */
	for(int i = 0, j = 0; i < len - 1; ++i)
	{
		out[i] = in[i] ^ key[j++];
		j %= (key_len - 1);
	}

	return out;
}
