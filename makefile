lispc: lisp_c.o
	gcc -o lispc lisp_c.o -lm 

lispc.o: lisp_c.c
	gcc -g lisp_c.c 
