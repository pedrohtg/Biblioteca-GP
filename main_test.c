#include <stdio.h>
#include <stdlib.h>
#include "gp.h"

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
	Training t = new_training(4);
	Individual a = new_individual(4, t);

	print_individual(a);
	printf("\n--\n" );
	insert_node(a, -3, 0);
	insert_node(a, -1, 1);

	print_individual(a);
}

void test_pop(){

}

void test_tra(){

}

void test_gp(){

}