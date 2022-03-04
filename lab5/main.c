#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "structures.h"
#include "list_queue.h"
#include "array_queue.h"

#define n 1000
#define interval 100

int main()
{
    srand(time(NULL));

    time_range T1 = {1, 5};
    time_range T2 = {0, 3};
    time_range T3 = {0, 4};
    time_range T4 = {0, 1};

    printf("Two queues\n");
    printf("The number of requests of the first type %d\n", n);
    printf("Intermediate result after %d requests\n", interval);

    printf("T1 = {%.0f; %.0f}\n", T1.min, T1.max);
    printf("T2 = {%.0f; %.0f}\n", T2.min, T2.max); 
    printf("T3 = {%.0f; %.0f}\n", T3.min, T3.max);
    printf("T4 = {%.0f; %.0f}\n", T4.min, T4.max);

    int choose = -1;
    while (choose)
    {
        printf("\nExit..............0\n");
        printf("Array queue.......1\n");
        printf("List queue........2\n");

        if (scanf("%d", &choose) != 1)
        {
            printf("Wrong input!\n");
            break;
        }

        if (choose == 1)
            impliment_array(n, interval, T1, T2, T3, T4);

        if (choose == 2)
            impliment_list(n, interval, T1, T2, T3, T4);
    }

    return 0;
}