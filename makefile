all: main

main: helloworld.o
	g++ helloworld.o -o main

helloworld.o: helloworld.cpp
	g++ -c helloworld.cpp  