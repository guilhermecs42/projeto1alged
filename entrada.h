#ifndef ENTRADA_H
	#define ENTRADA_H
	#include<stdio.h>
	#include<stdbool.h>
	#include<string.h>
	
	typedef enum{
		COMANDO_INVALIDO = -1,
		SAIR = 0,
		REGISTRAR_PACIENTE = 1,
		ALTA_PACIENTE = 2,
		ADD_PROCEDIMENTO = 3,
		DESFAZER_PROCEDIMENTO = 4,
		CHAMAR_PACIENTE = 5,
		MOSTRAR_FILA = 6,
		MOSTRAR_HISTORICO = 7,
	} Comando;

	Comando perguntar_comando();
	
#endif
