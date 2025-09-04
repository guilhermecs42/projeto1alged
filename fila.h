#ifndef FILA_H
	#define FILA_H
	#include<stdio.h>
	#include<stdbool.h>
	
	typedef struct fila_ FILA;
	
	FILA *fila_criar(void);
	int fila_tamanho(FILA* fila);
	bool fila_vazia(FILA* fila);
	bool fila_cheia(FILA* fila);
	void* fila_frente(FILA* fila);
	bool fila_inserir(FILA* fila, void* item);
	void* fila_remover(FILA* fila);
	void fila_apagar(FILA** fila);
	void fila_imprimir(FILA* fila, void (*funcao_imprimir)(void*));
	void fila_salvar(FILA* fila, char* nomearquivo, void (*funcao_salvar)(void*, FILE*));
	
#endif
