#include <stdio.h>
#define N 10


int main(void)
{
    int arr[N];

    printf("BEFORE\n");
    //initialize array with arithmetic progression
    for (int i = 0; i < N; i++)
    {
        arr[i] = i + 1;
        printf("%d ", arr[i]);
    }

    printf("\nAFTER\n");
    
    int tmp;

    // reverse array
    for (int i = 0; i < N / 2; i++)
    {
        tmp = arr[i];
        arr[i] = arr[N - i - 1];
        arr[N - i - 1] = tmp;
    }

    // print array
    for (int i = 0; i < N; i++)
    {
        printf("%d ", arr[i]);
    }

    printf("\n");

    return 0;
}
