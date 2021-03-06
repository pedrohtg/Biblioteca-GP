#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <float.h>
#include "heap.h"
#include "individual.h"
#include "training.h"
#include "utils.h"

#define INF -DBL_MAX

struct IndStruct{
	Heap tree;
	int id;
	int mx_height;
	double fitness;
};

void random_init(Training t, Individual i, int max_height, int actual_height, iterator pos){
	if(!valid(i->tree, pos)) return ;

	// init_genrand(time(NULL));
	if(actual_height >= max_height){
		insert_node(i,random_variable(t), pos);
	}
	else{
		double roll = genrand_res53();
		double prob = 1.0 / ( max_height - actual_height + 0.3);

		if(roll <= prob) insert_node(i,random_variable(t), pos);
		else{
			int op = random_operation(t);
			if(is_simple(op)) {
				insert_node(i, op, pos);
				random_init(t,i,max_height,actual_height+1, left_child(i->tree,pos));
			}
			else{
				insert_node(i, op, pos);
				random_init(t,i,max_height,actual_height+1, left_child(i->tree,pos));
				random_init(t,i,max_height,actual_height+1, right_child(i->tree,pos));
			}
		}
	}
}

//Cria um novo Indivuo
Individual new_individual(int max_height, Training t){
	Heap h = new_heap(pow(2,max_height + 1) - 1);

	Individual i = (Individual)malloc(sizeof(struct IndStruct));
	i->tree = h;
	i->id = 0;
	i->fitness = INF;
	i->mx_height = max_height;

	// random initialize heap
	random_init(t,i, max_height, 0, 0);
	//

	return i;
}

//Altera o fitness do individuo
void set_fitness(Individual i, double fit){
	i->fitness = fit;
}

//Retorna o fitness do individuo
double get_fitness(Individual i){
	return i->fitness;
}

//Altera o id do individuo
void set_id(Individual i, int id){
	i->id = id;
}

//Retorna o id do individuo
int get_id(Individual i){
	return i->id;
}

//Retorna a altura do individuo
int individual_height(Individual i){
	return heap_height(i->tree);
}

//Retorna a altura máxima que o individuo pode atingir
int individual_max_height(Individual i){
	return i->mx_height;
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
	printf("Error: Invalid node access.\n");
	return INF;
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

//Retorna o valor do nó pos no invidiuo
int node_value(Individual i, iterator pos){
	return value(i->tree,pos);
}

//Retorna um iterator para um nó aleatório
iterator random_node(Individual i){
	// init_genrand(time(NULL));
	
	int size = heap_size(i->tree);
	double prob;


	iterator it = begin(i->tree);
	iterator ret;

	int k;
	for(k = 1;k <= size && it != end(i->tree); k++, it = next(i->tree,it)){
		double roll = genrand_res53();
		prob = 1/(double)k;
		if(roll <= prob) ret = it;
	}

	return ret;
}

//Retorna um iterator para a raiz do Individuo
iterator root_individual(Individual i){
	return heap_root(i->tree);
}

//Retorna uma cópia do Individuo i
Individual copy_individual(Individual i){
	Heap h = new_heap(pow(2,i->mx_height + 1) - 1);
	heap_copy(i->tree, h);

	Individual cp = malloc(sizeof(struct IndStruct));
	cp->tree = h;
	cp->id = i->id;
	cp->fitness = i->fitness;
	cp->mx_height = i->mx_height;

	return cp;
}

//Apaga um Individuo
void delete_individual(Individual i){
	delete_heap(i->tree);

	free(i);
}

//Troca as subtrees dos individuos i1 e i2, com raizes r1 e r2, respectivamente
void swap_subtree(Individual i1, iterator r1, Individual i2, iterator r2){
	Heap aux = get_subtree(i1->tree, r1);
	heap_insert_subtree(i1->tree, i2->tree, r1, r2);
	heap_insert_subtree(i2->tree, aux, r2, 0);

	delete_heap(aux);
}

void print_aux(Heap h, iterator pos){
	//if(pos != heap_root(h)) printf(" ");
	if(is_operation(value(h,pos))) printf("%c\t",convert_operation_simbol(value(h,pos)));
	else printf("x%d\t",value(h,pos));


	if(use(h,left_child(h,pos))) print_aux(h,left_child(h,pos));
	if(use(h,right_child(h,pos))) print_aux(h,right_child(h,pos));
}

//Imprime o individuo na tela
//Formato : Notação Polonesa( Notação Infixa )
void print_individual(Individual i){
	int h = 0;
	iterator x;
	if(!heap_empty(i->tree)){
		printf("Notação Infixa\n");
		print_aux(i->tree, heap_root(i->tree));
		printf("\n\nTree Structure\n");
		for(x = begin(i->tree); x != end(i->tree); x = next(i->tree,x)){
			if(height_iterator(x) > h){
				printf("\n");
				h++;
			}

			printf("(%d)", parent(x));

			if(is_operation(value(i->tree,x))) printf("%c\t",convert_operation_simbol(value(i->tree,x)));
			else printf("x%d\t",value(i->tree,x));			
		}
	}
	else{
		printf("-- Empty Individual --\n");
	}
}
