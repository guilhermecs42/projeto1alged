#ifndef TRATAMENTO_H
	#define TRATAMENTO_H
	
	#include<stdbool.h>
	#include<stdio.h>
	
	typedef struct tratamento_ TRATAMENTO;
	
	TRATAMENTO* tratamento_criar(const char medicamento[]);
	
	bool tratamento_apagar(void** tratamento);
	
	bool tratamento_imprimir(void* tratamento);
	
	int tratamento_comparar(void* tratamento1, void* tratamento2);
	
	bool tratamento_salvar(void* tratamento, FILE* arquivo);
	
	bool tratamento_carregar(void** tratamento, FILE* arquivo);
	
#endif
