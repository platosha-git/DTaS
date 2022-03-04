#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "time.h"
#include "mem_free.h"
#include "changes.h"

#define ITER 1000

void time_testing(int n, int m, int fill)
{
    clock_t time_b = 0, time_e = 0;

    int *matr1, *matr2, *matr3;
    int n_z_el1, n_z_el2, n_z_el3;
    int n_z_rows1, n_z_rows2, n_z_rows3;

    int *A1, *A2, *A3;
    int *JA1, *JA2, *JA3;
    struct IA *IA1 = NULL, *IA2 = NULL, *IA3 = NULL, *tmp;

    // Simple addition
    allocate_matrices(&matr1, &matr2, &matr3, n*m);
    generate_matrix(matr1, n, m, fill);
    generate_matrix(matr2, n, m, fill);

    time_b = clock();
    for (int i = 0; i < ITER; i++)
        add_matrices_simple(matr1, matr2, matr3, n, m);
    time_e = clock();
    double res = (double) (time_e - time_b) / ITER;
    
    printf("\nSimple method\n");
    printf("Time = %.4f\n", res);
    printf("Size = %ld\n", sizeof(int) * n * m * 3);

    // Advanced addition
    count_non_zero(matr1, n, m, &n_z_rows1, &n_z_el1);
    A1 = malloc(n_z_el1 * sizeof(int));
    JA1 = malloc(n_z_el1 * sizeof(int));

    if (n_z_rows1)
    {
        IA1 = malloc(sizeof(struct IA));
        tmp = IA1;
        for (int i = 0; i < n_z_rows1 - 1; i++) {
            tmp->next = malloc(sizeof(struct IA));
            tmp = tmp->next;
        }
        tmp->next = NULL;
    }

    convert_matrix(matr1, n, m, A1, JA1, IA1);

    count_non_zero(matr2, n, m, &n_z_rows2, &n_z_el2);
    A2 = malloc(n_z_el2 * sizeof(int));
    JA2 = malloc(n_z_el2 * sizeof(int));

    if (n_z_rows2)
    {
        IA2 = malloc(sizeof(struct IA));
        tmp = IA2;
        for (int i = 0; i < n_z_rows2 - 1; i++) {
            tmp->next = malloc(sizeof(struct IA));
            tmp = tmp->next;
        }
        tmp->next = NULL;
    }

    convert_matrix(matr2, n, m, A2, JA2, IA2);

	A3 = malloc((n_z_el1 + n_z_el2) * sizeof(int));
    JA3 = malloc((n_z_el1 + n_z_el2) * sizeof(int));

    IA3 = malloc(sizeof(struct IA));
    IA3->Nk = -1;
    IA3->i = -1;
    IA3->next = NULL;

    time_b = 0, time_e = 0, res = 0;
    time_b = clock();
    for (int i = 0; i < ITER; i++)
        add_matrices_advanced(A1, JA1, IA1, n_z_el1, A2, JA2, IA2, n_z_el2, A3, JA3, IA3, &n_z_el3);
    time_e = clock();

    count_non_zero(matr3, n, m, &n_z_rows3, &n_z_el3);

    int size1 = sizeof(int)*n_z_el1*2 + sizeof(struct IA)*n_z_rows1;
    int size2 = sizeof(int)*n_z_el2*2 + sizeof(struct IA)*n_z_rows2;
    int size3 = sizeof(int)*n_z_el3*2 + sizeof(struct IA)*n_z_rows3;

    printf("\nAdvanced method\n");
    res = (double) (time_e - time_b) / ITER;
    printf("Time = %.4f\n", res);
    printf("Size = %d\n\n", size1 + size2 + size3);
}