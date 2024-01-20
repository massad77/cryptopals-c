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

    for(int i = 2; i < len/4 && i < 40; ++i)
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

