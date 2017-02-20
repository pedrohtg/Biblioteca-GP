#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <float.h>
#include "int_heap.h"
#include "individual.h"
#include "training.h"

#define INF DBL_MAX

struct IndStruct{
	Heap tree;
	int id;
	double fitness;
};

void random_init(Training t, Individual i, int max_height, int actual_height, iterator pos){
	if(!valid(i->tree, pos)) return ;

	srand(time(NULL));
	if(actual_height >= max_height){
		insert_node(i,random_variable(t), pos);
	}
	else{
		double roll = (double)rand() / (RAND_MAX + 1.0);
		double prob = 1.0 / ( max_height - actual_height + 0.3);

		if(roll <= prob) insert_node(i,random_variable(t), pos);
		else{
			int op = random_operation(t);
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
	Heap h = new_heap(pow(2,max_height + 1) - 1);

	Individual i = malloc(sizeof(struct IndStruct));
	i->tree = h;
	i->id = 0;
	i->fitness = INF;

	// random initialize heap
	random_init(i,t, max_height, 0, 0);
	//

	return i;
}

//Altera o fitness do individuo
void set_fitness(Individual i, double fit){
	i->fitness = fit;
}

//Retorna o fitness do individuo
int get_fitness(Individual i){
	return i->fitness;
}

//Funcao auxiliar para avaliar o valor de um individuo
double aux_avaliate(Heap h, int pos, Training t, int sample){
	if(use(h,pos)){
		int x = value(h,pos);
		if(is_operation(x)){
			if(is_simple(x)) {
				double l_value = aux_avaliate(h,left_child(h,pos), t, sample);
				return simple_value(x,l_value);
			}
			else {
				double l_value = aux_avaliate(h, left_child(h,pos), t, sample);
				double r_value = aux_avaliate(h, right_child(h,pos), t, sample);
				return composite_value(x,l_value,r_value);
			}
		}
		else{
			return input_value(x,t,sample);
		}
	}
}

//Avalia o valor do individuo i para dada instancia 'sample'
double avaliate_individual(Individual i, Training t, int sample){
	return aux_avaliate(i->tree, heap_root(i->tree), t, sample);
}

//Retorna a arvore do Individuo i
Heap get_tree(Individual i){
	return i->tree;
}

//Insere um novo nó(função ou variavel) no individuo
void insert_node(Individual i, int v, iterator pos){
	heap_insert(i->tree, v, pos);
}

//Insere uma subarvore no nó pos do individuo
void insert_subtree(Individual i, Heap h, iterator pos){
	heap_insert_subtree(i->tree, h, pos, 0);
}

//Verifica se o no é valido/existe no invidiuo
int valid_node(Individual i, iterator pos){
	return use(i->tree,pos);
}

//Retorna o valor do nó n no invidiuo
int node_value(Individual i, iterator pos){
	return value(i->tree,pos);
}

//Retorna um iterator para um nó aleatório
iterator random_node(Individual i){
	srand(time(NULL));
	
	int size = heap_size(i->tree);
	double prob;


	iterator it = begin(i->tree);
	iterator ret;

	int k;
	for(k = 1;k <= size && it != end(i->tree); k++, it = next(i->tree,it)){
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
void swap_subtree(Individual i1, iterator r1, Indivual i2, iterator r2){
	Heap aux = get_subtree(i1->tree, r1);
	heap_insert_subtree(i1->tree, i2->tree, r1, r2);
	heap_insert_subtree(i2->tree, aux, r2, 0);

	delete_heap(aux);
}

//Imprime o individuo na tela
//Formato : (Nó pai)valor do nó atual 
void print_individual(Individual i){
	int h = 0;
	if(!heap_empty(i->tree)){
		for(iterator x = begin(i->tree); x != end(i->tree); x = next(x)){
			printf("(%d)", parent(x));

			if(is_operation(x)) printf("%c\t",convert_operation_simbol(value(x)));
			else printf("x%d\t",value(x));
			
			if(height_iterator(x) > h || h == 0){
				printf("\n");
				h++;
			}
		}
	}
	else{
		printf("-- Empty Individual --\n");
	}
}
