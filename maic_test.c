#include <stdio.h>
#include <stdlib.h>
#include "char_heap.h"

void print_heap(Heap h, iterator i){
	if(valid(h,i)){
		if(use(h,i)){
			printf("%c ",value(h,i));
			
			print_heap(h,left_child(h,i));
			print_heap(h,right_child(h,i));
		}
		else printf("0 ");		
	}
}

int main(){
	Heap h = new_heap();
	Heap e = new_heap();
	int i;
	for(i = 0; i < 64; i++){
		heap_insert(h,'0'+i, i, i%2);
	}

	print_heap(h,heap_root(h));
	printf("\n");
	for(i = 0; i < 64; i++){
		if(use(h,i)) printf("%c ",value(h,i));
	}
	printf("\n");

	for(i = 0; i < 26; i++){
		heap_insert(e,'A'+i, i, (i+1)%2);
	}

	print_heap(e,heap_root(e));
	printf("\n");
	for(i = 0; i < 26; i++){
		if(use(e,i)) printf("%c ",value(e,i));
	}
	printf("\n");

	delete_heap(h);
	delete_heap(e);

	return 0;
}