#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "analyse.h"
#include "tree.h"
#include "hash.h"
#include "constants.h"

#define NUM_COLLIS 1

void menu(void);
int check_int(int *number, int min, int max);
char *input_str(int *error);

int main(void)
{
	printf("Tree and hash table from file.\n");

	int error = SUCCESS;
	int done;
	int flag;

	node_t *root = NULL;
	node_t *bal_root = NULL;
	node_hash **table = NULL;
	int len_table = 0;

	unsigned long int time_b, time_e;
	unsigned long int time_delete_tree;
	unsigned long int mem_delete_tree;
	unsigned long int time_delete_bal_tree;
	unsigned long int mem_delete_bal_tree;
	unsigned long int time_delete_hash;
	unsigned long int mem_delete_hash;
	unsigned long int memory;

	char *del_word = NULL;
	FILE *f = fopen(DATA_NAME, "r");

	if (!f)
	{
		printf("Can not open file with data.");
		return -4;
	}

	root = create_tree(root, f);
	rewind(f);
	bal_root = create_bal_tree(bal_root, f);
	fclose(f);

	int choose = -1;
	while (choose)
	{
		printf("\nExit.................0\n");
		printf("Tree.................1\n");
		printf("Hash table...........2\n");
		printf("Analyse..............3\n\n");

		if (scanf("%d", &choose) != 1)
		{
			printf("Wrong input.\n");
			continue;
		}

		if (choose == 1)
		{
			while (choose)
			{
				printf("\tExit.................0\n");
				printf("\tCreate tree..........1\n");
				printf("\tBalance tree.........2\n");
				printf("\tDelete word..........3\n\n");

				scanf("%d", &choose);
				if (choose == 1)
				{
					f = fopen(TREE_FILE_NAME, "w");
					export_to_dot(f, "test_tree", root, 1);
					fclose(f);
					continue;
				}

				if (choose == 2)
				{
					f = fopen(BAL_TREE_FILE_NAME, "w");
					export_to_dot(f, "test_tree", bal_root, 0);
					fclose(f);
					continue;
				}

				if (choose == 3)
				{
					flag = 0;
					del_word = input_str(&error);
					if (error)
					{
						printf("String input is incoorect.\n");
						return -1;
					}
				
					done = SUCCESS;
					time_b = tick();
					root = remove_in_tree(del_word, root, &done, &flag);
					time_e = tick();
					if (!flag)
						printf("There is no such word in the tree.\n");
					else
						printf("Word was deleted.\n\n");

					int count_node = 0;
                    count_node_tree(bal_root, &count_node);
                    memory = count_node*((sizeof(char*))*3);

					int remove_from = -1;
					while (remove_from != 0)
					{
						printf("\tExit................0\n");
						printf("\tTree................1\n");
						printf("\tBalanced tree.......2\n");
						printf("\tAnalyse.............3\n");

						scanf("%d", &remove_from);
						if (remove_from == 1)
						{
							f = fopen(TREE_FILE_NAME, "w");
                    		export_to_dot(f, "test_tree", root, 0);
                    		fclose(f);
                    		time_delete_tree = time_e - time_b;
						}

						if (remove_from == 2)
						{
							time_b = 0;
							time_e = 0;
							done = SUCCESS;
							flag = 0;
							time_b = tick();
							bal_root = remove_t(bal_root, del_word, &done, &flag);
							time_e = tick();

							time_delete_bal_tree = time_e - time_b;

							f = fopen(BAL_TREE_FILE_NAME, "w");
                    		export_to_dot(f, "test_tree", bal_root, 0);
                    		fclose(f);
                    	}

                    	if (remove_from == 3)
                    	{
                    		printf("\n\nBinary search tree:\n");
                    		printf("\tTime to delete     %lu ticks\n", time_delete_bal_tree);
                    		printf("\tEmployed memory    %lu bytes\n", memory);

                    		printf("AVL tree:\n");
                    		printf("\tTime to delete     %lu ticks\n", time_delete_tree);
                    		printf("\tEmployed memory    %ld bytes\n\n\n", memory + sizeof(int));
                    	}
                    }
				}
			}
			choose = 0;
		}

		if (choose == 2)
		{
			while (choose)
			{
				printf("\tExit..................0\n");
				printf("\tCreate table..........1\n");
				printf("\tDelete word...........2\n");

				scanf("%d", &choose);
				if (choose == 1)
				{
					printf("Input size of the table.\n");
					printf("It should be integer from 0 to 100.\n");
					int error = check_int(&len_table, 0, 1000);
					if (!error)
						table = create_hash_table(DATA_NAME, &len_table, NUM_COLLIS);
					if (!table)
						printf("Table is empty.\n");
					else
					{
						printf("\nHash table: \n");
						print_table(table, len_table);
					}
					continue;
				}

				if (choose == 2)
				{
					if (!table)
						printf("Table is empty.\n");
					else
					{
						del_word = input_str(&error);
						if (error)
						{
							printf("String input is incoorect.\n");
							return -1;
						}

						done = SUCCESS;
						time_b = tick();
						remove_from_hash_table(del_word, table, len_table, &done);
						time_e = tick();

						time_delete_hash = time_e - time_b;

						if (!done)
							printf("There is no such word in the tree.\n");
						else
						{
							printf("Word was deleted.\n\n");
							printf("\nHash table: \n");
							print_table(table, len_table);

                    		printf("\n\nHash table:\n");
                    		printf("\tTime to delete %ld ticks\n\n", time_delete_hash);
						}
					}
				} 
			}
		}

		if (choose == 3)
		{
			efficiency("data100.txt", 30, 3);	
			return 0;			
		}
	}
	return SUCCESS;
}

int check_int(int *number, int min, int max)
{
	int rc, num, error;
	char tmp;
	rc = scanf("%d%c", &num, &tmp);
	if (rc == 2 && (tmp == '\n' || tmp == ' '))
	{
		if ((num > min) && (num < max))
		{
			*number = num;
			error = SUCCESS;
		}
		else
			error = ERROR_VALUE;

		if (!min && !max && error == ERROR_VALUE)
		{
			*number = num;
			error = SUCCESS;
		}
		return error;
	}
	else if (tmp != '\n')
	{
		while(tmp != '\n')
			scanf("%c", &tmp);
		return ERROR_INPUT;
	}
	else
		return ERROR_INPUT;

	return SUCCESS;
}


char *input_str(int *error)
{
	*error = SUCCESS;
	char *str = malloc(100 * sizeof(char));
	printf("Input word:\n");
	fflush(stdin);
	int i = 0;
	getchar();
	scanf("%c", &str[i]);
	while (str[i] != '\n')
	{
		i++;
		scanf("%c", &str[i]);
	}
	str[i + 1] = '\0';

	check_end(str);
	if (!strlen(str))
		*error = -1;
	return str;
}