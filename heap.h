#ifndef HEAP_H_
#define HEAP_H_

typedef struct HeapStruct* Heap;
typedef int iterator;

#define heap_init_capacity 64

//Cria um novo Heap de CHAR
Heap new_heap();

//Retorna 0 se o heap esta vazio, e 1 caso contrario
int heap_empty(Heap h);

//Retorna o tamanho do heap
int heap_size(Heap h);

//Retorna o tamanho do heap
int heap_capacity(Heap h);

//Aumenta o tamanho máximo do heap
void heap_resize(Heap h, int new_capacity);

//Retorna um iterator para a raiz
iterator heap_root(Heap h);

//Retorna um iterator para begin(raiz do heap)
iterator begin(Heap h);

//Retorna um iterator para end(pos heap capacity)
//Obs: não é um iterator valido;
iterator end(Heap h);

//Retorna um iterator para o prox de i
iterator next(Heap h, iterator i);

//Retorna um iterator para o anterior de i
iterator prev(Heap h, iterator i);

//Retorna se o iterator i é um nó existente
int valid(Heap h, iterator i);

//Retorna o valor do iterator i;
int value(Heap h, iterator i);

//Retorna se o iterator i é um nó usado
int use(Heap h, iterator i);

//Retorna um iterator para o nó pai
iterator parent(iterator x);

//Retorna um iterator para o filho a esquerda
iterator left_child(Heap h, iterator x);

//Retorna um iterator para o filho a direita
iterator right_child(Heap h, iterator x);

//Retorna a altura de um nó;
int height_iterator(iterator x);

//Insere o inteiro v no nó i
void heap_insert(Heap h, int v, iterator i);

//Insere uma subtree do heap sub, com raiz em sub_root, no nó i do heap h
//OBS : Sobreescreve caracteres já adcionados.
void heap_insert_subtree(Heap h, Heap sub, iterator i, iterator sub_root);

//Retorna a subtree de h com raiz r
Heap get_subtree(Heap h, iterator r);

//Remove o o nó e toda a sua subarvore
void heap_remove(Heap h, iterator i);

//Troca o conteúdo entre os heap h e g
void heap_swap(Heap h, Heap g);

//Apaga o conteúdo do Heap h
void clear_heap(Heap h);

//Apaga o conteúdo de g e copia h para g
void heap_copy(Heap h, Heap g);

//Deleta o Heap h
void delete_heap(Heap h);


//DEBUG ONLY
int* array(Heap h);

#endif