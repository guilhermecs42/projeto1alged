#ifndef PACIENTE_H
	#define PACIENTE_H
	
	#include<stdbool.h>
	#include<stdio.h>
	#include "../utils/tabela_funcoes.h"
	#include "tratamento.h"
	
	typedef struct paciente PACIENTE;
	
	PACIENTE* paciente_criar(int id, char nome[]);
	
	void paciente_set_id(PACIENTE* paciente, int id);
	
	void paciente_set_nome(PACIENTE* paciente, char* nome);
	
	bool paciente_apagar(void** paciente);
	
	bool paciente_historico_cheio(PACIENTE* paciente);
	
	bool paciente_historico_vazio(PACIENTE* paciente);
	
	bool paciente_adicionar_tratamento(PACIENTE* paciente, TRATAMENTO* tratamento);
	
	bool paciente_remover_tratamento(PACIENTE* paciente);
	
	bool paciente_imprimir(void* paciente);
	
	int paciente_get_id(PACIENTE* paciente);
	
	char* paciente_get_nome(PACIENTE* paciente);
	
	bool paciente_consultar_historico(PACIENTE* paciente);
	
	int paciente_comparar(void* paciente1, void* paciente2);
	
	bool paciente_salvar(void* paciente, FILE* arquivo);
	
	bool paciente_carregar(void** paciente, FILE* arquivo);
	
	void* paciente_copiar(void* paciente);
	
	const TabelaFuncoes* get_paciente_funcoes();
	
#endif
