all: bloco.o cache.o gerador.o main.o memoria.o operacao.o
	@gcc bloco.o cache.o gerador.o main.o memoria.o operacao.o -o exe
	@rm -f bloco.o cache.o gerador.o main.o memoria.o operacao.o

bloco.o: bloco.c sistema.h
	@gcc bloco.c -c -Wall

cache.o: cache.c sistema.h
	@gcc cache.c -c -Wall

gerador.o: gerador.c sistema.h
	@gcc gerador.c -c -Wall

main.o: main.c sistema.h
	@gcc main.c -c -Wall

memoria.o: memoria.c sistema.h
	@gcc memoria.c -c -Wall

operacao.o: operacao.c sistema.h
	@gcc operacao.c -c -Wall

clean:
	@rm -f *.o exe

run: all
	@./exe