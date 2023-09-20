default: compile

string_functions.o: string_functions.c
	cc -c string_functions.c

main.o: main.c
	cc -c main.c

compile: string_functions.o main.o
	cc -o myapp string_functions.o main.o

clean:
	rm myapp *.o
