#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "char_heap.h"
#include "individual.h"
#include "training.h"

struct IndStruct{
	Heap tree;
	int id;
	double fitness;
};

void random_init(Training t, Individual i, int max_height, int actual_height, iterator pos){
	srand(time(NULL));
	if(actual_height == max_height){
		insert_node(i,random_variable(t), pos);
	}
	else{
		double roll = (double)rand() / (RAND_MAX + 1.0);
		double prob = 1.0 / ( max_height - actual_height + 0.75);

		if(roll <= prob) insert_node(i,random_variable(t), pos);
		else{
			char op = random_operation(t);
			if(is_simple(op)) {
				insert_node(i, op, pos);
				random_init(t,max_height,actual_height+1, left_child(i->tree,pos));
			}
			else{
				insert_node(i, op, pos);
				random_init(t,max_height,actual_height+1, left_child(i->tree,pos));
				random_init(t,max_height,actual_height+1, right_child(i->tree,pos));
			}
		}
	}
}

//Cria um novo Indivuo
Individual new_individual(int max_height, Training t){
	Heap h = new_heap();

	Individual o = malloc(sizeof(struct IndStruct));
	o->tree = h;
	o->id = 0;
	o->fitness = 0;


	// random initialize heap
	random_init(i,t, max_height, 0, 0);
	//

	return o;
}

//Altera o fitness do individuo
void set_fitness(Individual i, double fit){
	i->fitness = fit;
}

//Retorna o fitness do individuo
int get_fitness(Individual i){
	return i->fitness;
}

//Insere um novo nó(função ou variavel) no individuo
void insert_node(Individual i, char c, iterator pos){
	heap_insert(i->tree, c, pos, 0);
}

//Verifica se o no é valido/existe no invidiuo
int valid_node(Individual i, iterator n){
	return use(i->tree,n);
}

//Retorna um iterator para um nó
iterator random_node(Individual i){
	srand(time(NULL));
	
	int size = heap_size(i->tree);
	double prob;


	iterator it = begin(i->tree);
	iterator ret = it;

	int k;
	for(k = 2;k < size && it != end(i->tree); k++, it = next(i->tree,it)){
		double roll = (double)rand() / (RAND_MAX + 1.0);
		prob = 1/(double)k;
		if(roll <= prob) ret = it;
	}

	return ret;
}

//Retorna um iterator para a raiz do Individuo
iterator root_individual(Individual i){
	return heap_root(i->tree);
}

//Apaga um Individuo
void delete_individual(Individual i){
	delete_heap(i->tree);

	free(i);
}

//Troca as subtrees dos individuos i1 e i2, com raizes r1 e r2, respectivamente
void swap(Individual i1, iterator r1, Indivual i2, iterator r2){
	Heap aux = get_subtree(i1->tree, r1);
	heap_insert_subtree(i1->tree, i2->tree, r1, r2);
	heap_insert_subtree(i2->tree, aux, r2, 0);

	delete_heap(aux);
}
