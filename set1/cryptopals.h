#ifndef CRYPTOPALS_H_
#define CRYPTOPALS_H_

int hamming_distance(char const * const a, char const * const b, int const len);
int estimate_keysize(char const * const buf, const int len);
int aes128_ecb_decrypt(unsigned char const *chiffre, int const chiffre_len, unsigned char *plaintext, unsigned char const *key,  unsigned char const *iv);
int aes128_ecb_encrypt(unsigned char const *plaintext, int const plaintext_len, unsigned char *chiffre, unsigned char const *key,  unsigned char const *iv);

#endif // CRYPTOPALS_H_
