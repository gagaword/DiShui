#include<stdio.h>
#include<stdint.h>
#include "Test.h"


//½âÃÜº¯Êý
void decrypt_tea(uint32_t* v, uint32_t* k) {
    uint32_t v0 = v[0], v1 = v[1], sum = 16 * 0x9e3779b9, i;     /* set up */
    uint32_t delta = 0x9e3779b9;                            /* a key schedule constant */
    uint32_t k0 = k[0], k1 = k[1], k2 = k[2], k3 = k[3];    /* cache key */
    for (i = 0; i < 16; i++) {
        sum -= delta;
        v1 -= ((v0 << 4) + k2) ^ (v0 + sum) ^ ((v0 >> 5) + k3);
        v0 -= ((v1 << 4) + k0) ^ (v1 + sum) ^ ((v1 >> 5) + k1);

        /* basic cycle start */
    }                                                           /* end cycle */
    v[0] = v0; v[1] = v1;
}



int main()
{

    uint32_t key[5] = { 0x12345678, 0x09101112, 0x13141516, 0x15161718 };
    uint32_t data[] = {
                    0xBC2B4DF9,
                    0x6213DD13,
                    0x89FFFCC9,
                    0x0FC94F7D,
                    0x526D1D63,
                    0xE341FD50,
                    0x97287633,
                    0x6BF93638,
                    0x83143990,
                    0x1F2CE22C
    };
    data[9] ^= data[0];
    data[8] ^= data[1];

    printf("%x\n", data[9]);
    printf("%x\n", data[8]);

    for (int i = 0; i < 5; ++i)
    {
        decrypt_tea(data, key);
        printf("%#x %#x\n", data[0], data[1]);
    }
    printf("----------------------------------------\n");
    for (int i = 0; i < 10; ++i)
    {
        printf("%#x\n", data[i]);
    }
}