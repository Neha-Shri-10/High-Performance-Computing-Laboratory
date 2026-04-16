#include <stdio.h>
#include <omp.h>
#define SIZE 1000000  
int main()
{
    static int A[SIZE];
    static int B[SIZE];
    static int C[SIZE];
    int i;
    #pragma omp parallel for
    for (i = 0; i < SIZE; i++)
    {
        A[i] = i;
        B[i] = i * 2;
    }
    #pragma omp parallel for
    for (i = 0; i < SIZE; i++)
    {
        C[i] = A[i] + B[i];
    }
    printf("First 10 elements of result array:\n");
    for (i = 0; i < 15; i++)
    {
        printf("%d + %d = %d\n", A[i], B[i], C[i]);
    }
    
    return 0;
}
