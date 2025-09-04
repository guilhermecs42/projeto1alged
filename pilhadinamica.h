#ifndef PILHA_H
	#define PILHA_H
	
	#include<stdio.h>
	#include<stdbool.h>
	typedef struct pilha PILHA;
	
	PILHA* pilha_criar(void (*item_apagar)(void* item), void (*item_salvar)(void* item, FILE* arquivo), bool (*item_carregar)(void** item, FILE* arquivo));
	bool pilha_vazia(PILHA* pilha);
	bool pilha_cheia(PILHA* pilha);
	int pilha_altura(PILHA* pilha);
	bool pilha_empilhar(PILHA* pilha, void* item);
	void* pilha_desempilhar(PILHA* pilha);
	void pilha_apagar(PILHA** pilha_ptr);
	bool pilha_salvar(PILHA* pilha, FILE* arquivo);
	bool pilha_carregar(PILHA** pilha, FILE* arquivo);
	
#endif
