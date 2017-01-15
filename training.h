#ifndef TRAINING_H_
#define TRAINING_H_

typedef struct TrainingStruct* Training;

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

#endif