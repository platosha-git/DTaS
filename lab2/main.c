#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#define EXIT -1
#define NAME 20
#define STREET 20
#define MALLOC_STEP 10

struct Student
{
    char name[NAME];
    int gender; // 0 - женщина, 1 - мужчина
    int age;
    int average_score;
    int admission_year;
    int house_type; // 0 - дом, 1 - общежитие

    union
    {
        struct {
            char street[STREET];
            int house_num;
            int appartment_num;
        } home_adress;

        struct {
            int hostel_num;
            int room_num;
        } hostel;

    } adress;
};

int add_student(struct Student *stud);
    void input_string(char *str, int max_len);
    void input_number(char *msg, int *num, int beg, int end);

struct Student *add_student_to_file(FILE *f);

void output_student_memory(struct Student stud);
void output_student_file(FILE *f, struct Student stud);


struct StudentTable
{
    struct Student *ptr_first;
    int size;
    int size_max;
};

int load_table(FILE *f, struct StudentTable *tbl);
int add_to_table(struct StudentTable *tbl, const struct Student *stud);
int save_table(FILE *f, struct StudentTable *tbl);
int remove_from_table(struct StudentTable *tbl, int i);
void clear_table(struct StudentTable *tbl);
void output_table_memory(struct StudentTable *tbl);
int cmp_stud(const void *a, const void *b);
void sort_stud_table(struct StudentTable *tbl);
void qsort_stud_table(struct StudentTable *tbl);

void search(struct StudentTable *tbl);

struct Key
{
        int key;
        int id;
}Key;

struct KeyTable
{
        struct Key* ptr_first;
        int n;
};

int create_key_table(struct StudentTable* arr_stud, struct KeyTable* arr_keys);
void clear_key_table(struct KeyTable* arr_keys);
void sort_key_table_qsort(struct KeyTable* arr_keys);
void sort_key_table_bubble(struct KeyTable* arr_keys);
void print_key_table(const struct KeyTable* arr_keys);
int sort_stud_table_by_key(const struct StudentTable* arr_stud, const struct KeyTable* arr_keys);
int cmp_key(const void *key1, const void *key2);

unsigned long long tick(void);
void table_sorting_time_bubble(struct StudentTable *tbl);
void table_sorting_time_qsort(struct StudentTable *tbl);
void key_sorting_time_bubble(struct StudentTable *tbl);
void key_sorting_time_qsort(struct StudentTable *tbl);
void swap_students(struct Student *a, struct Student *b);
void swap_keys(struct Key *a, struct Key *b);


struct StudentTable stTbl = {NULL, 0, 0};
struct KeyTable keyTbl = {NULL, 0};


int main(void)
{
    int choose = 0;
    int tmp;
    char name[NAME];

    time_t lead_time;

    int ret = 0;

    struct Student tmp_stud;

    while (ret != EXIT) 
    {
        printf("\nExit.......................0\n");
        printf("Sourse table...............1\n");
        printf("Search.....................2\n\n");
        printf("Bubble sorting.............3\n");
        printf("Qsort sorting..............4\n\n");
        printf("Create key table...........5\n");
        printf("Sort key table(bubble).....6\n");
        printf("Sort key table(qsort)......7\n");
        printf("Sort table by key..........8\n");
        printf("Add student................9\n");
        printf("Remove student.............10\n");
        printf("Measure sorting time.......11\n\n");

        FILE *in = fopen("students.txt", "r");
        if (in == NULL)
        {
            printf("Can`t open file\n");
            exit(1);
        }

        if (load_table(in, &stTbl) != 0)
        {
            printf("Loading error.\n");
            exit(1);
        }
        fclose(in);
        
        if (scanf("%d", &choose) == 1)
        {
            if (choose == 0)
            {
                clear_table(&stTbl);
                exit(1);
            }

            else if (choose == 1)
                output_table_memory(&stTbl);

            else if (choose == 2)
                search(&stTbl);

            else if (choose == 3)
            {
        		sort_stud_table(&stTbl);
        		output_table_memory(&stTbl);
            }
            
            else if (choose == 4)
            {
            	qsort_stud_table(&stTbl);
            	output_table_memory(&stTbl);
            }
            else if (choose == 5)
            {
                create_key_table(&stTbl, &keyTbl);
                print_key_table(&keyTbl);            
			}

			else if (choose == 6)
			{
				sort_key_table_bubble(&keyTbl);
				print_key_table(&keyTbl);
			}

			else if (choose == 7)
			{
                sort_key_table_qsort(&keyTbl);
            	print_key_table(&keyTbl);  
            }

            else if (choose == 8)
                sort_stud_table_by_key(&stTbl, &keyTbl);

            else if (choose == 9)
            {
                add_student(&tmp_stud);
                add_to_table(&stTbl, &tmp_stud);
                FILE *out = fopen("students.txt", "w");
                save_table(out, &stTbl);
                fclose(out);
                output_table_memory(&stTbl);
            }

            else if (choose == 10)
            {
                input_number("Input index of student to delete.", &tmp, 1, stTbl.size);
                remove_from_table(&stTbl, tmp);
                output_table_memory(&stTbl);
            }
            
            else if (choose == 11)
            {
                table_sorting_time_bubble(&stTbl);
                table_sorting_time_qsort(&stTbl);
                key_sorting_time_bubble(&stTbl);
                key_sorting_time_qsort(&stTbl);
                printf("size table = %ld\n", sizeof(struct StudentTable));
                printf("size key table = %ld\n", (sizeof(struct KeyTable) + sizeof(struct Key)));
            }
            
            else
                printf("incorrect input.\n");
            clear_table(&stTbl);
        }
        else
        {
            printf("\nIncorrect input.\n");
			exit(1);   
		}
        ret = 0;
    }
    return 0;
}

//INPUT
void input_string(char *str, int max_len)
{
    char symb;
    int end = 0;

    fflush(stdin);
    getchar();
    while (1)
    {
        printf("|");
        for (int i = 0; i < max_len - 2; i++)
                printf("-");
        printf("|\n");

        for (int i = 0; i < max_len; i++)
        {
            if (scanf("%c", &symb) == 1)
            {
                if (symb != '\n')
                    str[i] = symb;
                else
                {
                    str[i] = 0;
                    end = 1;
                    break;
                }
            }
            else
            {
                str[i] = 0;
                end = 1;
                break;
            }
        }

        fflush(stdin);
        if (!end)
            printf("Your input is incorrect: too many characters.\n");
        else if (!str[0])
        {
            printf("The input couldn`t be empty.\n");
            end = 0;
        }
        else
            return;
    }

}

void input_number(char *msg, int *num, int beg, int end)
{
    printf("\n%s\n", msg);
    printf("It should be from %d to %d: \n", beg, end);
    while (scanf("%d", num) == 0 || !(*num <= end && *num >= beg))
    {
        printf("Your input is incorrect.\n");
        fflush(stdin);
    }
}


//TABLE
int load_table(FILE *f, struct StudentTable *tbl)
{
    int size;

    clear_table(tbl);
    if (fscanf(f, "%d\n\n", &size) != 1)
        return -2;

    for (int i = 0; i < size; i++)
    {
        fscanf(f, "\n");
        add_to_table(tbl, add_student_to_file(f));
    }

    return 0;
}

int add_to_table(struct StudentTable *tbl, const struct Student *stud)
{

    if (tbl->size >= tbl->size_max)
    {
        struct Student *ptr_free = tbl->ptr_first;
        struct Student *ptr_tmp = tbl->ptr_first;

        tbl->size_max += MALLOC_STEP;

        struct Student *ptr_new = (struct Student*) malloc(tbl->size_max * sizeof(struct Student));

        if (!ptr_new)
            return -1;

        tbl->ptr_first = ptr_new;

        for (int i = 0; i < tbl->size; i++)
        {
            *ptr_new = *ptr_tmp;
            ptr_tmp++;
            ptr_new++;
        }
        free(ptr_free);
    }

    tbl->size++;
    tbl->ptr_first[tbl->size - 1] = *stud;

    return 0;
}

int remove_from_table(struct StudentTable *tbl, int i)
{
    if (i > 0 && i <= tbl->size)
    {
        tbl->ptr_first[i-1] = tbl->ptr_first[tbl->size-1];
        tbl->size--;
        return 0;
    }
    else
        return -1;
}

int save_table(FILE *f, struct StudentTable *tbl)
{
    struct Student *ptr_cur = tbl->ptr_first;
    fprintf(f, "%d\n\n", tbl->size);

    for (int i = 0; i < tbl->size; i++)
    {
        output_student_file(f, *ptr_cur);
        ptr_cur++;
    }

    return 0;
}

void clear_table(struct StudentTable *tbl)
{
    tbl->size = 0;
    tbl->size_max = 0;

    if (tbl->ptr_first)
    {
        free(tbl->ptr_first);
        tbl->ptr_first = NULL;
    }
}


struct Student *add_student_to_file(FILE *f)
{
    struct Student *stud = (struct Student*)malloc(sizeof(struct Student));
    int flag = 0;
    if (fscanf(f, "%[^\n]\n", stud->name) != 1) flag = -1;
    if (fscanf(f, "%d", &stud->gender) != 1) flag = -1;
    if (fscanf(f, "%d", &stud->age) != 1) flag = -1;
    if (fscanf(f, "%d", &stud->average_score) != 1) flag = -1;
    if (fscanf(f, "%d", &stud->admission_year) != 1) flag = -1;
    if (fscanf(f, "%d", &stud->house_type) != 1) flag = -1;

    if (stud->house_type)
    {

        if (fscanf(f, "%d", &stud->adress.hostel.hostel_num) != 1) flag = -1;
        if (fscanf(f, "%d", &stud->adress.hostel.room_num) != 1) flag = -1;
    }
    else
    {
        if (fscanf(f, "\n%[^\n]\n", stud->adress.home_adress.street) != 1)
            flag = -1;

        if (fscanf(f, "%d", &stud->adress.home_adress.house_num) != 1) flag = -1;
        if (fscanf(f, "%d", &stud->adress.home_adress.appartment_num) != 1) flag = -1;
    }
    (void)flag;
    return stud;
}

void output_table_memory(struct StudentTable *tbl)
{
    struct Student *ptr_cur = tbl->ptr_first;
    printf("\nTable:\n");

    if (!tbl->size)
        puts("Table is empty!");
    else
    {
        printf("_______________\n");
        for (int i = 0; i < tbl->size; i++)
        {
            printf("%d student\n", (i + 1));
            output_student_memory(*ptr_cur);
            printf("_______________\n");
            ptr_cur++;
        }
    }
}

void output_student_memory(struct Student stud)
{
    printf("\nStudent:           %s\n", stud.name);

    printf("Gender:            ");
    if (stud.gender)
        printf("man\n");
    else
        printf("woman\n");

    printf("Age:               %d\n", stud.age);
    printf("Average score:     %d\n", stud.average_score);
    printf("Admission year:    %d\n", stud.admission_year);
 
    printf("House type:        ");
    if (stud.house_type)
    {
        printf("hostel\n");
        printf("Hostel number:     %d\n", stud.adress.hostel.hostel_num);
        printf("Room number:       %d\n", stud.adress.hostel.room_num);
    }
    else
    {
        printf("home\n");
        printf("Street:            %s\n", stud.adress.home_adress.street);
        printf("House number:      %d\n", stud.adress.home_adress.house_num);
        printf("Appartment number: %d\n", stud.adress.home_adress.appartment_num);
    }
}

void output_student_file(FILE *f, struct Student stud)
{
    fprintf(f, "%s\n", stud.name);
    fprintf(f, "%d\n", stud.gender);
    fprintf(f, "%d\n", stud.age);
    fprintf(f, "%d\n", stud.average_score);
    fprintf(f, "%d\n", stud.admission_year);
    fprintf(f, "%d\n", stud.house_type);
    if (stud.house_type)
    {
        fprintf(f, "%d\n", stud.adress.hostel.hostel_num);
        fprintf(f, "%d\n", stud.adress.hostel.room_num);
    }
    else
    {
        fprintf(f, "%s\n", stud.adress.home_adress.street);
        fprintf(f, "%d\n", stud.adress.home_adress.house_num);
        fprintf(f, "%d\n", stud.adress.home_adress.appartment_num);
    }
    fprintf(f, "\n");
}

void sort_stud_table(struct StudentTable *tbl)
{ 
    struct Student *pb = (struct Student *)tbl->ptr_first;
    struct Student *pe = pb + tbl->size;

    for (int i = 0; i < tbl->size; i++)
    {
        for (int j = 0; j < (pe - pb) - 1; j++)
            if (cmp_stud((void*)(pb + j), (void*)(pb + j+1)) >= 0)
                swap_students((pb + j), (pb + j+1));
        pe--;
    }
}

int cmp_stud(const void *a, const void *b)
{
    return ((struct Student*) a)->age - ((struct Student*) b)->age;
}

int add_student(struct Student *stud)
{
    printf("Input name.\nIts length shouldn`t exceed %d characters.\n", NAME - 1);
    input_string(stud->name, NAME);
    input_number("Input gender (0 - woman, 1 - man).", &(stud->gender), 0, 1);
    input_number("Input age.", &(stud->age), 10, 70);
    input_number("Input average score.", &(stud->average_score), 0, 100);
    input_number("Input admission year.", &(stud->admission_year), 1900, 2019);
    input_number("Input house type (0-home, 1-hostel).", &(stud->house_type), 0, 1);

    if (stud->house_type)
    {
        input_number("Input hostel number.", &(stud->adress.hostel.hostel_num), 1, 100);
        input_number("Input room number.", &(stud->adress.hostel.room_num), 1, 100);
    }
    else
    {
        printf("Input street name.\nIts length shouldn`t exceed %d characters.\n", STREET - 1);
        input_string(stud->adress.home_adress.street, STREET);
        input_number("Input house number.", &(stud->adress.home_adress.house_num), 1, 1000);
        input_number("Input appartment number.", &(stud->adress.home_adress.appartment_num), 1, 1000);
    }

    return 0;
}


//KEY TABLE
int create_key_table(struct StudentTable* arr_stud, struct KeyTable* arr_keys)
{
    struct Student *ptr_s;
    struct Key *ptr_key;

    clear_key_table(arr_keys);

    arr_keys->n = arr_stud->size;

    arr_keys->ptr_first = (struct Key*) malloc(sizeof(struct Key) * arr_keys->n);

    if (arr_keys->ptr_first == NULL)
        return -1;

    ptr_s = arr_stud->ptr_first;
    ptr_key = arr_keys->ptr_first;
    for (int i = 0; i < arr_keys->n; i++)
    {
        ptr_key->id = i;
        ptr_key->key = ptr_s->age;
        ptr_key++;
        ptr_s++;
    }
    return 0;
}

void print_key_table(const struct KeyTable* arr_keys)
{
    printf("\nKey table:\n");

    if (arr_keys->n <= 0)
        printf("Key table is empty.\n");
    else
    {
        struct Key* ptr_key = arr_keys->ptr_first;

        printf("_______________\n");
        for (int i = 0; i < arr_keys->n; i++)
        {
            printf("ID:   %d\n", ptr_key->id + 1);
            printf("Key:  %d\n", ptr_key->key);
            printf("_______________\n");

            ptr_key++;
        }
    }
}

void clear_key_table(struct KeyTable* arr_keys)
{
    if (arr_keys->ptr_first != NULL)
    {
        free(arr_keys->ptr_first);
        arr_keys->ptr_first = NULL;
    }
    arr_keys->n = 0;
}

void search(struct StudentTable *tbl)
{
    int flag = 0;
    int year;

    struct Student *ptr = tbl->ptr_first;
    printf("Input year of admission to find students, that live in hostel.\nIt should be from 1900 to 2019:\n");
    scanf("%d", &year);

    if (year < 1900 || year > 2019)
    	printf("\nIncorrect date");

    for (int i = 0; i < tbl->size; i++)
    {
        if (ptr->admission_year == year && ptr->house_type == 1)
        {
            flag++;

            printf("\n%d student\n", (i + 1));
            output_student_memory(*ptr);
            printf("_______________\n");
        }
        ptr++;
    }

    if (!flag)
        printf("\nNothing found.\n");
}

void qsort_stud_table(struct StudentTable *tbl)
{
	qsort(tbl->ptr_first, tbl->size, sizeof(struct Student), cmp_stud);
}

void sort_key_table_qsort(struct KeyTable *arr_keys)
{
    qsort(arr_keys->ptr_first, arr_keys->n, sizeof(struct Key), cmp_key);
}

void sort_key_table_bubble(struct KeyTable* arr_keys)
{
	struct Key *pb = (struct Key*) arr_keys->ptr_first;
    struct Key *pe = pb + arr_keys->n;

    for (int i = 0; i < arr_keys->n; i++)
    {
        for (int j = 0; j < (pe - pb) - 1; j++)
            if (cmp_key((void*)(pb + j), (void*)(pb + j+1)) >= 0)
            {
                swap_keys((pb + j), (pb + j+1));
            }
        pe--;
    }
}

int cmp_key(const void *key1, const void  *key2)
{
    return ((struct Key*) key1)->key - ((struct Key*) key2)->key;
}

int sort_stud_table_by_key(const struct StudentTable* arr_stud, const struct KeyTable* arr_keys)
{
    struct Key* ptr_key = arr_keys->ptr_first;

    if (arr_keys-> n != arr_stud->size)
    {
        printf("Try to create key table.\n");
        return -1;
    }

    printf("\nTable in key order:\n");

    if (arr_stud->size == 0)
        printf("Table is empty.\n");
    else
    {
        printf("_______________\n");
        for (int i = 0; i < arr_keys->n; i++)
        {
            if (ptr_key->id<0 || ptr_key->id>(arr_stud->size - 1))
                return -2;

            printf("\n%d student\n", (i + 1));
            output_student_memory(arr_stud->ptr_first[ptr_key->id]);
            printf("_______________\n");

            ptr_key++;
            }
    }
    return 0;
}

unsigned long long tick(void)
{
    unsigned long long d;
    __asm__ __volatile__ ("rdtsc" : "=A" (d));
    return d;
}

void table_sorting_time_bubble(struct StudentTable *tbl)
{
    unsigned long long tm, tms = 0;

    for (int i = 0; i < 100; i++)
    {
        tm = tick();
        sort_stud_table(tbl);
        tm = tick() - tm;
        tms += tm;
        FILE *in = fopen("students.txt", "r");
        load_table(in, tbl);
        fclose(in);
    }
    printf("Bubble sort result: ");
    printf("%lld\n", tms / 100);
}

void table_sorting_time_qsort(struct StudentTable *tbl)
{
    unsigned long long tm, tms = 0;

    for (int i = 0; i < 100; i++)
    {
        tm = tick();
        qsort_stud_table(&stTbl);
        tm = tick() - tm;
        tms += tm;
        FILE *in = fopen("students.txt", "r");
        load_table(in, tbl);
        fclose(in);
    }
    printf("Qsort result: ");
    printf("%lld\n", tms / 100);
}

void key_sorting_time_bubble(struct StudentTable *tbl)
{
	unsigned long long tm, tms = 0;

    struct KeyTable key_tbl = {NULL, 0};

    for (int i = 0; i < 100; i++)
    {
        create_key_table(tbl, &key_tbl);
        tm = tick();
        sort_key_table_bubble(&key_tbl);
        tm = tick() - tm;
        tms += tm;
    }

    struct Key* ptr_key = key_tbl.ptr_first;
    for (int i = 0; i < 100; i++)
    {
        tm = tick();
        for (int i = 0; i < key_tbl.n; i++)
            ptr_key++;

        tm = tick() - tm;
        tms += tm;
        FILE *in = fopen("students.txt", "r");
        load_table(in, tbl);
        fclose(in);
    }

    clear_key_table(&key_tbl);
    printf("\nBuuble sort by key result: ");
    printf("%lld\n", tms/100);
}

void key_sorting_time_qsort(struct StudentTable *tbl)
{
    unsigned long long tm, tms = 0;

    struct KeyTable key_tbl = {NULL, 0};

    for (int i = 0; i < 100; i++)
    {
        create_key_table(tbl, &key_tbl);
        tm = tick();
        sort_key_table_qsort(&key_tbl);
        tm = tick() - tm;
        tms += tm;
    }

    struct Key* ptr_key = key_tbl.ptr_first;
    for (int i = 0; i < 100; i++)
    {
        tm = tick();
        for (int i = 0; i < key_tbl.n; i++)
            ptr_key++;

        tm = tick() - tm;
        tms += tm;
        FILE *in = fopen("students.txt", "r");
        load_table(in, tbl);
        fclose(in);
    }

    clear_key_table(&key_tbl);
    printf("Qsort by key result: ");
    printf("%lld\n", tms/100);
}


void swap_students(struct Student *a, struct Student *b)
{
    struct Student tmp = *a;
    *a = *b;
    *b = tmp;
}

void swap_keys(struct Key *a, struct Key *b)
{
	struct Key tmp = *a;
    *a = *b;
    *b = tmp;
}