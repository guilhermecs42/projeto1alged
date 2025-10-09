#ifndef LISTA_H
  #define LISTA_H
	#include <stdio.h>
	#include <stdbool.h>
  #include "tabela_funcoes.h"
	
	typedef struct lista_ LISTA;
	
	LISTA* lista_criar(const TabelaFuncoes* funcoes);
	void lista_apagar(LISTA** lista);
	int lista_tamanho(LISTA* lista);
	bool lista_vazia(LISTA* lista);
	bool lista_cheia(LISTA* lista);
	bool lista_inserir_inicio(LISTA* lista, void* item);
	bool lista_inserir_fim(LISTA* lista, void* item);
	bool lista_inserir_posicao(LISTA* lista, void* item, int n);
	void* lista_remover_inicio(LISTA* lista);
	void* lista_remover_posicao(LISTA* lista, int n);
	void* lista_remover_fim(LISTA* lista);
	bool lista_remover_chave(LISTA* lista, void* chave);
	void lista_imprimir(LISTA* lista);
	LISTA* lista_buscar(LISTA* lista, void* item);
	void* lista_buscar_ponteiro(LISTA* lista, void* chave);
	bool lista_salvar(LISTA* lista, FILE* arquivo);
	bool lista_carregar(LISTA** lista, FILE* arquivo);

#endif
