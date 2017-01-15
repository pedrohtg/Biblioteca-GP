#ifndef POPULATION_H_
#define POPULATION_H_

typedef struct PopStruct* Population;
typedef struct IndStruct* Indvidual;

//Cria uma nova população com tamanho max. size
Population new_population(int size);

//Retorna o melhor individuo da População p
Individual best(Population p);

//Verifica se o individual(identificado pelo id) se encontra na População
int find_individual(Population p, int id);

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

#endif