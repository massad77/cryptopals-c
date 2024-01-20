#ifndef BASE64_CONVERSION_H
#define BASE64_CONVERSION_H

char *decode_base16(char const * const in, const int in_len, int *out_len);
char *decode_base64(char const * const in, const int in_len, int *out_len);

#endif
