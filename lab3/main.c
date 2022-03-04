#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <time.h>
#include <ctype.h>

#include "mem_free.h"
#include "changes.h"
#include "output.h"
#include "time.h"
#include "exit_codes.h"

int main()
{
    srand(time(0));

    int choose;
    int exit_code = SUCCESS;
    int fill = 40;

    int *matr1, *matr2, *matr3;
    int x, y;
    int n_z_el1, n_z_el2, n_z_el3;
    int n_z_rows1, n_z_rows2;

    int *A1, *A2, *A3;
    int *JA1, *JA2, *JA3;
    struct IA *IA1 = NULL, *IA2 = NULL, *IA3 = NULL, *tmp;

    printf("Addition of two integer matrices using vectors A, JA and linked list IA.\n");

    printf("\nInput matrices........0\n");
    printf("Analyses matrices.....1\n");
    scanf("%d", &choose);

    if (!choose)
    {
        printf("\nInput by hand.............0\n");
        printf("Input by random...........1\n\n");
        scanf("%d", &choose);

        printf("\nInput size\n");
        printf("x = ");
        scanf("%d", &x);
        if (x < 1 || x > 1000 || isdigit(x) != 0)
        {
    	   printf("Your input is incorrect.\n");
    	   exit_code = INPUT_ERR;
            return exit_code;
        }

        printf("y = ");
        scanf("%d", &y);
        if (y < 1 || y > 1000 || isdigit(y) != 0)
        {
    	    printf("Your input is incorrect.\n");
    	    exit_code = INPUT_ERR;
            return exit_code;
        }

        exit_code = allocate_matrices(&matr1, &matr2, &matr3, x * y);
        if (exit_code) 
        {
            printf("\nAllocation error.\n");
            exit_code = ALLOC_ERR;
            return exit_code;
        }

        // Creation of two matrices
        if (!choose) 
        {
            printf("\nInput first matrix:\n");
            input_matrix(matr1, x, y);
            getchar();
            printf("\nInput second matrix:\n");
            input_matrix(matr2, x, y);
        } 
        else 
        {
            generate_matrix(matr1, x, y, fill);
            generate_matrix(matr2, x, y, fill);
        }

        // First matrix
        count_non_zero(matr1, x, y, &n_z_rows1, &n_z_el1);
        A1 = malloc(n_z_el1 * sizeof(int));
        JA1 = malloc(n_z_el1 * sizeof(int));

        if (n_z_rows1) 
        {
            IA1 = malloc(sizeof(struct IA));
            tmp = IA1;
            for (int i = 0; i < n_z_rows1 - 1; i++) 
            {   
                tmp->next = malloc(sizeof(struct IA));
                tmp = tmp->next;
            }
            tmp->next = NULL;
        }
        convert_matrix(matr1, x, y, A1, JA1, IA1);

        // Second
        count_non_zero(matr2, x, y, &n_z_rows2, &n_z_el2);

        A2 = malloc(n_z_el2 * sizeof(int));
        JA2 = malloc(n_z_el2 * sizeof(int));

        if (n_z_rows2) 
        {
            IA2 = malloc(sizeof(struct IA));
            tmp = IA2;
            for (int i = 0; i < n_z_rows2 - 1; i++) 
            {
                tmp->next = malloc(sizeof(struct IA));
                tmp = tmp->next;
            }
            tmp->next = NULL;
        }
        convert_matrix(matr2, x, y, A2, JA2, IA2);


        // Addition
        A3 = malloc((n_z_el1 + n_z_el2) * sizeof(int));
        JA3 = malloc((n_z_el1 + n_z_el2) * sizeof(int));

        IA3 = malloc(sizeof(struct IA));
        IA3->Nk = -1;
        IA3->i = -1;
        IA3->next = NULL;

        add_matrices_simple(matr1, matr2, matr3, x, y);
        add_matrices_advanced(A1, JA1, IA1, n_z_el1, A2, JA2, IA2, n_z_el2, A3, JA3, IA3, &n_z_el3);

        if (x <= 10 && y <= 10)
        {
            puts("\nFirst:");
            print_matrix(matr1, x, y);
            print_advanced(A1, JA1, IA1, n_z_el1);

            puts("\nSecond:");
            print_matrix(matr2, x, y);
            print_advanced(A2, JA2, IA2, n_z_el2);

            puts("\nResult:");
            print_matrix(matr3, x, y);
            print_advanced(A3, JA3, IA3, n_z_el3);
        }
        else
        {
            puts("\nFirst:");
            print_advanced(A1, JA1, IA1, n_z_el1);

            puts("\nSecond:");
            print_advanced(A2, JA2, IA2, n_z_el2);

            puts("\nResult:");
            print_advanced(A3, JA3, IA3, n_z_el3);    
        }

        // Free memory
        free_all(matr1, A1, JA1, IA1);
        free_all(matr2, A2, JA2, IA2);
        free_all(matr3, A3, JA3, IA3);
    }
    else
    {
        printf("\nTime output\n");
        printf("Input size:\n");
        printf("x = ");
        int x = 0, y = 0, proc = 0;
        scanf("%d", &x);
        printf("y = ");
        scanf("%d", &y);
        printf("Pocentage = ");
        scanf("%d", &proc);
        time_testing(x, y, proc);
    }
    return 0;
}