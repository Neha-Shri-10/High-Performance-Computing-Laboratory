#include <stdio.h>
#include <omp.h>

double f(double x) { return x * x; }

int main() {
    double a = 0.0, b = 1.0;
    int n = 10000000; // Large N to highlight timing differences
    double h = (b - a) / n;
    double start, end;
    int i;
    // 1. REDUCTION METHOD
    double integral_red = (f(a) + f(b)) / 2.0;
    start = omp_get_wtime();
    #pragma omp parallel for reduction(+:integral_red)
    for (i = 1; i < n; i++) {
        integral_red += f(a + i * h);
    }
    integral_red *= h;
    end = omp_get_wtime();
    printf("Reduction Time: %f seconds (Result: %f)\n", end - start, integral_red);

    // 2. CRITICAL METHOD
    double integral_crit = (f(a) + f(b)) / 2.0;
    start = omp_get_wtime();
    #pragma omp parallel for
    for (i = 1; i < n; i++) {
        double val = f(a + i * h);
        #pragma omp critical
        {
            integral_crit += val;
        }
    }
    integral_crit *= h;
    end = omp_get_wtime();
    printf("Critical Time:  %f seconds (Result: %f)\n", end - start, integral_crit);

    return 0;
}
