#ifndef __FIXED_XOR_H_
#define __FIXED_XOR_H_

/*
  Takes two equal sized buffers and XORs them
 */
int fixed_xor(const char *in1, const char *in2, char *result, int length);
/*
 * Inout: buf
 * XORs 'buf' of length 'len with 'key'
 */
void singlebyte_xor(char *buf, int len, const char key);

char score_text(const char txt[], int size);

/*
 * input: text, size
 * Score text based on most common chars in english
 * output: score
 */
char score_text2(const char txt[], int size);

#endif // __FIXED_XOR_H_
