agros: main.o agros.o
	gcc -Wall `pkg-config --libs glib-2.0` -o agros main.o agros.o
	rm -f *.o

main.o: agros.o include/agros.h
	gcc -Wall -c -I include/ `pkg-config --cflags glib-2.0` src/main.c

agros.o: src/agros.c include/agros.h
	gcc -Wall -c -I include/ src/agros.c

clean:  
	rm -f *.o agros

