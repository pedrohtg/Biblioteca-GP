#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include "training.h"
//------------------------------------------------------------
//Private section --------------------------------------------
// OPERATION FUNCTIONS
double add(double a, double b);
double sub(double a, double b);
double mux(double a, double b);
double div(double a, double b);
double pot(double a, double b);
double sqr(double a);
// ------------------------
// Auxiliar functions -----
int get_input_position(char in, Training t);
// ------------------------
// ------------------------

struct TrainingStruct{
	int operation[6];//Representa se as operações {+, -, *, /, sqroot(√), exponetial(^)} são validas, respectivamente;
	char* inputs;
	int op_number;
	int in_number;
	double** data;
	int data_size;
};

//Cria um novo objeto para Training;
Training new_training();

//Configura os operadorse validos;
void set_operations(int number, ...);

//Configura as variaveis(inputs)
void set_inputs(int number, ...);

//Inicializa a entidade com os valores do dataset
void initialize_data(char* filename, Training t);

//Determina se tal Representante c, é uma operação ou variavel/constante
int is_operation(char c);

//Determina se tal representante c é uma operação simples(atua sobre uma variavel) ou composta(atua sobre duas variaveis)
//Obs: Assume que c representa uma operação
int is_simple(char c);

//Retorna uma operação aleatória da Entidade
char random_operation(Training t);

//Retorna uma varivel/constante aleatória da Entidade
char random_variable(Training t);

//Retorna o valor de um input em uma amostra/caso do dataset
double input_value(char in, Training t, int sample);

//Retorna o valor da operação simples para o dado input
double simple_value(char op, double input);

//Retorna o valor da operação composta para o dado input
double composite_value(char op, double input1, double input2);

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
int get_input_position(char in, Training t){
	int i,j;
	int size = t->in_number;

	for(i = 0; j = size - 1; i < j; i++, j--){
		if(t->inputs[i] == in) return i;
		if(t->inputs[j] == in) return j;
	}
	//O input é invalido
	return -1;
}
// ------------------------
