#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "gp.h"
#include "population.h"
#include "individual.h"
#include "gp.h"
#include "training.h"
#include "utils.h"

void test_ind();
void test_pop();
void test_tra();
void test_gp();

int main(){
	
	//test_ind();
	// test_pop();
	// test_tra();
	 test_gp();

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
	Training t1 = new_training(1);
	Training t2 = new_training(1);
	Training t3 = new_training(1);
	Training t4 = new_training(1);

	set_operations(t1, 4, '+', '*', 's', '#');
	set_operations(t2, 3, '-', '/', '@');
	set_operations(t3, 2, '+', 'e');
	set_operations(t4, 3, 'c', 'l', '^');


	char op1 = convert_operation_simbol(random_operation(t1));
	char op2 = convert_operation_simbol(random_operation(t2));
	char op3 = convert_operation_simbol(random_operation(t3));
	char op4 = convert_operation_simbol(random_operation(t4));

	printf("OP1 : %c\tOP2 : %c\tOP3 : %c\tOP4 : %c\n", op1,op2,op3,op4);

	printf("1\n");
	op1 = convert_operation_simbol(random_simple_operation(t1));
	printf("2\n");
	op2 = convert_operation_simbol(random_simple_operation(t2));
	printf("3\n");
	op3 = convert_operation_simbol(random_simple_operation(t3));
	printf("4\n");
	op4 = convert_operation_simbol(random_simple_operation(t4));

	printf("OP1 : %c\tOP2 : %c\tOP3 : %c\tOP4 : %c\n", op1,op2,op3,op4);

	printf("1a\n");
	op1 = convert_operation_simbol(random_composite_operation(t1));
	printf("2a\n");
	op2 = convert_operation_simbol(random_composite_operation(t2));
	printf("3a\n");
	op3 = convert_operation_simbol(random_composite_operation(t3));
	printf("4a\n");
	op4 = convert_operation_simbol(random_composite_operation(t4));

	printf("OP1 : %c\tOP2 : %c\tOP3 : %c\tOP4 : %c\n", op1,op2,op3,op4);


	initialize_data("data_sets/catalanSequence.txt", t1);
	initialize_data("data_sets/magicSquareSequence.txt", t2);
	initialize_data("data_sets/squareSequence.txt", t3);
	initialize_data("data_sets/symbolic.txt", t4);

	int i, j;
	i = training_size(t1);
	printf("T1 (%d)\n",i);
	for(j = 0; j < i; j++){
		double a = output_value(t1,j);
		printf(">%lf\n", a);
		//printf(">%lf\n", output_value(t1,j));
	}
	// printf("\n--\n");
	// i = training_size(t2);
	// printf("T2 (%d)\n",i);
	// for(j = 0; j < i; j++){
	// 	printf("%lf\n", output_value(t2,j));
	// }
	// printf("\n--\n");
	// i = training_size(t3);
	// printf("T3 (%d)\n",i);
	// for(j = 0; j < i; j++){
	// 	printf("%lf\n", output_value(t3,j));
	// }
	// printf("\n--\n");
	// i = training_size(t4);
	// printf("T4 (%d)\n",i);
	// for(j = 0; j < i; j++){
	// 	printf("%lf\n", output_value(t4,j));
	// }
	 printf("\n--\n");
	
	double r[11];

	r[0] = simple_value(-7, 4.0);
	r[1] = simple_value(-8, -5.0);
	r[2] = simple_value(-9, M_PI);
	r[3] = simple_value(-10, M_PI);
	r[4] = simple_value(-11, 5.0);

	r[5] = composite_value(-1, 2.0, 3.0);
	r[6] = composite_value(-2, 2.0, 3.0);
	r[7] = composite_value(-3, 2.0, 3.0);
	r[8] = composite_value(-4, 2.0, 3.0);
	r[9] = composite_value(-5, 2.0, 3.0);
	r[10] = composite_value(-6, 2.0, 3.0);

	printf("Simple op results\n");
	for(i = 0; i < 5; i++){
		printf("%lf\n",r[i]);
	}
	printf("Compos op results\n");
	for(; i < 11; i++){
		printf("%lf\n",r[i]);
	}

	delete_training(t1);
	delete_training(t2);
	delete_training(t3);
	delete_training(t4);
}

void test_gp(){
	Training t1 = new_training(1);
	Training t2 = new_training(1);
	Training t3 = new_training(1);
	Training t4 = new_training(1);

	set_operations(t1, 4, '+', '*', 's', '#');
	set_operations(t2, 3, '-', '/', '@');
	set_operations(t3, 2, '+', 'e');
	set_operations(t4, 3, 'c', 'l', '^');

	initialize_data("data_sets/squareSequence.txt", t1);

	Individual a = new_individual(4, t1);
	Individual b = new_individual(6, t1);

	int int_param[TOTAL_INT_PARAMETER_SIZE] = {100,50,5,0,0,0,0,0};
	double double_param[TOTAL_DOUBLE_PARAMETER_SIZE] = {0.2, .05};

	GP gp = new_gp(t1,int_param,double_param);
	
	Individual c = run_gp(gp);

	print_individual(c);

	delete_gp(gp);

	//delete_training(t1);
	delete_training(t2);
	delete_training(t3);
	delete_training(t4);

	delete_individual(a);
	delete_individual(b);
}