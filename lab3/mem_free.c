#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "changes.h"

#define MAX_SIZE 1000000

int allocate_matrices(int **matr1, int **matr2, int **matr3, int size)
{
    if (size < 0 || size > MAX_SIZE)
        return -1;

    *matr1 = calloc(size, sizeof(int));
    *matr2 = calloc(size, sizeof(int));
    *matr3 = calloc(size, sizeof(int));

    return 0;
}

void free_list(struct IA *head)
{
    struct IA *next;

    for (; head; head = next)
    {
        next = head->next;
        free(head);
    }
}

void free_all(int *matr, int *A, int *JA, struct IA *IA)
{
    free(matr);
    free(A);
    free(JA);
    free_list(IA);
}