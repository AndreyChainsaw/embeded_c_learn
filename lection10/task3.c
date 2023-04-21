#include <stdio.h>
#define N 4 // string
#define M 4 // column


int main(void)
{
    int arr[N][M];


    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
            int tmp = (i + j >= M - 1) ? 1 : 0;
            arr[i][j] = tmp;
        }
    }

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
            printf("%d ", arr[i][j]);
        }
        printf("\n");
    }
    return 0;
}