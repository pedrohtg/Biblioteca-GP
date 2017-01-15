CC			= gcc
CP			= g++ -std=c++11
OBJC		= main.o heap.o
OBJD		= mainD.o heapP.o
LINKOBJC	= main.o heap.o
LINKOBJB	= mainD.o heapP.o
BINC		= main.exe
BIND		= main_T.exe

all: $(BINC)

cp: $(BIND)

$(BINC): $(OBJC)
	$(CC) $(LINKOBJC) -o $(BINC) -lm

$(BIND): $(OBJD)
	$(CP) $(LINKOBJD) -o $(BIND) -lm 

mainDB.o: main.c
	$(CC) -c -g -o mainDB.o main.c

pqueueDB.o: pqueue.c
	$(CC) -c -g -o pqueueDB.o pqueue.c

debug: $(OBJCDB)
	$(CC) -g -o $(BINC) $(LINKOBJCDB)

main.o: maic_test.c
	$(CC) -c maic_test.c -o main.o

mainD.o: test.cpp
	$(CP) -c test.cpp -o mainD.o

heap.o: char_heap.c
	$(CC) -c char_heap.c -o heap.o -lm

heapP.o: char_heap.c
	$(CP) -c char_heap.c -o heap.o -lm

clean:
	rm *o $(BINC)
