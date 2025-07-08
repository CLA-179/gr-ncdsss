

#include "gold.h"
#include <stdio.h>

int count_one(unsigned short in)
{
    static int out[16] = { 0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4 };

    int o = out[in & 0x0f];
    o += out[(in >> 4) & 0x0f];
    o += out[(in >> 8) & 0x0f];
    o += out[(in >> 12) & 0x0f];

    // printf ("%d\n", o);

    return o;
}

int count_one32(unsigned int in)
{
    static int out[16] = { 0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4 };

    int o = out[in & 0x0f];
    o += out[(in >> 4) & 0x0f];
    o += out[(in >> 8) & 0x0f];
    o += out[(in >> 12) & 0x0f];
    o += out[(in >> 16) & 0x0f];
    o += out[(in >> 20) & 0x0f];
    o += out[(in >> 24) & 0x0f];
    o += out[(in >> 28) & 0x0f];

    // printf ("%d\n", o);

    return o;
}

void gold_gen(unsigned short _x1,
              unsigned short _x2,
              unsigned short _n1,
              unsigned short _n2,
              unsigned char* gold)
{
    unsigned short x1, x2, n1, n2, temp;

    x1 = _x1;
    n1 = _n1;
    x2 = _x2;
    n2 = _n2;

    for (int i = 0; i < 1023; i++) {
        gold[i] = ((x1 ^ x2) & 0x200) >> 9;
        printf("%d", ((x1 ^ x2) & 0x200) >> 9);

        temp = (count_one(x1 & n1) & 0x01);
        x1 = ((x1 << 1) & 0x3fe) | temp;

        temp = (count_one(x2 & n2) & 0x01);
        x2 = ((x2 << 1) & 0x3fe) | temp;
    }
    printf("\n");
}

void gold_buf_gen(unsigned char* gold, unsigned int* gold_buf)
{
    for (unsigned short i = 0; i < 1023; i ++)
    {
        for (unsigned char j = 0; j < 32; j++)
        {
            gold_buf[i] <<= 1;
            gold_buf[i] |= (i + j < 1023 ? gold[i + j] : gold[i + j - 1023]) & 0x01U;
            
        }
        for (unsigned char j = 0; j < 32; j++)
        {
            printf("%d", (gold_buf[i] >> j) & 0x01);
        }
        printf("\n");
        
        
    }
    // exit(0);
}
