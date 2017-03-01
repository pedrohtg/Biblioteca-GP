#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <random.h>
#include <float.h>
#include "population.h"
#include "individual.h"
#include "gp.h"
#include "training.h"

struct GPStruct{
	Training tr;
	Population p;
	//GP Params list
	int number_gen; 					// Number of generations
	int pop_size; 						// Population size(fixed max size)
	int ind_max_height; 				// The maximum height possible for a Individual
	int mutation_type; 					// (1 => Random change mutation_size nodes; 2 => Insert a random subtree in random node)
	int mutation_size; 					// For the type 1 mutation; Number of nodes that will be changed
	int selection_type; 				// (1 => Tournament Selection; 2 => (TODO) Roulette Selection)
	int tournament_round_size; 			// Number of individuals in a single round of tournament
	int replace_type;					// (1 => The pop_size best individuals in the pool of parents + offspring; 1 => Selection Method in a pool of parents and offspring)
	//Doubles
	double crossover_probabilty;		// Percentual of population to be in crossover
	double mutation_probability;		// Percentual of population that can be mutated
};	

//General

//Cria uma nova instancia de GP
//OBS.: Espera-se que o Training t ja tenha sido inicializado.
GP new_gp(Training t, int integer_parameters[TOTAL_INT_PARAMETER_SIZE], double double_parameters[TOTAL_DOUBLE_PARAMETER_SIZE]){
	GP gp = malloc(sizeof(struct GPStruct));

	// Set INT Parameters -------------------------------
	gp->number_gen = integer_parameters[0] > 0 ? integer_parameters[0] : DEFAULT_number_gen;
	gp->pop_size = integer_parameters[1] > 0 ? integer_parameters[1] : DEFAULT_pop_size;
	gp->ind_max_height = integer_parameters[2] > 0 ? integer_parameters[2] : DEFAULT_ind_max_height;			
	gp->mutation_type = integer_parameters[3] > 0 ? integer_parameters[3] : DEFAULT_mutation_type;				
	gp->mutation_size = integer_parameters[4] > 0 ? integer_parameters[4] : DEFAULT_mutation_size;				
	gp->selection_type = integer_parameters[5] > 0 ? integer_parameters[5] : DEFAULT_selection_type;			
	gp->tournament_round_size = integer_parameters[6] > 0 ? integer_parameters[6] : DEFAULT_tournament_round_size;
	gp->replace_type = integer_parameters[7] > 0 ? integer_parameters[7] : DEFAULT_replace_type;
	// ----------------------------------------------

	// Set DOUBLE Parameters -------------------------------	
	gp->crossover_probabilty = double_parameters[0] > 0 ? double_parameters[0] : DEFAULT_crossover_probabilty;				
	gp->mutation_probability = double_parameters[1] > 0 ? double_parameters[1] : DEFAULT_mutation_probability;				
	// ----------------------------------------------

	Population pop = new_population(gp->pop_size);
	initialize_population(pop,gp->ind_max_height, t);

	gp->tr = t;
	gp->p = pop;

	return gp;
}

//Executa o processo de gp dada as configurações atuais
Individual run_gp(GP gp){
	//init
	if(!initialized(gp->t)){
		printf("Error: The training object wasn't correctly initialized.\n");
		return;
	}
	int g,i,j,k;
	int cr_size = (int)(gp->crossover_probabilty * gp->pop_size);
	int mt_size = (int)(gp->crossover_probabilty * gp->pop_size);
	
	int *paired = malloc(cr_size*sizeof(int));
	int *mutated = malloc(2*cr_size*sizeof(char));

	Population selected, offspring;

	offspring = new_population(2*cr_size);
	
	for(g = 0; g < gp->number_gen; g++){
		srand(time(NULL));
		eval_population(gp->p);
		//selection method --> TODO
		selected = tournament(gp->p, cr_size, gp->tournament_round_size);

		//Crossover
		for(k = 0; k < cr_size; k++){
			paired[k] = -1;
			mutated[k] = 0;
			mutated[k + cr_size] = 0;
		}

		for(i = 0; i < cr_size; i++){
			int r = rand()%cr_size;

			while(r == i || paired[r] == i) r = rand()%cr_size;

			crossover(get_individual(selected,i), get_individual(selected,r), offspring);
			paired[i] = r;
		}
		// ---------------------

		//Mutation
		for(j = 0; j < mt_size; j++){
			int r = rand()%(2*cr_size);

			while(mutated[r] == 0) r = rand%(2*cr_size);

			mutation(gp, get_individual(offspring, r));
			mutated[r] = 0;
		}

		//Replace
		replace(gp, offspring);

		clear_population(offspring);
	}

	free(paired);
	free(mutated);

	return best_individual(gp->p);

	//for 0 to number_gen
		//eval population
		//selection
		//reproduction
		//replace
	//
	//return best_individual
}

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

	for(i = 0; i < rounds; i++){

		for(k = 0; k < round_size; k++){
			chosen = rand() % size;
			fit = get_fitness(get_individual(p,chosen));
			if(fit >= best_fit){
				best = chosen;
				best_fit = fit;
			}
		}

		insert_population(ret,get_individual(p,best));
	}

	return ret;
}

//Realiza uma seleção por torneio em um pool formado pela população p + população l
Population tournament_pool(Population p, Population l, int rounds, int round_size){
	srand(time(NULL));

	Population ret = new_population(rounds);
	int i,k;
	int size_p = size_population(p);
	int size_l = size_population(l);
	int size = size_p + size_l;
	int chosen,best,best_mod;
	double best_fit,fit;

	chosen_id = rand() % size;

	best = chosen_id;
	best_fit = chosen_id < size_p ? get_fitness(get_individual(p,chosen_id)) : get_fitness(get_individual(l,(chosen_id - size_p)));

	for(i = 0; i < rounds; i++){

		for(k = 0; k < round_size; k++){
			chosen = rand() % size;
			fit = chosen < size_p ? get_fitness(get_individual(p,chosen)) : get_fitness(get_individual(l,(chosen - size_p)));
			if(fit >= best_fit){
				best = chosen;
				best_fit = fit;
			}
		}

		if(best < size_p)
			insert_population(ret,copy_individual(get_individual(p,best)));
		else
			insert_population(ret,copy_individual(get_individual(l,best - size_p)));
	}

	return ret;
}


//Seleciona os n melhores individuos do pool (p + l) 
Population select_best_pool(Population p, Population l, int n){
	int size_p = size_population(p);
	int size_l = size_population(l);

	if(n > size_p + size_l){
		printf("Error: in function 'select_best_pool': Size of the pool is smaller than the number of to be select individuals.\n");
		return NULL;
	}

	int v* = calloc(size_p + size_l, sizeof(int));

	Population ret = new_population(n);
	int best_p, best_l;
	double b_fit_p, b_fit_l;
	int i, k, get_p, get_l;
	get_p = get_l = 1;

	for(k = 0; k < n; k++){
		if(get_p){
			//pega best p
			int best_id = -1;
			double best_fit = DBL_MIN;

			for(i = 0; i < size_p; i++){
				double fit = get_fitness(get_individual(p,i));
				if(fit >= best_fit && !v[i]){
					best_id = i;
					best_fit = fit;
				}
			}

			best_p = best_id;
			b_fit_p = best_fit;
		}
		if(get_l){
			//pega best l
			int best_id = -1;
			double best_fit = DBL_MIN;

			for(i = 0; i < size_l; i++){
				double fit = get_fitness(get_individual(l,i));
				if(fit >= best_fit && !v[i + size_p]){
					best_id = i;
					best_fit = fit;
				}
			}

			best_l = best_id;
			b_fit_l = best_fit;
		}

		if(best_l == -1 || b_fit_p > b_fit_l){
			insert_population(ret, copy_individual(get_individual(p, best_p)));
			get_p = 1;
			get_l = 0;
			v[best_p] = 1;
		}
		else{
			insert_population(ret, copy_individual(get_individual(l, best_l)));
			get_p = 0;
			get_l = 1;
			v[best_l + size_p] = 1;
		}		
		
	}

	free(v);
	return ret;
}

// ------------------------------------------------------------

//Reproduction Methods: Crossovers and Mutations
void crossover(Individual i1, Individual i2, Population offspring){
	srand(time(NULL));
	Individual k1 = copy_individual(i1);
	Individual k2 = copy_individual(i2);

	iterator r1 = random_node(k1);
	iterator r2 = random_node(k2);

	//Trata o caso em que os dois nos escolhidos sao as raizes do individuos,
	//resultando em um simples swap;
	int coin;
	while(r1 == root_individual(k1) && r2 == root_individual(k2)){
		coin = rand() % 2;
		if(coin) r1 = random_node(k1);
		else r2 = random_node(k2);
	}


	swap_subtree(k1, r1, k2 ,r2);
	insert_population(k1);
	insert_population(k2);
}

void mutation(GP gp, Individual i){
	srand(time(NULL));
	if(gp->mutation_type == 1){
		int x = 0;
		for(;x < gp->mutation_size; x++){
			iterator chosen = random_node(i);
			int chosen_value  = node_value(i,chosen);

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

// --------------------------------------------------
// Replace method
void replace(GP gp, Population offspring){
	if(gp->replace_type == 1){
		Population ret = select_best_pool(gp->p, offspring, gp->pop_size);
		delete_population(gp->p);
		gp->p = ret;
	}
	else if(gp->replace_type == 2){
		Population ret = tournament_pool(gp->p, offspring, gp->pop_size, gp->tournament_round_size);
		delete_population(gp->p);
		gp->p = ret;		
	}
}

//Stop(?)

