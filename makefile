all:
	gcc -c fila.c -o fila.o
	gcc -c pilha.c -o pilha.o
	gcc -c lista.c -o lista.o
	gcc -c tratamento.c -o tratamento.o
	gcc -c paciente.c -o paciente.o
	gcc -c entrada.c -o entrada.o
	gcc fila.o pilha.o lista.o tratamento.o paciente.o entrada.o main.c -o main
	./main
clean:
	rm fila.o
	rm entrada.o
	rm pilha.o
	rm lista.o
	rm tratamento.o
	rm paciente.o
	rm main
