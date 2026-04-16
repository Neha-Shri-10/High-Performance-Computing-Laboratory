#include <stdio.h>
#include <time.h>

int main()
{
    int r1, c1, r2, c2;
    int a[50][50], b[50][50], c[50][50];
    int i, j, k;

    // Using struct timespec for nanosecond precision
    struct timespec start, end;
    double time_ms;

    printf("Enter rows and columns of first matrix: ");
    scanf("%d %d", &r1, &c1);
    printf("Enter rows and columns of second matrix: ");
    scanf("%d %d", &r2, &c2);

    if (c1 != r2)
    {
        printf("Matrix multiplication not possible\n");
        return 0;
    }

    printf("Enter elements of first matrix:\n");
    for (i = 0; i < r1; i++)
        for (j = 0; j < c1; j++)
            scanf("%d", &a[i][j]);

    printf("Enter elements of second matrix:\n");
    for (i = 0; i < r2; i++)
        for (j = 0; j < c2; j++)
            scanf("%d", &b[i][j]);

    // Start high-resolution timer
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (i = 0; i < r1; i++)
    {
        for (j = 0; j < c2; j++)
        {
            c[i][j] = 0;
            for (k = 0; k < c1; k++)
            {
                c[i][j] = c[i][j] + a[i][k] * b[k][j];
            }
        }
    }

    // End high-resolution timer
    clock_gettime(CLOCK_MONOTONIC, &end);

    // Display the Result Matrix
    printf("\nResultant Matrix:\n");
    for (i = 0; i < r1; i++)
    {
        for (j = 0; j < c2; j++)
        {
            printf("%d\t", c[i][j]);
        }
        printf("\n");
    }

    // Calculate time difference in milliseconds
    time_ms = (end.tv_sec - start.tv_sec) * 1000.0 +
              (end.tv_nsec - start.tv_nsec) / 1000000.0;

    printf("\nTime taken for matrix multiplication: %.6f milliseconds\n", time_ms);

    return 0;
}
