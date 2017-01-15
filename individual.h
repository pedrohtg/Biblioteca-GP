#ifndef INDIVIDUAL_H_
#define INDIVIDUAL_H_

typedef struct IndStruct* Indvidual;
typedef int iterator;

//Cria um novo Indivuo
Individual new_individual();

//Altera o fitness do individuo
void set_fitness(Individual i, double fit);

//Retorna o fitness do individuo
int get_fitness(Individual i;

//Insere um novo nó(função ou variavel) no individuo
void insert_node();

//Verifica se o no é valido/existe no invidiuo
int valid_node(Individual i, iterator n);

//Retorna um iterator para um nó
iterator random_node(Individual i);

//Retorna um iterator para a raiz do Individuo
iterator root_individual(Individual i);

//Apaga um Individuo
void delete_individual(Individual i);

//Troca as subtrees dos individuos i1 e i2, com raizes r1 e r2, respectivamente
void swap(Individual i1, iterator r1, Indivual i2, iterator r2);

#endif