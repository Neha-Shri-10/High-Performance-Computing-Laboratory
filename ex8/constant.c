#include <stdio.h>
#include <omp.h>
#define SIZE 1000000
int main()
{
    static int A[SIZE];   
    int i;
    int constant;
    printf("Enter constant value:");
    scanf("%d",&constant);
    for (i = 0; i < SIZE; i++)
    {
        A[i] = i;
    }
    #pragma omp parallel for
    for (i = 0; i < SIZE; i++)
    {
        A[i] = A[i] + constant;
    }
    printf("First 10 elements after adding constant:\n");
    for (i = 0; i < 10; i++)
    {
        printf("%d  ", A[i]);
    }
    printf("\n");
    return 0;
}
