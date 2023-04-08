#include <stdio.h>
#include <stdlib.h>


// Перевод адреса из HEX в стандартное представление для IPv4
void printHexAddressToDecimal(unsigned int);

// Возвращение массива из IP-адреса сети и IP-адреса хоста в HEX для IPv4
unsigned int *getNetworkAddress(unsigned int, unsigned int);


int main(void)
{
    //Адрес 192.168.1.10
    unsigned int ipAddress = 0xC0A8010A;

    //Маска 255.255.255.0
    unsigned int wildcardMask = 0xFFFFFF00;

    unsigned int *addresses = getNetworkAddress(ipAddress, wildcardMask);
    
    printf("IP address ");
    printHexAddressToDecimal(ipAddress);

    printf("Wildcard mask ");
    printHexAddressToDecimal(wildcardMask);
    
    printf("Network Address ");
    printHexAddressToDecimal(addresses[0]);

    printf("Host address ");
    printHexAddressToDecimal(addresses[1]);

    return 0;
}

unsigned int *getNetworkAddress(unsigned int address, unsigned int mask)
{
    // Инициализирую массив в куче, чтобы вернуть указатель и работать с массивом дальше вне функции
    unsigned int *addresses = (unsigned int*) malloc(sizeof(unsigned int) * 2);

    // Адрес сети
    addresses[0] = address & mask;

    // Адрес хоста
    addresses[1] = addresses[0] ^ address;

    return addresses;
}

void printHexAddressToDecimal(unsigned int address)
{
    unsigned int mask = 0x000000FF;
    char numberOktetsIPv4 = 4;

    unsigned char oktet;

    printf("(reverse format) = ");
    for (int i=0; i < numberOktetsIPv4; i++)
    {
        oktet = address & mask;
        printf("%u.", oktet);
        address = address >> 8;
    }
    printf("\n");
}
