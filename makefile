contabilidade: main.o util.o fatos.o conta.o lancamentocontabil.o
	gcc -o contabilidade main.o util.o fatos.o conta.o lancamentocontabil.o

main.o: main.c estrutura.h util.h fatos.h conta.h lancamentocontabil.h
	gcc -c main.c

util.o: util.c util.h estrutura.h
	gcc -c util.c

lancamentocontabil.o: lancamentocontabil.c lancamentocontabil.h estrutura.h 
	gcc -c lancamentocontabil.c

fatos.o: fatos.c fatos.h estrutura.h
	gcc -c fatos.c

conta.o: conta.c conta.h estrutura.h util.h 
	gcc -c conta.c