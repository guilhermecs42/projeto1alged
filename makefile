all:
	gcc -c fila.c -o fila.o
	gcc -c pilhadinamica.c -o pilhadinamica.o
	gcc -c entrada.c -o entrada.o
	gcc entrada.o pilhadinamica.o fila.o main.c -o main
clean:
	rm fila.o
	rm entrada.o
	rm pilhadinamica.o
	rm main
