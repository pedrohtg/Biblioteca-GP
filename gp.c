#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <random.h>
#include "population.h"
#include "individual.h"

//General

//Selection Methods

//Realiza uma seleção por torneio
Population tournament(Population p, int rounds, int round_size){
	srand(time(NULL));

	Population ret = new_population(rounds);
	int i,k;
	int size = size_population(p);
	int chosen,best;
	double best_fit,fit;

	chosen_id = rand() % size;

	best = chosen_id;
	best_fit = get_fitness(get_individual(p,chosen_id));

	for(int i = 0; i < rounds; i++){

		for(k = 0; k < round_size; k++){
			chosen = rand() % size;
			fit = get_fitness(get_individual(p,chosen));
			//TODO ->
			//Which is Better Fitness - Implemented in Population(???)
			// ------
			if(fit == which_is_better(fit,best_fit)){
				best = chosen_id;
				best_fit = fit;
			}
		}

		insert_population(ret,get_individual(best));
	}

	return ret;
}

//Reproduction Methods: Crossovers and Mutations
void crossover(Individual i1, Individual i2){
	srand(time(NULL));
	iterator r1 = random_node(i1);
	iterator r2 = random_node(i2);

	//Trata o caso em que os dois nos escolhidos sao as raizes do individuos;
	//Resultando em um simples swap;
	int coin;
	while(r1 == root_individual(i1) && r2 == root_individual(i2)){
		coin = rand() % 2;
		if(coin) r1 = random_node(i1);
		else r2 = random_node(i2);
	}


	swap_subtree(i1, r1, i2 ,r2);
}

void mutation(Individual i);

//Stop(?)

