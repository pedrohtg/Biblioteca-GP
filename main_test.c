#include <stdio.h>
#include <stdlib.h>
#include "gp.h"

typedef int iterator;

void test_ind();
void test_pop();
void test_tra();
void test_gp();

int main(){
	
	test_ind();
	// test_pop();
	// test_tra();
	// test_gp();

	printf("\n\n");
	return 0;
}

void test_ind(){
	srand(time(NULL));
	init_genrand(time(NULL));

	Training t = new_training(4);
	Individual a = new_individual(4, t);
	Individual b = new_individual(6, t);

	printf("A:\n");
	print_individual(a);
	printf("\n--\nB:\n" );
	print_individual(b);
	printf("\n--\n" );

	Individual c = copy_individual(a);

	iterator r1 = random_node(a);
	iterator r2 = random_node(b);
	printf("R1 : %d \t R2 : %d\n",r1,r2);
	swap_subtree(a,r1,b,r2);

	printf("A:\n");
	print_individual(a);
	printf("\n--\nB:\n" );
	print_individual(b);
	printf("\n--\nC:" );
	print_individual(c);
	printf("\n--\n" );

	delete_individual(a);
	delete_individual(b);
	delete_individual(c);

	delete_training(t);
}

void test_pop(){

}

void test_tra(){
	Training t = new_training(1);
}

void test_gp(){

}