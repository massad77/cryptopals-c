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
void singlebyte_xor(char const * const buf, char *res, int len, const char key);

/*
 * input: text, size
 * Score text based on most common chars in english
 * output: score
 */
float score_text(const char txt[], int size);

/*
 * Takes XOR encrypted buffer and empty buffer.
 * Returns decrypted result in buffer and decryption key.
 */
char break_singlebyte_xor(char const * const buf, char *res, int len);

/*
 * input: text to encode, key, lengths
 * output: encoded text
 */
char *XORencode(char const * const in, int len, char const * const key, int key_len);

#endif // __FIXED_XOR_H_
