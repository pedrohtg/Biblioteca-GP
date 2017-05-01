CC			= gcc
CP			= g++ -std=c++11
OBJC		= main.o heap.o individual.o population.o training.o gp.o utils.o
LINKOBJC	= main.o heap.o individual.o population.o training.o gp.o utils.o
BINC		= main.exe

all: $(BINC)

cp: $(BIND)

$(BINC): $(OBJC)
	$(CC) $(LINKOBJC) -o $(BINC) -lm

debug: $(OBJC)
	$(CC) $(LINKOBJC) -g -o $(BINC) -lm 

main.o: main_test.c
	$(CC) -c main_test.c -o main.o

heap.o: heap.c
	$(CC) -c heap.c -o heap.o -lm

individual.o: individual.c
	$(CC) -c individual.c -o individual.o -lm

population.o: population.c
	$(CC) -c population.c -o population.o -lm

training.o: training.c
	$(CC) -c training.c -o training.o -lm

gp.o: gp.c
	$(CC) -c gp.c -o gp.o -lm

utils.o: utils.c
	$(CC) -c utils.c -o utils.o -lm

clean:
	rm *o $(BINC)
