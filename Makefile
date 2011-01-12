agros: main.o agros.o
	gcc -lreadline -lncurses -o agros main.o agros.o
	rm -f *.o

main.o: agros.o include/agros.h
	gcc -c -I include/ -lreadline -lncurses src/main.c

agros.o: src/agros.c include/agros.h
	gcc -c -I include/ -lreadline -lncurses src/agros.c

clean:  
	rm -f *.o agros

