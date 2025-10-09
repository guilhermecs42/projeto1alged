#ifndef FILA_H
	#define FILA_H
	#include <stdio.h>
	#include <stdbool.h>
    #include "tabela_funcoes.h"
	
	typedef struct fila_ FILA;
	
	FILA *fila_criar(const TabelaFuncoes* funcoes);
	int fila_tamanho(FILA* fila);
	bool fila_vazia(FILA* fila);
	bool fila_cheia(FILA* fila);
	void* fila_frente(FILA* fila);
	bool fila_inserir(FILA* fila, void* item);
	void* fila_remover_frente(FILA* fila);
	void fila_apagar(FILA** fila);
	void fila_imprimir(FILA* fila);
	bool fila_salvar(FILA* fila, FILE* arquivo);
	bool fila_carregar(FILA** fila, FILE* arquivo);
	bool fila_buscar(FILA* fila, void* item);

#endif
