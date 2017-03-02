#ifndef INDIVIDUAL_H_
#define INDIVIDUAL_H_

typedef struct HeapStruct* Heap;
typedef struct IndStruct* Individual;
typedef struct TrainingStruct* Training;
typedef int iterator;

//Cria um novo Indivuo
Individual new_individual(int max_height, Training t);

//Altera o fitness do individuo
void set_fitness(Individual i, double fit);

//Retorna o fitness do individuo
double get_fitness(Individual i);

//Altera o id do individuo
void set_id(Individual i, int id);

//Retorna o id do individuo
int get_id(Individual i);

//Retorna a altura do individuo
int individual_height(Individual i);

//Retorna a altura máxima que o individuo pode atingir
int individual_max_height(Individual i);

//Avalia o valor do individuo i para dada instancia 'sample'
double avaliate_individual(Individual i, Training t, int sample);

//Retorna a arvore do Individuo i
Heap get_tree(Individual i);

//Insere um novo nó(função ou variavel) no individuo
void insert_node(Individual i, int v, iterator pos);

//Insere uma subarvore no nó pos do individuo
void insert_subtree(Individual i, Heap h, iterator pos);

//Verifica se o no é valido/existe no invidiuo
int valid_node(Individual i, iterator pos);

//Retorna o valor do nó pos no invidiuo
int node_value(Individual i, iterator pos);

//Retorna um iterator para um nó aleatório
iterator random_node(Individual i);

//Retorna um iterator para a raiz do Individuo
iterator root_individual(Individual i);

//Retorna uma cópia do Individuo i
Individual copy_individual(Individual i);

//Apaga um Individuo
void delete_individual(Individual i);

//Troca as subtrees dos individuos i1 e i2, com raizes r1 e r2, respectivamente
void swap_subtree(Individual i1, iterator r1, Individual i2, iterator r2);

//Imprime o individuo na tela
void print_individual(Individual i);



#endif