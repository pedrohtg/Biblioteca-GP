#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <stdarg.h>
#include "training.h"
//------------------------------------------------------------
//Private section --------------------------------------------
// OPERATION FUNCTIONS
#define TOTAL_OP_NUMBER 7

double add(double a, double b);
double sub(double a, double b);
double mux(double a, double b);
double div(double a, double b);
double pot(double a, double b);
double sqr(double a);
double mod(double a);
// ------------------------
// Auxiliar functions -----
//int get_input_position(char in, Training t);
int convert_simbol_operation(char c);
double (*fun)(double) op_func_simple(int op);
double (*fun)(double, double) op_func_composite(int op);
int op_pos(int op);
// ------------------------
// ------------------------

struct TrainingStruct{
	int operation[TOTAL_OP_NUMBER];	// Representa se as operações {+, -, *, /,  [a^b]exponencial(^), square_root(√) modulo(#)} são validas, respectivamente;
	int op_number;		// Numero de operações ativadas. 
	int in_number;		// Numero de inputs; As variaveis são representadas de 1 até in_number.
	double** data;		// Matriz que armazena os valores das instancias de treinamento
	int data_size;		// Numero de instancias de treinameto
};

//Cria um novo objeto para Training;
Training new_training(int input_number){
	Training t = malloc(sizeof(struct TrainingStruct));
	t->in_number = input_number;

	// Default operations{+,-,*,/}
	int i;
	for(i = 0; i < 4; i++) t->operation[i] = 1;
	t->op_number = 4;
	t->data_size = 0;

	return t;
}

//Apaga um objeto de Training;
void delete_training(Training t){
	int i;
	for(i = 0; i < t->data_size; i++){
		free(t->data[i]);
	}
	free(t->data);
	free(t);
}

//Configura os operadores validos;
void set_operations(Training t, int number, ...){
	int i,pos;
	char c;

	if(number <= 0){
		printf("Error: Not a valid number of operations. The default setting will be used.\n");
		return;
	}

	va_list v;
	va_start(v,number);

	t->op_number = 0;
	for(i = 0; i < TOTAL_OP_NUMBER; i++) t->operation[i] = 0;

	for(i = 0; i < number; i++){
		c = va_arg(v,char);
		pos = convert_simbol_operation(c);
		if(pos >= 0 && pos < TOTAL_OP_NUMBER){
			if(t->operation[pos] == 0) t->op_number++;
			t->operation[pos] = 1;
		}
	}

	va_end(v);
}

//Inicializa a entidade com os valores do dataset
void initialize_data(char* filename, Training t){
	FILE *fp = fopen(filename, "r");

	if(fp == NULL){
		printf("Error: File couldn't be open. Check if the name typed is correct.\n");
		//printf("Erro: O arquivo não pode ser aberto. Verifique se o nome digitado está correto.\n");
		return;
	}

	int n, m;
	fscanf(fp, "%d %d",n,m);
	if(n != t->in_number){
		printf("Error: Incompatible number of variables in file and configured in the program.\n");
		//printf("Erro: O número de váriaveis do arquivo é diferente do configurado no programa.\n");
		return;
	}
	if(n <= 0 || m <= 0){
		printf("Error: Insufficient number of variables and/or instances.\n");
		return;
	}

	int i,j;

	t->data_size = m;
	t->data = malloc(m*sizeof(double*));

	for(i = 0; i < m; i++){
		t->data[i] = malloc((n+1)*sizeof(double));
		for(j = 0; j < n+1; j++){
			fscanf(fp,"%lf", t->data[i][j]);
		}
	}

	fclose(fp);
}

//Checa se o objeto foi inicializado corretamente
int initialized(Training t){
	return t->data_size > 0;
}

//Determina se tal Representante k, é uma operação ou variavel/constante
//OBS.: Se k é uma operação então assume-se que é válida
int is_operation(int k){
	if(-7 <= k && k <= - 1) return 1;
	else return 0;
}

//Determina se tal representante k é uma operação simples(atua sobre uma variavel) ou composta(atua sobre duas variaveis)
//Obs: Assume que k representa uma operação
int is_simple(int k){
	if(k == -6 || k == -7) return 1;
	return 0;
}

//Retorna uma operação aleatória da Entidade
int random_operation(Training t){
	srand(time(NULL));

	int op = rand() % TOTAL_OP_NUMBER;
	while(!t->operation[op]) op = rand() % TOTAL_OP_NUMBER;

	return ((-1)*op - 1);
}

//Retorna uma operação SIMPLES aleatória da Entidade
int random_simple_operation(Training t){
	srand(time(NULL));
	int coin = rand()%2;
	int sq = convert_simbol_operation('√');
	int md = convert_simbol_operation('#');
	if(t->operation[sq] && coin) return ((-1)*sq - 1);
	else if(t->operation[md]) return ((-1)*md - 1);
	else if(t->operation[sq]) return ((-1)*sq - 1);
	else{
		printf("Error: No simple operation set in this Training.\n");
		return random_operation(t);
	}
}

//Retorna uma operação COMPOSTA aleatória da Entidade
int random_composite_operation(Training t){
	srand(time(NULL));
	int sum = 0;
	int i;
	int pt = convert_simbol_operation('^');
	for(i = 0; i < pt; i++) sum += t->operation[i];

	if(sum  == 0){
		printf("Error: No composite operation set in this Training.\n");
		return random_operation(t);
	}
	else{
		int op = rand() % pt;
		while(!t->operation[op]) op = rand() % pt;

		return op;
	}
}

//Retorna uma varivel/constante aleatória da Entidade
int random_variable(Training t){
	srand(time(NULL));

	double r = rand()/(1.0 + RAND_MAX); 
    return (r * (t->in_number + 1)) + 1;
}

//Retorna o valor de um input em uma amostra/caso do dataset
//Assume que in é uma variável válida
double input_value(int in, Training t, int sample){
	return t->data[sample][in-1];
}

//Retorna o valor de output de uma amostra/caso do dataset
double output_value(Training t, int sample){
	return t->data[sample][t->in_number];
}

//Retorna o valor da operação simples para o dado input
double simple_value(int op, double input){
	double (*func)(double) = op_func_simple(op);
	return func(input);
}

//Retorna o valor da operação composta para o dado input
double composite_value(int op, double input1, double input2){
	double (*func)(double,double) = op_func_composite(op);
	return func(input1,input2);
}

//------------------------------------------------------------
//Private section --------------------------------------------

// OPERATION FUNCTIONS
double add(double a, double b){
	return a+b;
}

double sub(double a, double b){
	return a-b;
}

double mux(double a, double b){
	return a*b;
}

double div(double a, double b){
	if(b == 0) return DBL_MAX;
	return a/b;
}

double pot(double a, double b){
	return pow(a,b);
}

double sqr(double a){
	return sqrt(a);
}

double mod(double a){
	return fabs(a);
}
// ------------------------

// Auxiliar functions -----
// int get_input_position(char in, Training t){
// 	int i,j;
// 	int size = t->in_number;

// 	for(i = 0; j = size - 1; i < j; i++, j--){
// 		if(t->inputs[i] == in) return i;
// 		if(t->inputs[j] == in) return j;
// 	}
// 	//O input é invalido
// 	return -1;
// }

char convert_operation_simbol(int op){
	switch(op){
		case -1: 
			return '+';
			break;
		case -2: 
			return '-';
			break;
		case -3: 
			return '*';
			break;
		case -4: 
			return '/';
			break;
		case -5: 
			return '^';
			break;
		case -6: 
			return '√';
			break;
		case -7: 
			return '#';
			break;
		default :
			return '.';
	}
}

int convert_simbol_operation(char op){
	switch(op){
		case '+': 
			return 0;
			break;
		case '-': 
			return 1;
			break;
		case '*': 
			return 2;
			break;
		case '/': 
			return 3;
			break;
		case '^': 
			return 4;
			break;
		case '√': 
			return 5;
			break;
		case '#': 
			return 6;
			break;
		default :
			return -1;
	}
}

double (*fun)(double) op_func_simple(int op){
	switch(op){
		case -6: 
			return &sqr;
			break;
		case -7: 
			return &mod;
			break;
	}
}

double (*fun)(double, double) op_func_composite(int op){
	switch(op){
		case -1: 
			return &add;
			break;
		case -2: 
			return &sub;
			break;
		case -3: 
			return &mux;
			break;
		case -4: 
			return &div;
			break;
		case -5: 
			return &pot;
			break;
	}
}

int op_pos(int op){
	return (-1*op - 1);
}

// ------------------------
