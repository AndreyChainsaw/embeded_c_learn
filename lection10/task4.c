#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define N 3


int main(void)
{
    int arr[N][N];

    int i = 0;
    int j = 0;
    int size = N;
    int value = 1;
    int vector[2] = {0, 0};

    for (int z = 0; z < 2 * N - 1; z++)
    {
        int route = (z % 4); // узнаю направление
        int vector[2];
        double radian = route * 90 * 3.14 / 180; // Перевод угла в радианы

        // i += 1 * vector[1];
        // j += 1 * vector[0];

        for (int k = 0; k < size; k++)
        {
            arr[i][j] = value;

            i += 1 * vector[1];
            j += 1 * vector[0];
            
            value += 1;
            
        }

        if (route % 2 == 0)
        {
            size -= 1;
        }

        // Уберем лишний индекс (i or j - 1)
        i -= 1 * vector[1];
        j -= 1 * vector[0];

        // сместим i или j по позиции
        vector[0] = rint(cos(radian));
        vector[1] = rint(sin(radian));

        i += 1 * vector[1];
        j += 1 * vector[0];

    }



    for (int f = 0; f < N; f++)
    {
        for (int t = 0; t < N; t++)
        {
            printf("%d\t", arr[f][t]);
        }
        printf("\n");
    }
    return 0;
}