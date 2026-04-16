#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <limits.h>

int main() {
    int size,i;
    printf("Enter the size of the array: ");
    if (scanf("%d", &size) != 1 || size <= 0) {
        printf("Invalid input for array size. Exiting.\n");
        return 1;
    }

    int *input_array = (int *)malloc(size * sizeof(int));
    int *running_min_array = (int *)malloc(size * sizeof(int));

    if (input_array == NULL || running_min_array == NULL) {
        printf("Memory allocation failed. Exiting.\n");
        return 1;
    }
    printf("Enter %d elements for the array:\n", size);
    for (i = 0; i < size; i++) {
        if (scanf("%d", &input_array[i]) != 1) {
            printf("Invalid input. Exiting.\n");
            free(input_array);
            free(running_min_array);
            return 1;
        }
    }
    if (size > 0) {
        running_min_array[0] = input_array[0];
        for (i = 1; i < size; i++) {
            if (input_array[i] < running_min_array[i-1]) {
                running_min_array[i] = input_array[i];
            } else {
                running_min_array[i] = running_min_array[i-1];
            }
        }
    }
    printf("\nRunning minimum array:\n");
    #pragma omp parallel for
    for (i = 0; i < size; i++) {
        #pragma omp critical
        {
            printf("Index %d: %d\n", i, running_min_array[i]);
        }
    }
    free(input_array);
    free(running_min_array);

    return 0;
}
