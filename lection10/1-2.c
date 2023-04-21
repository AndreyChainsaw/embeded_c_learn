#include <stdio.h>
#define N 3


int main(void)
{
    // Перебор матрицы с одним индексом
    int squareMatrix[N*N];

    for (int i = 0; i < N * N; i++)
    {
        squareMatrix[i] = i + 1;
    }


    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            printf("%d\t", squareMatrix[i * N + j]);
        }
        printf("\n");
    }

    return 0;
}