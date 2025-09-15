#ifndef PILHA_H
	#define PILHA_H
	
	#include<stdio.h>
	#include<stdbool.h>
	#include "tabela_funcoes.h"
	
	PILHA* pilha_criar(const TabelaFuncoes* funcoes);
	bool pilha_vazia(PILHA* pilha);
	bool pilha_cheia(PILHA* pilha);
	int pilha_altura(PILHA* pilha);
	void* pilha_topo(PILHA* pilha);
	bool pilha_empilhar(PILHA* pilha, void* item);
	void* pilha_desempilhar(PILHA* pilha);
	bool pilha_apagar(PILHA** pilha_ptr);
	bool pilha_imprimir(PILHA* pilha);
	bool pilha_buscar(PILHA* pilha, void* item);
	bool pilha_salvar(PILHA* pilha, FILE* arquivo);
	bool pilha_carregar(PILHA** pilha, FILE* arquivo);
	
#endif
