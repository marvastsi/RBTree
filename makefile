arvore_rb:
	gcc -c abb_rb.c
	gcc main.c abb_rb.o -o main.exe
	./main.exe

clean:
	rm *.o
	rm *.exe
