all: graph.o main.o
	g++ graph.o main.o -o main -std=c99 -Wall

valgrind:
	valgrind ./main

graph.o:
	g++ -c graph.cpp -o graph.o

main.o:
	g++ -c main.cpp -o main.o

clean:
	rm *.o main

run:
	./main