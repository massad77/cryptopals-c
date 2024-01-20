#ifndef BASE64_CONVERSION_H
#define BASE64_CONVERSION_H

/* input: char * -> number as hex digits, ascii coded
 * output: char * -> number as base_radix digits, ascii coded
 */
int ascii_hex_to_base64(const char *in_num, int in_len, char *out_num, int out_len);

/* input: char * -> number as base_radix digits, ascii coded
 * output: char * -> number as hex digits, ascii coded
 */
int ascii_base64_to_hex(const char *in_num, int in_len, char *out_num, int out_len);

#endif
