#include <ctype.h>
#include "utils.h"

typedef struct {
	char letter;
	int freq;
} letter_freq_t;

/* Reference: http://pi.math.cornell.edu/~mec/2003-2004/cryptography/subs/frequencies.html */
static const letter_freq_t ENG_COMMON_LETTER[] = {
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
	{'u', 3},
	{' ', 14}
};

static int letter_freq(char c)
{
	for(int i = 0; i < COUNT_OF(ENG_COMMON_LETTER); ++i)
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
 * input: text, size
 * Score text based on most common chars in english
 * output: score
 */
int score_text(const char txt[], int size)
{
	int score = 0;

	/* count english letters */
	for(int i = 0; i < size; ++i)
	{
		score += letter_freq(tolower(txt[i]));
	}
	return score;
}
