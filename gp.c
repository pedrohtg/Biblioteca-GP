#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <random.h>
#include "population.h"
#include "individual.h"
#include "gp.h"
#include "training.h"

struct GPStruct{
	Training tr;
	Population p;
	//GP Params list
	int pop_size; 						// Population size(fixed max size)
	int ind_max_height; 				// The maximum height possible for a Individual
	int mutation_type; 					// (1 => Random change mutation_size nodes; 2 => Insert a random subtree in random node)
	int mutation_size; 					// For the type 1 mutation
	int selection_type; 				// (1 => Tournament Selection; 2 => (TODO) Roulette Selection)
	int selection_size_reproduction; 	// Number of selected individuals for reproduction 
	int tournament_round_size; 			// Number of individuals in a single round of tournament
	//TO BE CONTINUED --- 
};	

//General

//Cria uma nova instancia de GP
GP new_gp(Training t, int integer_parameters[TOTAL_INT_PARAMETER_SIZE], double double_parameters[TOTAL_DOUBLE_PARAMETER_SIZE]){
	GP gp = malloc(sizeof(struct GPStruct));

	// Set INT Parameters -------------------------------
	gp->pop_size = integer_parameters[0] ? integer_parameters[0] : DEFAULT_pop_size;
	gp->ind_max_height = integer_parameters[1] ? integer_parameters[1] : DEFAULT_ind_max_height;			
	gp->mutation_type = integer_parameters[2] ? integer_parameters[2] : DEFAULT_mutation_type;				
	gp->mutation_size = integer_parameters[3] ? integer_parameters[3] : DEFAULT_mutation_size;				
	gp->selection_type = integer_parameters[4] ? integer_parameters[4] : DEFAULT_selection_type;			
	gp->selection_size_reproduction = integer_parameters[5] ? integer_parameters[5] : DEFAULT_selection_size_reproduction;
	gp->tournament_round_size = integer_parameters[6] ? integer_parameters[6] : DEFAULT_tournament_round_size;
	// ----------------------------------------------

	// Set DOUBLE Parameters -------------------------------
	/*
	gp->pop_size = integer_parameters[0] ? integer_parameters[0] : DEFAULT_pop_size;				
	gp->mutation_type = integer_parameters[1] ? integer_parameters[1] : DEFAULT_mutation_type;				
	gp->mutation_size = integer_parameters[2] ? integer_parameters[2] : DEFAULT_mutation_size;	
	*/		
	// ----------------------------------------------

	Population pop = new_population(gp->pop_size);
	initialize_population(pop,gp->ind_max_height, t);

	gp->tr = t;
	gp->p = pop;

	return gp;
}

//Executa o processo de gp dada as configurações atuais
void run_gp(GP gp);

//Apaga a instancia de GP
void delete_gp(GP gp){
	delete_training(gp->tr);
	delete_population(gp->p);

	free(gp);
}


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

	//Trata o caso em que os dois nos escolhidos sao as raizes do individuos,
	//resultando em um simples swap;
	int coin;
	while(r1 == root_individual(i1) && r2 == root_individual(i2)){
		coin = rand() % 2;
		if(coin) r1 = random_node(i1);
		else r2 = random_node(i2);
	}


	swap_subtree(i1, r1, i2 ,r2);
}

void mutation(GP gp, Individual i){
	srand(time(NULL));
	if(gp->mutation_type == 1){
		int i = 0;
		for(;i < gp->mutation_size; i++){
			iterator chosen = random_node(i);
			char chosen_value  = node_value(i,chosen);

			if(is_operation(chosen_value)){
				if(is_simple(chosen_value)){
					insert_node(i, random_simple_operation(gp->tr), chosen);
				}
				else{
					insert_node(i, random_composite_operation(gp->tr), chosen);
				}
			}

			else{
				insert_node(i, random_variable(gp->tr), chosen);
			}

		}
	}

	else if(gp->mutation_type == 2){
		int new_subtree_height = 1 + (rand() % (gp->ind_max_height/2));
		Individual sub = new_individual(new_subtree_height, gp->tr);

		iterator chosen = random_node(i);
		insert_subtree(i, get_tree(sub), chosen);

		delete_individual(sub);
	}

}

//Stop(?)

