#include <stdio.h>
#include <stdlib.h>

#define FILE_IN "input.txt"
#define GRAPH_SIMPLE "graph.gv"
#define GRAPH_PATH "graph1.gv"

int check_files(FILE *f1);

int** alloc_rows(int n);
void free_rows(int **data, int n);

int scan_from_file(FILE *f, int **matr, int n);
void print_graph_file(FILE *out, int **matr, int n, int *path, int p_l);

int empty(int **matr, int n);
int max_path(int **matr, int n, int *path, int *p_l);
int obhod(int **matr, int n, int num, int *Q, int *q_l, int *M, int *path, int *p_l);
int find_int(int *Q, int n, int i);

int main()
{
    FILE *f_in, *f_out1, *f_out2;
    int n = 0, **Matr, *path, p_l = 0;

    f_in = fopen(FILE_IN, "r");
    f_out1 = fopen(GRAPH_SIMPLE, "w");
    f_out2 = fopen(GRAPH_PATH, "w");

    if(!(check_files(f_in) && check_files(f_out1) && check_files(f_out2)))
    {
        puts("File error.");
        fclose(f_in);
        fclose(f_out1);
        fclose(f_out2);
        return -1;
    }

    if (fscanf(f_in, "%d", &n) != 1)
    {
        printf("Wrong n in file.\n");
        return -2;
    }

    if (n == 0)
    {
        printf("Empty graph.");
        return 0;
    }

    path = calloc(n, sizeof(int));

    Matr = alloc_rows(n);
    if (Matr == NULL)
    {
        printf("Error allocating memory.\n");
        return -1;
    }

    if (scan_from_file(f_in, Matr, n) != 0)
    {
        printf("Wrong values in file.\n");
        free_rows(Matr, n);
        return -1;
    }

    if (empty(Matr, n))
    {
        printf("Can't build graph with no connections.");
        return 0;
    }

    max_path(Matr, n, path, &p_l);

    printf("The longest way:\n");
    for (int i = 0; i < p_l - 1; i++)
        printf("%d -> ", path[i] + 1);
    printf("%d", path[p_l - 1] + 1);
    printf("\n\n");

    print_graph_file(f_out2, Matr, n, path, p_l);
    fclose(f_out2);
    system("dot -Tpng graph1.gv -o ograph.png");

    free_rows(Matr, n);
    free(path);
    fclose(f_in);
    return 0;
}


int check_files(FILE *f1)
{
    if (!f1)
        return 0;
    return 1;
}

int scan_from_file(FILE *f, int **Matr, int n)
{
    for (int i = 0; i < n; i++)
    {
        if (feof(f))
            return -1;
        for (int j = 0; j < n; j++)
            if (fscanf(f, "%d", *(Matr + i) + j) != 1)
                return -2;
    }
    return 0;
}

void print_graph_file(FILE *out, int **Matr, int n, int *path, int p_l)
{
    fprintf(out, "digraph { \n\trankdir=LR; \n");

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (Matr[i][j] == 1)
                fprintf(out, "\t%d -> %d;\n", i+1, j+1);

    for (int i = 0; i < p_l - 1; i++)
        fprintf(out, "\t%d -> %d[color=red, penwidth=1.0];", path[i] + 1, path[i + 1] + 1);

    fprintf(out, "}\n");
}

int empty(int **matr, int n)
{
    int flag = 1;
    for (int i = 0; i < n && flag; i++)
        for (int j = 0; j < n && flag; j++)
            if (matr[i][j])
                flag = 0;

    return flag;
}

int max_path(int **Matr, int n, int *path, int *p_l)
{
    int M[n], Q[n], q_l, *path_t, p_l_t;
    path_t = malloc(n * sizeof(int));
    if (path_t == NULL)
        return -1;

    for (int i = 0; i < n; i++)
    {
        q_l = 0;
        p_l_t = 0; // размер текущего пути
        if (find_int(path_t, p_l_t, i) == 0)
        {
            for (int j = 0; j < n; j++)
                M[j] = 0;
            Q[q_l++] = i;

            obhod(Matr, n, i, Q, &q_l, M, path_t, &p_l_t);
        }

        if (p_l_t > *p_l)
        {
            *p_l = p_l_t;
            for (int i = 0; i < *p_l; i++)
                path[i] = path_t[i];
        }
    }

    free(path_t);
    return 0;
}

int obhod(int **Matr, int n, int num, int *Q, int *q_l, int *M, int *path, int *p_l)
{
    int d, t, t_l = *q_l;

    for (int i = 0; i < n; i++)
    {
        if (Matr[num][i] == 1 && num != i)
        {
            d = M[num] + 1;
            if (d > M[i])
            {
                M[i] = d;
                if (find_int(Q, *q_l, i) == 0)
                {
                    Q[(*q_l)++] = i;
                    obhod(Matr, n, i, Q, q_l, M, path, p_l);
                }
            }
        }
    }

    if (*p_l < *q_l)
    {
        *p_l = *q_l;
        for (int i = 0; i < *p_l; i++)
            path[i] = Q[i];
    }

    (*q_l)--;

    return 0;
}

int find_int(int *Q, int n, int i)
{
    for (int j = 0; j < n; j++)
    {
        if (Q[j] == i)
            return 1;
    }
    return 0;
}

int** alloc_rows(int n)
{
    int **data = calloc(n, sizeof(int*));
    if (!data)
        return NULL;

    for (int i = 0; i < n; i++)
    {
        data[i] = calloc(n, sizeof(int));
        if (!data[i])
        {
            free_rows(data, n);
            return NULL;
        }
    }

    return data;
}

void free_rows(int **data, int n)
{
    for (int i = 0; i < n; i++)
       free(data[i]);

    free(data);
}