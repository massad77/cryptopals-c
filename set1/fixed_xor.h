#ifndef __FIXED_XOR_H_
#define __FIXED_XOR_H_

/*
  Takes two equal sized buffers and XORs them
 */
int fixed_xor(const char *in1, const char *in2, char *result, int length);
char score_text(const char txt[], int size);

#endif // __FIXED_XOR_H_
