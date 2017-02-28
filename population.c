#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "training.h"
#include "individual.h"
#include "population.h"

struct PopStruct{
	int max_size;
	Individual* array;
	int size;
};

//Cria uma nova população com tamanho max. = size
Population new_population(int size){
	Population p = malloc(sizeof(struct PopStruct));
	p->max_size = size;

	Individual* arr = malloc(size * sizeof(Individual));
	p->array = arr;
	p->size = 0;

	return p;	
}

//Inicializa uma população -> Adiciona novos individuos até sua capacidade max
void initialize_population(Population p, int ind_max_height, Training t){
	int i;
	for(i = 0; i < p->max_size; i++){
		p->array[i] = new_individual(ind_max_height,t);
		set_id(p->array[i], i);
	}
	p->size = p->max_size;
}

//Retorna o melhor individuo da População p
//Caso mais de um individuo tenha o melhor fitness, retorna o mais recentemente adicionado a população
Individual best_individual(Population p){
	int i;
	int best = 0;
	for(i = 1; i < p->size; i++){
		if(p->array[i]->fitness >= p->array[best]->fitness){
			best = i;
		}
	}

	return p->array[best];
}

//Avalia todos os indivuos de uma dada população
void eval_population(Population p, Training t){
	int i;
	for(i = 0; i < p->size; i++){
		p[i]->fitness = fitness(p[i],t);
	}
}

//Verifica se o individual(identificado pelo id) se encontra na População
int exist_individual(Population p, int id){
	return id >= 0 && id <= p->size;
}

//Retorna o individuo identificado pelo id, caso exista
Individual get_individual(Population p, int id){
	if(exist_individual(p,id)) return p->array[id];
	else return NULL;
}

//Retorna o número de individuos daquela população
int size_population(Population p){
	return p->size;
}

//Insere um novo individuo na População
void insert_population(Population p, Individual i){
	if(p->size < p->max_size){
		p->array[p->size] = i;
		set_id(p->array[p->size], p->size);
		p->size++;
	}
	else{
		printf("Error: Population at Max Size. Could not insert a new Individual.\n");
	}
}

//Apaga o ultimo individuo adicionado a população
void pop_population(Population p){
	p->size--;
	delete_individual(p->array[p->size]);
}

//Apaga todos os individuos de uma população p
void clear_population(Population p){
	int i;
	for(int i = 0; i < p->size; i++){
		delete_individual(p->array[i]);
	}
	p->size = 0;
}

//Deleta a população p
void delete_population(Population p){
	clear_population(p);
	free(p->array);
	free(p);
}

//Fitness Function is implemented in this class
//OBS.: This library is maximum fitness based, in other words, it's maximize the fitness function
//Example and Default function : 
//		Minimum Square Distance

double fitness(Individual i, Training t){
	int k;
	double sum = 0.0;
	double val;
	double out;
	double dif;
	for(k = 0; k < t->data_size; k++){
		val = avaliate_individual(i,t,k);
		out = output_value(t,k);
		dif = val - out;
		sum += dif*dif;
	}

	return -1*sum;	
}
