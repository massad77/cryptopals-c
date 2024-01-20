#include <openssl/err.h>
#include <openssl/evp.h>
#include "cryptopals.h"

static float score_keysize(char const * const buf, int const len, int const keysize)
{
    float dist = 0.0;
    if(len/keysize < 4) return -1;
    int iter = len / keysize;

    for(int i = 0; i < (iter - 1); ++i)
    {
        int ham_dist = hamming_distance(&buf[i * keysize], &buf[(i + 1) * keysize], keysize);
        dist = dist + (float) ham_dist / (float) keysize;
    }
    dist = dist/iter;

    return dist;
}

int hamming_distance(char const * const a, char const * const b, int const len)
{
    // https://en.wikipedia.org/wiki/Hamming_distance
    int dist = 0;
    for(int i = 0; i < len; ++i)
    {
        // The ^ operators sets to 1 only the bits that are different
        for(unsigned val = a[i] ^ b[i]; val > 0; ++dist)
        {
            // We then count the bit set to 1 using the Peter Wegner way
            val = val & (val - 1); // Set to zero val's lowest-order 1
        }
    }
    return dist;
}

int estimate_keysize(char const * const buf, const int len)
{
    if(len < 2) return -1;
    float dist = 0;
    float dist_min = 2048*4;
    int keysize = -2;
    const int MAX_KEYSIZE = 40;

    for(int i = 2; i < MAX_KEYSIZE; ++i)
    {
        dist = score_keysize(buf, len, i);
        if(dist > 0 && dist < dist_min)
        {
            dist_min = dist;
            keysize = i;
        }
    }
    return keysize;
}

static void ssl_handle_errors(void)
{
    ERR_print_errors_fp(stderr);
    abort();
}

int aes128_ecb_decrypt(unsigned char const *chiffre, int const chiffre_len, unsigned char *plaintext, unsigned char const *key,  unsigned char const *iv)
{
    int len = -1;
    int plaintext_len = -1;
    EVP_CIPHER_CTX *ctx;

    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new()))
        ssl_handle_errors();

     /*
     * Initialise the decryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 128 bit AES (i.e. a 128 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits
     */
    if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, iv))
        ssl_handle_errors();

    /*
     * Provide the message to be decrypted, and obtain the plaintext output.
     * EVP_DecryptUpdate can be called multiple times if necessary.
     */
    if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, chiffre, chiffre_len))
        ssl_handle_errors();
    plaintext_len = len;

    /*
     * Finalise the decryption. Further plaintext bytes may be written at
     * this stage.
     */
    if(1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len))
        ssl_handle_errors();
    plaintext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    return plaintext_len;
}

int aes128_ecb_encrypt(unsigned char const *plaintext, int const plaintext_len, unsigned char *chiffre, unsigned char const *key,  unsigned char const *iv)
{
    int len = -1;
    int chiffre_len = -1;
    EVP_CIPHER_CTX *ctx;

    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new()))
        ssl_handle_errors();

    if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, iv))
        ssl_handle_errors();

    if(1 != EVP_EncryptUpdate(ctx, chiffre, &len, plaintext, plaintext_len))
        ssl_handle_errors();
    chiffre_len = len;

    if(1 != EVP_EncryptFinal_ex(ctx, chiffre + len, &len))
        ssl_handle_errors();
    chiffre_len += len;

    EVP_CIPHER_CTX_free(ctx);

    return chiffre_len;
}
