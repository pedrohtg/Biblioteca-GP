#ifndef GP_H_
#define GP_H_

typedef struct PopStruct* Population;
typedef struct IndStruct* Individual;
typedef struct TrainingStruct* Training;
typedef struct GPStruct* GP;

#define TOTAL_INT_PARAMETER_SIZE 8
#define TOTAL_DOUBLE_PARAMETER_SIZE 2

// Params -------------------------------------------

//DEFAULT Params INT
#define DEFAULT_number_gen						200 	// Population size(fixed max size)
#define DEFAULT_pop_size						100 	// Population size(fixed max size)
#define DEFAULT_ind_max_height					6 		// The maximum height possible for a Individual
#define DEFAULT_mutation_type					1 		// (1 => Random change mutation_size nodes; 2 => Insert a random subtree in random node)
#define DEFAULT_mutation_size					2 		// For the type 1 mutation
#define DEFAULT_selection_type					1 		// (1 => Tournament Selection; 2 => (TODO) Roulette Selection)
#define DEFAULT_tournament_round_size  			10 		// Number of individuals in a single round of tournament
#define DEFAULT_replace_type					1 		// (1 => The pop_size best individuals in the pool of parents + offspring; 1 => Selection Method in a pool of parents and offspring)
//DEFAULT Params DOUBLE
#define DEFAULT_crossover_probability			0.2		// Percentual of population to be in crossover
#define DEFAULT_mutation_probability			0.05	// Percentual of population to be mutated

// --------------------------------------------------

//General -------------------------------------------

//Cria uma nova instancia de GP
/*		
	Cada posição do vetor integer_parameters representa um parametro:
	OBS: Posições com valor igual a 0 terão por consequencia um valor Default
[0] - Número de Gerações
[1] - Tamanho da População
[2] - Altura máxima de um Individuo
[3] - Tipo de Mutação
[4] - Tamanho da Mutação(Somente para Mutação Tipo 1)
[5] - Tipo de Seleção
[6] - Números de Individuos observados em um Round de Tournament(Somente para seleção Tournament)
[7] - Tipo de Substituição na população
[8] - 
[9] - 
.
.
.	
	Cada posição do vetor double_parameters representa um parametro:
	OBS: Posições com valor igual a 0 terão por consequencia um valor Default
[0] - Percentual da população que será usada no crossover
[1] - Percentual da população que sofrerá mutação
[2] - ...
[3] - ...
[4] - ...
[5] - ...
.
.
.
*/
GP new_gp(Training t, int integer_parameters[TOTAL_INT_PARAMETER_SIZE], double double_parameters[TOTAL_DOUBLE_PARAMETER_SIZE]);

//Executa o processo de gp dada as configurações atuais
Individual run_gp(GP gp);

//Apaga a instancia de GP
void delete_gp(GP gp);

// --------------------------------------------------


//Selection Methods ---------------------------------

//Realiza uma seleção por torneio
Population tournament(Population p, int rounds, int round_size);

//Realiza uma seleção por torneio em um pool formado pela população p + população l
Population tournament_pool(Population p, Population l, int rounds, int round_size);

//Seleciona os n melhores individuos do pool (p + l) 
Population select_best_pool(Population p, Population l, int n);

// --------------------------------------------------

//Reproduction Methods: Crossovers and Mutations ----

void crossover(Individual i1, Individual i2, Population offspring);

void mutation(GP gp, Individual i);

// --------------------------------------------------

//Replace Method
void replace(GP gp, Population offspring);

//Stop(?)


#endif