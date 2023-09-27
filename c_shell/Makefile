mysh: shell.c interpreter.c shellmemory.c
	gcc -D FRAME=$(framesize) -D VAR=$(varmemsize) -c shell.c interpreter.c shellmemory.c
	gcc -o mysh shell.o interpreter.o shellmemory.o

clean: 
	rm mysh; rm *.o
