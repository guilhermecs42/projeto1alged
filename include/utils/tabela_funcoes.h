#ifndef TABELA_FUNCOES_H
	#define TABELA_FUNCOES_H
	
	#include<stdbool.h>
	#include<stdio.h>
	
	typedef struct tabelafuncoes_ {
		bool (*item_apagar)(void**);
		bool (*item_imprimir)(void*);
		int  (*item_comparar)(void*, void*);
		bool (*item_salvar)(void*, FILE*);
		bool (*item_carregar)(void**, FILE*);
		void* (*item_copiar)(void* item_original);
	} TabelaFuncoes; // o nome técnico é Dispatch Table
	
#endif
