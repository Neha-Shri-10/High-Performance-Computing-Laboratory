#include <stdio.h>
#include <omp.h>

int main() {
    int m, n, k,i , j;
    printf("Enter rows and columns: ");
    scanf("%d %d", &m, &n);

    int a[m][n];
    printf("Enter matrix elements:\n");
    for(i = 0; i < m; i++) {
        for(j = 0; j < n; j++) {
            scanf("%d", &a[i][j]);
        }
    }

    printf("Enter value of k: ");
    scanf("%d", &k);

    int sum = 0;

    #pragma omp parallel for reduction(+:sum)
    for(i = 0; i < m; i++) {
        for(j = 0; j < n; j++) {
            if(a[i][j] % k == 0) {
                sum += a[i][j];
            }
        }
    }

    printf("Sum of elements divisible by %d = %d\n", k, sum);

    return 0;
}
