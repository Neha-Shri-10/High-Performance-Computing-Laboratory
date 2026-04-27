#include <stdio.h>
#include <omp.h>

int main() {
    int m, n;
    printf("Enter rows and columns: ");
    scanf("%d %d", &m, &n);

    int a[m][n];
    int i,j,k;
    printf("Enter matrix elements:\n");
    for(i = 0; i < m; i++) {
        for(j = 0; j < n; j++) {
            scanf("%d", &a[i][j]);
        }
    }

    int count = 0;

    #pragma omp parallel for reduction(+:count)
    for(i = 0; i < m; i++) {
        for(j = 0; j < n; j++) {

            int isMinRow = 1;
            int isMaxCol = 1;

            // Check minimum in row
            for(k = 0; k < n; k++) {
                if(a[i][k] < a[i][j]) {
                    isMinRow = 0;
                    break;
                }
            }

            // Check maximum in column
            for(k = 0; k < m; k++) {
                if(a[k][j] > a[i][j]) {
                    isMaxCol = 0;
                    break;
                }
            }

            if(isMinRow && isMaxCol) {
                count++;
            }
        }
    }

    printf("Number of saddle points = %d\n", count);

    return 0;
}
