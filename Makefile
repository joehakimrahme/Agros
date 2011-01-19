agros: main.o agros.o
	gcc -o agros main.o agros.o
	rm -f *.o

main.o: agros.o include/agros.h
	gcc -c -I include/ src/main.c

agros.o: src/agros.c include/agros.h
	gcc -c -I include/ src/agros.c

clean:  
	rm -f *.o agros

