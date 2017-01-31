#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "heap.h"

#define log2(a) log(a)/log(2)
#define null -1

typedef struct HeapStruct{
	int* array;
	int size;
	int height;
	int capacity;
}HS;

int* array(Heap h){
	return h->array;
}

//Cria um novo Heap de INT
Heap new_heap(){
	Heap h = (Heap)malloc(sizeof(HS));
	h->size = 0;
	h->height = 0;
	h->capacity = heap_init_capacity;
	int *v = (int*)calloc(h->capacity, sizeof(int));
	h->array = v;

	return h;
}

//Retorna 0 se o heap esta vazio, e 1 caso contrario
int heap_empty(Heap h){
	return h->size == 0;
}

//Retorna o tamanho do heap
int heap_size(Heap h){
	return h->size;
}

//Aumenta o tamanho máximo do heap
void heap_resize(Heap h, int new_capacity){
	if(new_capacity > h->capacity){
		int *v = (int*)calloc(new_capacity, sizeof(int));
		int i;
		for(i = 0; i < h->capacity; i++){
			v[i] = h->array[i];
		}

		free(h->array);
		h->array = v;
		h->capacity = new_capacity;
	}
}

//Retorna um iterator para a raiz
iterator heap_root(Heap h){
	return 0;
}

//Retorna um iterator para begin(raiz do heap)
iterator begin(Heap h){
	if(!heap_empty(h)) return 0;
	else return end(h);
}

//Retorna um iterator para end(pos heap capacity)
//Obs: não é um iterator valido;
iterator end(Heap h){
	return h->capacity;
}

//Retorna um iterator para o prox de i
iterator next(Heap h, iterator i){
	i++;
	while(!use(h,i) && i < h->capacity) i++;

	return i;
}

//Retorna um iterator para o anterior de i
iterator prev(Heap h, iterator i){
	i--;
	while(!use(h,i) && i > 0) i--;

	return i;
}

//Retorna se o iterator i é um nó existente
int valid(Heap h, iterator i){
	if(i >= 0 && i < h->capacity) return 1;
	else return 0; 
}

//Retorna o valor do iterator i
int value(Heap h, iterator i){
	return h->array[i];
}

//Retorna se o iterator i é usado
int use(Heap h, iterator i){
	if(valid(h,i))
		return h->array[i] != 0;
	else return 0;
}

//Retorna um iterator para o nó pai
iterator parent(iterator x){
	if(x <= 0) return null;
	else return (x - 1)/2; 
}

//Retorna um iterator para o filho a esquerda
iterator left_child(Heap h, iterator x){
	int l = 2*x + 1;
	if(l > h->capacity) return null;
	else return l; 
}

//Retorna um iterator para o filho a direita
iterator right_child(Heap h, iterator x){
	int r = 2*x + 2;
	if(r > h->capacity) return null;
	else return r;
}

//Retorna a altura de um nó;
int height_iterator(iterator x){
	return floor(log2(x+1));
}

//Insere o inteiro v no nó i
//OBS : Sobreescreve numeros já adcionados.
void heap_insert(Heap h, int v, iterator i){
	if(valid(h,i)){
		if(use(h,i)){
			h->array[i] = v;
		}
		else{
			h->array[i] = v;
			h->size++;

			//Atualiza a altura do heap
			int i_height = height_iterator(i);
			if(h->height < i_height){
				h->height = i_height;
			}
		}
	}			
}

//Função auxiliar, para a inserção de uma subárvore sub em um heap h
void aux_subtree(Heap h, Heap sub, int* count, iterator h_actual, iterator sub_actual){
	if((*count) == heap_size(sub)) return ;
	else{
		if(!use(h,h_actual)) h->size++;
		h->array[h_actual] = sub->array[sub_actual];
		(*count)++;

		//Atualiza a altura do heap
		int actual_height = height_iterator(h_actual);
		if(h->height < actual_height){
			h->height = actual_height;
		}

		iterator left_sub = left_child(sub,sub_actual);
		iterator right_sub = right_child(sub,sub_actual);

		if(use(sub,left_sub)) aux_subtree(h,sub,count,left_child(h,h_actual),left_sub);
		if(use(sub,right_sub)) aux_subtree(h,sub,count,right_child(h,h_actual),right_sub);
	}
}

//Insere uma subtree do heap sub, com raiz em sub_root, no nó i do heap h
//OBS : Sobreescreve caracteres já adcionados.
void heap_insert_subtree(Heap h, Heap sub, iterator i, iterator sub_root){
	int count = 0;
	int *p_count = &count;

	int i_height = height_iterator(i);
	int max_height = log2(h->capacity + 1) - 1;

	if(i_height + sub->height + 1 > max_height){
		heap_resize(h,pow(2, i_height + sub->height + 1));
	}

	heap_remove(h, i);

	aux_subtree(h, sub,p_count,i,sub_root);
}

//Retorna a subtree de h com raiz r
Heap get_subtree(Heap h, iterator r){
	if(r == heap_root(h)) return h;

	Heap sub = new_heap();
	heap_insert_subtree(sub,h,0,r);

	return sub;
}


//Remove o o nó e toda a sua subarvore
void heap_remove(Heap h, iterator i){
	if(valid(h,i)){
		if(use(h,i)) h->size--;
		h->array[i] = 0;
		
		iterator l = left_child(h,i);
		iterator r = right_child(h,i);

		if(use(h,l)) heap_remove(h,l);
		if(use(h,r)) heap_remove(h,r);
	}
}

//Troca o conteúdo entre os heap h e g
void heap_swap(Heap h, Heap g){
	int lg_capacaity = h->capacity > g->capacity ? h->capacity : g->capacity;

	heap_resize(h,lg_capacaity);
	heap_resize(g,lg_capacaity);

	int *aux = h->array;
	int s = h->size;
	int ht = h->height;

	h->array = g->array;
	g->array = aux;

	h->size = g->size;
	g->size = s;

	h->height = g->height;
	g->height = ht;
}

//Apaga o conteúdo do Heap h
void clear_heap(Heap h){
	int i = 0;
	for(i = 0; i < h->capacity; i++)
		h->array[i] = 0;
	h->size = 0;
	h->height = 0;
}

//Apaga o conteúdo de g e copia h para g
void heap_copy(Heap h, Heap g){
	int lg_capacaity = h->capacity > g->capacity ? h->capacity : g->capacity;

	heap_resize(h,lg_capacaity);
	heap_resize(g,lg_capacaity);

	int *v = (int*)calloc(lg_capacaity, sizeof(int));
	int i;
	for(i = 0; i < h->capacity; i++){
		v[i] = h->array[i];
	}

	free(g->array);
	g->array = v;
	g->size = h->size;
	g->height = h->height;
}

//Deleta o Heap h
void delete_heap(Heap h){
	free(h->array);
	free(h);
}
