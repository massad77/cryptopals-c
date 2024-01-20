#include "cryptopals.h"

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
