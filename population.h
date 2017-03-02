#ifndef POPULATION_H_
#define POPULATION_H_

typedef struct PopStruct* Population;
typedef struct IndStruct* Individual;
typedef struct TrainingStruct* Training;

//Cria uma nova população com tamanho max. size
Population new_population(int size);

//Inicializa uma população -> Adiciona novos individuos até sua capacidade max
void initialize_population(Population p, int ind_max_height, Training t);

//Retorna o melhor individuo da População p
Individual best_individual(Population p);

//Avalia todos os indivuos de uma dada população
void eval_population(Population p, Training t);

//Verifica se o individual(identificado pelo id) se encontra na População
int exist_individual(Population p, int id);

//Retorna o individuo identificado pelo id, caso exista
Individual get_individual(Population p, int id);

//Retorna o número de individuos daquela população
int size_population(Population p);

//Insere um novo individuo na População
void insert_population(Population p, Individual i);

//Apaga o ultimo individuo adicionado a população
void pop_population(Population p);

//Apaga todos os individuos de uma população p
void clear_population(Population p);

//Deleta a população p
void delete_population(Population p);

//Fitness Function is implemented in this class
//OBS.: This library is maximum fitness based, in other words, it's maximize the fitness function
/*
The fitness function have to respect the following:
Return type: double
Parameters: Individual i, Training t
*/
double fitness(Individual i, Training t);


#endif