#ifndef __UTILS_H_
#define __UTILS_H_

#include <stdio.h>

static inline int get_length (const char *str)
{
    int len = 0;
    for(int i = 0; str[i] != '\0'; ++i)
        ++len;
    return len;
}

static inline int print_string(const char *str)
{
    int len = 0;
    for(len = 0; str[len] != '\0'; ++len)
        printf("%c", str[len]);
    return len;
}

static inline void print_string_hex(const char *str, int size)
{
    printf("0x");
    for(int i = 0; i < size - 1; ++i) // do not print the trailing '\0'
        printf("%02x", str[i]);
}

static inline void init_byte_array(char *array, int length)
{
    for(int i = 0; i < length; ++i)
        array[i] = 0x00;
}

#endif // __UTILS_H_
