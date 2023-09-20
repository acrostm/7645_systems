default: compile

swap2.o: swap2.c
	cc -c swap2.c 

main.o: main.c 
	cc -c main.c 

compile: swap2.o main.o
	cc -o myapp swap2.o main.o

clean:
	rm myapp *.o