#ifndef TRAINING_H_
#define TRAINING_H_

typedef struct TrainingStruct* Training;

//Cria um novo objeto para Training;
Training new_training(int input_number);

//Apaga o objeto de Training
void delete_training(Training t);

//Configura os operadorse validos;
void set_operations(Training t, int number, ...);

//Inicializa a entidade com os valores do dataset
//O arquivo deve seguir um padrão:
// => A 1ª linha do arquivo contem 2 inteiros, n e m, onde n representa o número de váriaveis e m o número de instâncias
// => As próximas m linhas contem (n + 1) reais onde os n primeiros são os valores das váriaveis e o n+1 é o valor da função dadas as entradas
void initialize_data(char* filename, Training t);

//Determina se tal Representante k, é uma operação ou variavel/constante
int is_operation(int k);

//Determina se tal representante k é uma operação simples(atua sobre uma variavel) ou composta(atua sobre duas variaveis)
//Obs: Assume que k representa uma operação
int is_simple(int k);

//Retorna uma operação aleatória da Entidade
int random_operation(Training t);

//Retorna uma operação SIMPLES aleatória da Entidade
int random_simple_operation(Training t);

//Retorna uma operação COMPOSTA aleatória da Entidade
int random_composite_operation(Training t);

//Retorna uma varivel/constante aleatória da Entidade
int random_variable(Training t);

//Retorna o valor de um input em uma amostra/caso do dataset
double input_value(char in, Training t, int sample);

//Retorna o valor da operação simples para o dado input
double simple_value(char op, double input);

//Retorna o valor da operação composta para o dado input
double composite_value(char op, double input1, double input2);

#endif