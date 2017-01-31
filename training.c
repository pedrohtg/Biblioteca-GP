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
int convert_operation_simbol(char c);
int op_pos(int op);
// ------------------------
// ------------------------

struct TrainingStruct{
	int operation[TOTAL_OP_NUMBER];	// Representa se as operações {+, -, *, /, raiz quadrada(√), exponencial(^), modulo(#)} são validas, respectivamente;
	int op_number;		// Numero de operações ativadas. 
	int in_number;		// Numero de inputs; As variaveis são representadas de 1 até in_number.
	double** data;
	int data_size;
};

//Cria um novo objeto para Training;
Training new_training(int input_number){
	Training t = malloc(sizeof(struct TrainingStruct));
	t->in_number = input_number;

	// Default operations{+,-,*,/}
	int i;
	for(i = 0; i < 4; i++) t->operation[i] = 1;
	t->op_number = 4;

	return t;
}

//Configura os operadorse validos;
void set_operations(Training t, int number, ...){
	int i,pos;
	char c;
	va_list v;
	va_start(v,number);

	for(i = 0; i < TOTAL_OP_NUMBER; i++) t->operation[i] = 0;

	for(i = 0; i < number; i++){
		c = va_arg(v,char);
		pos = convert_operation_simbol(c);
		if(t->operation[pos] == 0) t->op_number++;
		t->operation[pos] = 1;
	}

	va_end(v);
}

//Inicializa a entidade com os valores do dataset
void initialize_data(char* filename, Training t);

//Determina se tal Representante k, é uma operação ou variavel/constante
int is_operation(int k);

//Determina se tal representante k é uma operação simples(atua sobre uma variavel) ou composta(atua sobre duas variaveis)
//Obs: Assume que k representa uma operação
int is_simple(int k){
	if(k == -5 || k == -7) return 1;
	return 0;
}

//Retorna uma operação aleatória da Entidade
int random_operation(Training t){
	srand(time(NULL));

	int op = rand() % TOTAL_OP_NUMBER;
	while(!t->operation[op]) op = rand() % TOTAL_OP_NUMBER;

	return ((-1)*op - 1);
}

//Retorna uma varivel/constante aleatória da Entidade
int random_variable(Training t){
	srand(time(NULL));

	double r = rand()/(1.0 + RAND_MAX); 
    return (r * t->in_number) + 1;
}

//Retorna o valor de um input em uma amostra/caso do dataset
double input_value(int in, Training t, int sample);

//Retorna o valor da operação simples para o dado input
double simple_value(int op, double input);

//Retorna o valor da operação composta para o dado input
double composite_value(int op, double input1, double input2);

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

double exp(double a, double b){
	return pow(a,b);
}

double sqr(double a){
	return sqrt(a);
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
			return '√';
			break;
		case -6: 
			return '^';
			break;
		case -7: 
			return '#';
			break;
		default :
			return '.';
	}
}

int op_pos(int op){
	return (-1*op - 1);
}

// ------------------------
