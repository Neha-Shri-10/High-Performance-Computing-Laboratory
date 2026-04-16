#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    int rows, cols, i, j;
    printf("Enter rows and columns: ");
    scanf("%d %d", &rows, &cols);
    int (*matrix)[cols] = (int (*)[cols])malloc(rows * cols * sizeof(int));
    int (*mirror)[cols] = (int (*)[cols])malloc(rows * cols * sizeof(int));

    printf("Enter elements:\n");
    for (i = 0; i < rows; i++)
        for (j = 0; j < cols; j++)
            scanf("%d", &matrix[i][j]);

    #pragma omp parallel for private(j)
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            mirror[i][cols - 1 - j] = matrix[i][j];
        }
    }

    printf("\nMirror Matrix:\n");
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            printf("%d ", mirror[i][j]);
        }
        printf("\n");
    }

    free(matrix);
    free(mirror);
    return 0;
}
