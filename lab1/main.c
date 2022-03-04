#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <math.h>

void number_input(char *elem, int size);
void empty_input_err(char *elem);
void zero_input_err(char *elem);
void input_err1(char *elem);	
void input_err2(char *elem);
int search_m(int sign_fl, char *elem);
int search_n(int m, int sign_fl, char *elem);
int search_k(int m, int n, int sign_fl, char *elem);
int search_dgr(int sign_fl, int m, int n, char *elem);
void search_mantissa(int *mantissa, int sign_fl, int m, int n, char *elem);
void long_multiplication(int *arg1, int *arg2, int *result);
void rounding(int *res);
void result_output(int *res, int sign1, int sign2, int degree, int n);

int main(void)
{
	printf("Multiplication of two numbers, the first of which is real, and the second is an integer.\n");
	printf("Enter the first number.\nIt must be valid.\nThe input must be in the format:\n+-m.n E +-k, where\nm + n <= 30, k <= 5\n");
	printf("1        10        20       30 E 1   5\n");
    printf("|--------|---------|---------|---|---|\n");
	char elem1[42];
	number_input(elem1, 42);

	empty_input_err(elem1);	
	input_err1(elem1);
	zero_input_err(elem1); 
	 
//Поиск знака 1го числа
	int sign1 = 1;                                      
	int sign_fl1 = 0;
	if (elem1[0] == '+') {
		sign1 = 1;
		sign_fl1 = 1;
	}
	if (elem1[0] == '-') {
		sign1 = -1;
		sign_fl1 = 1;
	}
//	printf("sign1=%d\n", sign1);

	int m1 = search_m(sign_fl1, elem1);
//	printf("m1=%d\n", m1);

	int n = search_n(m1, sign_fl1, elem1);
//	printf("n=%d\n", n);
	
	if (m1 + n > 30) {
		printf("Your input is incorrect.\nThe mantissa of the first number is too long.\n");
		exit(1);
	}

	if (m1 == 0 && n == 0) {
		printf("Your input is incorrect.\nThe mantissa of the first number is empty.\n");
		exit(1);
	}

	int k1 = search_k(m1, n, sign_fl1, elem1) - 1;
//	printf("k1=%d\n", k1);

	if (k1 > 5) {
		printf("Your input is incorrect.\nThe degree of the first number is too big.\n");
		exit(1);		
	}

//Поиск степени 1го числа
	int degree1 = 0;
	if (k1 == 0) {
		degree1 = 0;
	} 
	else {
		degree1 = search_dgr(m1, n, sign_fl1, elem1);
		if (degree1 > 99999 || degree1 < -99999) {
			printf("Your input is incorrect.\n");
			exit(1);
		}
//		printf("degree1=%d\n", degree1);
	}

	if (elem1[sign_fl1 + m1 + n + 5] == '+' || elem1[sign_fl1 + m1 + n + 5] == '-') {
		printf("Your input is incorrect1.\n");
		exit(1);
	}

	int normalize = 0;
	for (int i = 0; i <= sign_fl1 + m1; i++) {
		if (elem1[i] == '0')
			normalize++;
	}
	if (normalize == m1) {
		printf("\nNormalized form of the first number:\n");
		printf("0.");
		int i = sign_fl1 + m1 + 1;
		while (elem1[i] == '0')
			i++;
		int j = i;
		while (elem1[j] != '\n' && elem1[j] != 'E') {
			printf("%c", elem1[j]);
			j++;
		}
		printf(" E ");
		printf("%d\n", (i - sign_fl1 - m1 - 1) * (-1) + degree1);
	}

	printf("\n");
	printf("Enter the second number.\nIt must be integer.\nIt`s length mustn`t exceed 30 decimal digits.\n");
	printf("1        10        20       30\n");
    printf("|--------|---------|---------|\n");
	char elem2[32];   
	number_input(elem2, 32);

	empty_input_err(elem2);
	input_err2(elem2);
	zero_input_err(elem2);
	printf("\n");

	
//Поиск знака 2го числа
	int sign2 = 1;                                          
	int sign_fl2 = 0;
	if (elem2[0] == '+') {
		sign2 = 1;
	 	sign_fl2 = 1;
	}
	if (elem2[0] == '-') {
	 	sign2 = -1;
	 	sign_fl2 = 1;
	}
//	printf("sign2=%d\n", sign2);

	int m2 = 0;
	int i = sign_fl2;
	while (elem2[i] != '.' && elem2[i] != 'E' && elem2[i] != 'e' && elem2[i] != ' ' && i <= 30 && elem2[i] != '\n') {
		if (elem2[i] > '9' || elem2[i] < '0') {
			printf("Your input is incorrect5.\n");
			exit(1);
		}
		m2++;
		i++;
	}
//	printf("m2=%d\n", m2);


	if (sign_fl2 == 0) {
		if (elem2[m2 + sign_fl2] != '\n') {
			printf("Your input is incorrect.\nThe second element should be integer.\n");
			exit(1);	
		}
	}

	if (m2 > 30 || m2 == 0) {
		printf("Your input is incorrect.\nThe mantissa of the second number is too long.\n");
		exit(1);
	}

//Поиск мантисы 1го числа
	int mantissa[32] = {0};
	search_mantissa(mantissa, sign_fl1, m1, n, elem1);

/*	for (int i = 0; i < 32; i++)
		printf("%d", mantissa[i]);
	printf("\n");*/

//Перевод 2го числа из char в int
	int digit_elem2[32] = {0};
	for (int i = sign_fl2; i <= m2; i++)
		digit_elem2[32 - m2 + i - sign_fl2] = elem2[i] - '0';
	
/*	for (int i = 0; i < 32; i++)
		printf("%d", digit_elem2[i]);
	printf("\n");*/

//Перемножение двух чисел
	int res[61] = {0};
	long_multiplication(mantissa, digit_elem2, res);

	printf("1        10        20       30\n");
    printf("|--------|---------|---------|\n");	
	
	/*for (int i = 0; i < 61; i++)
    	printf("%d", res[i]);
    printf("\n");*/

//Округление результата в случае переполнения
    rounding(res);

//Вывод результата на экран
	printf("The result is\n");
    result_output(res, sign1, sign2, degree1 - n, n);
    
	return 0;
}


void number_input(char *elem, int size)
{
	fflush(stdin);
	if (fgets(elem, size, stdin) == 0) {
		printf("Your input is incorrect2.\n");
		exit(1);
	}
}

void empty_input_err(char *elem)
{
	int i = 0, err = 0;
	while (elem[i] != '\n') {
		if (elem[i] == ' ')
			err++;
		i++;
	}
	if (err == i) {
		printf("Your input is incorrect.\nNumber input can`t be empty.\n");
		exit(1);
	}
}

void zero_input_err(char *elem)
{
	int i = 0, err = 0, k = 0; 
	while (elem[i] != '\n') {
		if (elem[i] >= '0' && elem[i] <= '9') {
			if (elem[i] == '0')
				err++;
			k++;
		}
		i++;
	}
	if (err == k) {
		printf("The result is\n0\n");
		exit(1);
	}
}

void input_err1(char *elem)
{
	int sign = 0, number = 0, e_count = 0, space = 0, dot = 0;
	int i = 0;
	while (elem[i] != '\n') {
		if (elem[i] == '+' || elem[i] == '-')
			sign++;
		if (elem[i] <= '9' && elem[i] >= '0')
			number++;
		if (elem[i] == 'E' || elem[i] == 'e')
			e_count++;
		if (elem[i] == ' ')
			space++;
		if (elem[i] == '.')
			dot++;
		i++;
	}
	if (sign > 2 || e_count > 1 || dot > 1 || sign + number + e_count + space + dot < strlen(elem) - 1) {
		printf("Your input is incorrect3.\n");
		exit(1);
	}
}

void input_err2(char *elem)
{
	int sign = 0, number = 0, space = 0;
	int i = 0;
	while (elem[i] != '\n' && i != 31) {
		if (elem[i] == '+' || elem[i] == '-')
			sign++;
		if (elem[i] <= '9' && elem[i] >= '0')
			number++;
		if (elem[i] == ' ')
			space++;
		i++;
	}
	if (sign > 1 || sign + number + space < strlen(elem) - 1) {
/*		printf("sign=%d\n", sign);
		printf("number=%d\n", number);
		printf("space=%d\n", space);
		printf("sign + number + space=%d\n", sign + number + space);
		printf("strlen(elem) - 1=%ld\n", strlen(elem) - 1);
		printf("Your input is incorrect4.\n");*/
		exit(1);
	}
}

int search_m(int sign_fl, char *elem)
{
	int m = 0;
	int i = sign_fl;
	while (elem[i] != '.' && elem[i] != 'E' && elem[i] != 'e' && elem[i] != ' ' && elem[i] != '\n') {
//		printf("elem[i]=%c\n", elem[i]);
//		printf("i=%d\n", i);
		if (elem[i] > '9' || elem[i] < '0') {
			printf("Your input is incorrect5.\n");
			exit(1);
		}
		m++;
		i++;
	}
	return m;
}

int search_n(int m, int sign_fl, char *elem)
{
	int n = 0;
	int i = sign_fl + m + 1;
	while (elem[i] <= '9' && elem[i] >= '0') {
		n++;
		i++;
	}
	return n;
}

int search_k(int m, int n, int sign_fl, char *elem)
{
	int k = 0;
	int i = sign_fl + m + n;
	while (elem[i] != '\n') {
		if (elem[i] == '+' || elem[i] == '-' || elem[i] == 'E' || elem[i] == 'e' || elem[i] == ' ') {
			k--;
		}
		k++;
		i++;
	}
	return k;
}

int search_dgr(int m, int n, int sign_fl, char *elem)
{
	int sign_dgr = 1;
	int degree = 0;
	int i = sign_fl + m + n;

	if (n != 0) {
		i++;
	}

//	printf("sign_fl=%d\n", sign_fl);
//	printf("m=%d\n", m);
//	printf("n=%d\n", n);
//	printf("i=%d\n", i);
	while (elem[i] != '\n') {
		if (elem[i] == '+') {
			sign_dgr = 1;
			i++;
		}
		if (elem[i] == '-') {
			sign_dgr = -1;
			i++;
		}
		if (elem[i] == '+' || elem[i] == '-') {
			printf("Your input is incorrect.\nThe first element should be in the format +-m.n E +-k\n");
			exit(1);
		}
		if (elem[i] == '.') {
			printf("Your input is incorrect.\nThe first element should be in the format +-m.n E +-k\n");
			exit(1);	
		}

		if (elem[i] >= '0' && elem[i] <= '9') {
			degree += elem[i] - '0';
			degree *= 10;
		}	
 		i++;
	}
	degree /= 10;
	degree = sign_dgr * degree;
	return degree;
}

void search_mantissa(int *mantissa, int sign_fl, int m, int n, char *elem)
{
	int j = 31;
	for (int i = sign_fl + m + n + 1; i >= 0; i--) {
		if (elem[i] <= '9' && elem[i] >= '0') {
			mantissa[j] += elem[i] - '0';
			j--;
		}
	}
}

void long_multiplication(int *arg1, int *arg2, int *result)
{
	for (int i = 0; i < 32; i++) {
        for (int j = 0; j < 32; j++)
            result[60 - (i + j)] += arg1[31 - j] * arg2[31 - i];
    }

    for (int i = 60; i > 0; i--) {
        if (result[i] > 9) {
        	result[i - 1] +=  result[i] / 10;
        	result[i] -= ((result[i] / 10) * 10);
        }
    }
}

void rounding(int *res)
{
	int i = 0;
	int fl_owf = 0;
    while (res[i] == 0) {
    	i++;
    }
    int begin = i;

    if (61 - begin > 30) {
    	fl_owf = 1;
    }

    i = 61;
    while (res[i] == 0) {
    	i--;
    }
    int end = i;

    if (fl_owf == 1) {
    	int j = 60;
    	//printf("res[j]=%d\n", res[j]);
    	while (j >= begin + 1 && res[j] >= 5) {
    		res[j - 1] += 1;
    		j--;
    	}

		for (int i = 60; i > 0; i--) {
        	if (res[i] > 9) {
        		//res[i - 1] +=  res[i] / 10;
        		res[i] -= ((res[i] / 10) * 10);
        	}
    	}
    }

    /*for (int i = 0; i < 61; i++)
    	printf("%d ", res[i]);
    printf("\n");*/
}

void result_output(int *res, int sign1, int sign2, int degree, int n)
{
	int i = 0;
    while (res[i] == 0) {
    	i++;
    }
    int begin = i;

    i = 60;
    while (res[i] == 0) {
    	i--;
    }
    int end = i;

//    printf("degree=%d\n", degree);
    degree += 61 - begin;
    if (degree > 99999 || degree < -99999) {
    	printf("\nOverflow\n");
    	exit(1);
    }

    if (sign1 == sign2)
    	printf("+");
    else
    	printf("-");
    printf("0.");
    
    int min = (begin + 30 > end) ? end : begin + 29;
    for (int j = begin; j <= min; j++) {
    	printf("%d", res[j]);
    }
    printf(" E ");

    if (degree > 0)
    	printf("+");
    printf("%d\n", degree);
}