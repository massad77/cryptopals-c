#ifndef __UTILS_H_
#define __UTILS_H_

#include <stdio.h>

int get_length (const char *str)
{
    int len = 0;
    for(int i = 0; str[i] != '\0'; ++i)
        ++len;
    return len;
}

int print_string(const char *str)
{
    int len = 0;
    for(len = 0; str[len] != '\0'; ++len)
        printf("%c", str[len]);
    return len;
}

void init_byte_array(char *array, int length)
{
    for(int i = 0; i < length; ++i)
        array[i] = 0x00;
}

#endif // __UTILS_H_
