#include <stdio.h>
#define N 4


int main(void)
{
    int squareMatrix[N][N];

    // initialize matrix
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            squareMatrix[i][j] = (i * N) + j + 1;
        }
    }


    // print matrix for validation
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            printf("%d\t", squareMatrix[i][j]);
        }
        printf("\n");
    }

    return 0;
}