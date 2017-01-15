#ifndef GP_H_
#define GP_H_

typedef PopStruct* Population;
typedef struct IndStruct* Indvidual;

//General

//Selection Methods

//Realiza uma seleção por torneio
Population tournament(Population p, int rounds, int round_size);

//Reproduction Methods: Crossovers and Mutations
void crossover(Individual i1, Individual i2);

void mutation(Individual i);

//Stop(?)


#endif