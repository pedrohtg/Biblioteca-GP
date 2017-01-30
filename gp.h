#ifndef GP_H_
#define GP_H_

typedef PopStruct* Population;
typedef struct IndStruct* Indvidual;
typedef struct GPStruct* GP;

#define TOTAL_INT_PARAMETER_SIZE 7
#define TOTAL_DOUBLE_PARAMETER_SIZE 3

// Params -------------------------------------------

//DEFAULT Params
#define DEFAULT_pop_size						100 // Population size(fixed max size)
#define DEFAULT_ind_max_height					6 	// The maximum height possible for a Individual
#define DEFAULT_mutation_type					1 	// (1 => Random change mutation_size nodes; 2 => Insert a random subtree in random node)
#define DEFAULT_mutation_size					2 	// For the type 1 mutation
#define DEFAULT_selection_type					1 	// (1 => Tournament Selection; 2 => (TODO) Roulette Selection)
#define DEFAULT_selection_size_reproduction 	25  // Number of selected individuals for reproduction 
#define DEFAULT_tournament_round_size  			10 	// Number of individuals in a single round of tournament
// TO BE CONTINUED

// --------------------------------------------------

//General -------------------------------------------

//Cria uma nova instancia de GP
/*		
	Cada posição do vetor integer_parameters representa um parametro:
	OBS: Posições com valor igual a 0 terão por consequencia um valor Default
[0] - Tamanho da População
[1] - Altura máxima de um Individuo
[2] - Tipo de Mutação
[3] - Tamanho da Mutação(Somente para Mutação Tipo 1)
[4] - Tipo de Seleção
[5] - Número de Individuos selecionados para Reprodução/Crossover
[6] - Números de Individuos observados em um Round de Tournament(Somente para seleção Tournament)
[7] - 
[8] - 
[9] - 
.
.
.	
	Cada posição do vetor double_parameters representa um parametro:
	OBS: Posições com valor igual a 0 terão por consequencia um valor Default
[0] - ...
[1] - ...
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
void run_gp(GP gp);

//Apaga a instancia de GP
void delete_gp(GP gp);

// --------------------------------------------------


//Selection Methods ---------------------------------

//Realiza uma seleção por torneio
Population tournament(Population p, int rounds, int round_size);

// --------------------------------------------------

//Reproduction Methods: Crossovers and Mutations ----

void crossover(Individual i1, Individual i2);

void mutation(Individual i);

// --------------------------------------------------

//Stop(?)


#endif