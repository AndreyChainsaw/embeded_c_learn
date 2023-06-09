#include <stdio.h>


int main(void)
{
    // Берем максимальное отрицательное число
    char a = 0x80;

    // HEX = FFFFFF80 Decimal = -128
    // Откуда появилось столько F? char - 8 бит всего. А тут 32 бита
    // Ответ - число записывается обратным кодом, поэтому F в начале
    // 32 бита - потому что использовал формат вывода целого числа без знака
    // Т.е. в данном случае необходимо смотреть на последний два разряда
    // Причем это именно дополнительный код, а не обратный
    // Обратный для 0x80 - ()
    printf("HEX = %X Decimal = %d\n", a, a);

    a = -0x80;

    // HEX = FFFFFF80 Decimal = -128
    // Отрицательный знак игнорируется
    printf("HEX = %X Decimal = %d\n", a, a);
    printf("Size of char type (byte) = %d\n", sizeof(a));
    return 0;
}