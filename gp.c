#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <float.h>
#include "heap.h"
#include "population.h"
#include "individual.h"
#include "gp.h"
#include "training.h"
#include "utils.h"

#define RAND_LOOP_LIMIT 333

struct GPStruct{
	Training tr;
	Population p, offspring;
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
	double crossover_probability;		// Percentual of population to be in crossover
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
	gp->crossover_probability = double_parameters[0] > 0 ? double_parameters[0] : DEFAULT_crossover_probability;				
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
	if(!initialized(gp->tr)){
		printf("Error: The training object wasn't correctly initialized.\n");
		return;
	}
	int g,i,j,k;
	int cr_size = (int)ceil((gp->crossover_probability * gp->pop_size));
	int mt_size = (int)ceil((gp->mutation_probability * gp->pop_size));
	
	printf("Crossover size> %d\nMutation Size> %d\n", cr_size, mt_size);

	int *paired = calloc(cr_size,sizeof(int));
	int *mutated = calloc(2*cr_size,sizeof(int));

	Population selected;

	gp->offspring = new_population(2*cr_size);
	
	for(g = 0; g < gp->number_gen; g++){
		// init_genrand(time(NULL));
		eval_population(gp->p, gp->tr);
		

		printf("SELECTION\n\n");
		selected = selection(gp);


		//Crossover
		for(k = 0; k < cr_size; k++){
			paired[k] = -1;
			mutated[k] = -1;
			mutated[k + cr_size] = -1;
		}

		int count = 0;
		printf("CROSSOVER\n\n");
		for(i = 0; i < cr_size; i++){
			int r = genrand_int32()%cr_size;

			while((r == i || paired[r] == i) && count < RAND_LOOP_LIMIT){
				r = genrand_int32()%cr_size;
				count++;
			}
			crossover(get_individual(selected,i), get_individual(selected,r), gp->offspring);
			paired[i] = r;
		}

		delete_only_population(selected);
		// ---------------------

		//Mutation
		printf("MUTATION\n\n");
		count = 0;
		for(j = 0; j < mt_size; j++){
			int r = genrand_int32()%(2*cr_size);

			while(mutated[r] == 0 && count < gp->pop_size){
				r = genrand_int32()%(2*cr_size);
				count++;
			}

			mutation(gp, get_individual(gp->offspring, r));
			mutated[r] = 0;
		}

		printf("REPLACE\n\n");
		//Replace
		replace(gp, gp->offspring);

		printf("CLEAR offspring\n\n");
		clear_population(gp->offspring);
		printf("GO TO NEXT ITERATION(%d)\n\n",g+1);
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
	delete_population(gp->offspring);

	free(gp);
}


//Selection Methods

//Selection
Population selection(GP gp){
	int cr_size = (int)ceil((gp->crossover_probability * gp->pop_size));
	if(gp->selection_type == 1){
		return tournament(gp->p, cr_size, gp->tournament_round_size);
	}
	/*
	else if(gp->selection_type == 2){
		return roullete(gp->p, cr_size);
	}
	*/
	else return NULL;
}

//Realiza uma seleção por torneio
Population tournament(Population p, int rounds, int round_size){
	// init_genrand(time(NULL));

	Population ret = new_population(rounds);
	int i,k;
	int size = size_population(p);
	int chosen,best;
	double best_fit,fit;

	chosen = genrand_int32() % size;

	best = chosen;
	best_fit = get_fitness(get_individual(p,chosen));

	for(i = 0; i < rounds; i++){

		for(k = 0; k < round_size; k++){
			chosen = genrand_int32() % size;
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
	// init_genrand(time(NULL));

	Population ret = new_population(rounds);
	int i,k;
	int size_p = size_population(p);
	int size_l = size_population(l);
	int size = size_p + size_l;
	int chosen, chosen_id,best,best_mod;
	double best_fit,fit;

	chosen_id = genrand_int32() % size;

	best = chosen_id;
	best_fit = chosen_id < size_p ? get_fitness(get_individual(p,chosen_id)) : get_fitness(get_individual(l,(chosen_id - size_p)));

	for(i = 0; i < rounds; i++){

		for(k = 0; k < round_size; k++){
			chosen = genrand_int32() % size;
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

	int *v = calloc(size_p + size_l, sizeof(int));

	Population ret = new_population(n);
	int best_p, best_l;
	double b_fit_p, b_fit_l;
	int i, k, get_p, get_l;
	get_p = get_l = 1;
	for(k = 0; k < n; k++){
		if(get_p){
			//pega best p
			int best_id = -1;
			double best_fit = -DBL_MAX;

			for(i = 0; i < size_p; i++){
				double fit = get_fitness(get_individual(p,i));
				//printf("%lf %lf << \n",DBL_MIN, fit);
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
			double best_fit = -DBL_MAX;

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

		//printf(">>%d %d<<\n",best_p, best_l);

		if(best_p != -1 && best_l != -1){
			if(b_fit_p > b_fit_l){
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
		else if(best_l != -1){
			insert_population(ret, copy_individual(get_individual(l, best_l)));
			get_p = 0;
			get_l = 1;
			v[best_l + size_p] = 1;
		}
		else if(best_p != -1){
			insert_population(ret, copy_individual(get_individual(p, best_p)));
			get_p = 1;
			get_l = 0;
			v[best_p] = 1;
		}	
		
	}

	free(v);
	return ret;
}

// ------------------------------------------------------------

//Reproduction Methods: Crossovers and Mutations
void crossover(Individual i1, Individual i2, Population offspring){
	// init_genrand(time(NULL));
	Individual k1 = copy_individual(i1);
	Individual k2 = copy_individual(i2);

	iterator r1 = random_node(k1);
	iterator r2 = random_node(k2);

	int max_height = individual_max_height(k1);

	//Trata o caso em que os dois nos escolhidos sao as raizes do individuos,
	//resultando em um simples swap;
	int coin;
	int count = 0;
	while(r1 == root_individual(k1) && r2 == root_individual(k2) 
			&& subheap_height(get_tree(k1), r1) >= (max_height - height_iterator(r2))
			&& subheap_height(get_tree(k2), r2) >= (max_height - height_iterator(r1))
			&& count < RAND_LOOP_LIMIT){
		coin = rand() % 2;
		if(coin) r1 = random_node(k1);
		else r2 = random_node(k2);
		count++;
	}

	if(count == RAND_LOOP_LIMIT && r1 == r2){
		if(next(get_tree(k1),r1) != end(get_tree(k1))){
			r1 = next(get_tree(k1), r1);
		}
		else if(next(get_tree(k2),r2) != end(get_tree(k2))){
			r2 = next(get_tree(k2), r2);
		}
	}

	swap_subtree(k1, r1, k2 ,r2);
	insert_population(offspring,k1);
	insert_population(offspring,k2);
}

void mutation(GP gp, Individual i){
	// init_genrand(time(NULL));
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
		iterator chosen = random_node(i);
		int new_subtree_height = gp->ind_max_height - height_iterator(chosen);
		Individual sub = new_individual(new_subtree_height, gp->tr);

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

