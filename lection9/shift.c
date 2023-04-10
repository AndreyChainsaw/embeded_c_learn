#include <stdio.h>


int main(void)
{
    // << 

    // 01100000
    char a = 0x60;

    printf("Before = %d\n", a);
    
    // 11000000
    printf("After << 1 = %d\n", (char)(a << 1));

    // 10000000
    printf("After << 2 = %d\n", (char)(a << 2));

    // 00000000
    printf("After << 3 = %d\n", (char)(a << 3));

    // Дальше будет число "0", независимо от порядка сдвига и его направления


    // >> 

    // 10000000 (отрицательное число)
    a = 0x80;

    printf("\nBefore = %d\n", a);
    
    // 11000000
    printf("After >> 1 = %d\n", (char)(a >> 1));

    // 10100000
    printf("After >> 2 = %d\n", (char)(a >> 2));

    // 10010000
    printf("After >> 3 = %d\n", (char)(a >> 3));

    // 10001000
    printf("After >> 4 = %d\n", (char)(a >> 4));

    // 10000100
    printf("After >> 5 = %d\n", (char)(a >> 5));

    // 10000010
    printf("After >> 6 = %d\n", (char)(a >> 6));

    // 10000001
    printf("After >> 7 = %d\n", (char)(a >> 7));
    
    // 10000001???
    printf("After >> 8 = %d\n", (char)(a >> 8));


    return 0;
}