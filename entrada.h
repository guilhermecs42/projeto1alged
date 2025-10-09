#ifndef ENTRADA_H
	#define ENTRADA_H
	#include<stdio.h>
	#include<stdbool.h>
	#include<string.h>
	
	typedef enum{
		SAIR, // valor implícito 0
		REGISTRAR_PACIENTE, // valor implícito 1
		CHAMAR_PACIENTE, // valor implícito 2
		ADD_PROCEDIMENTO,
		DESFAZER_PROCEDIMENTO,
		MOSTRAR_FILA,
		MOSTRAR_HISTORICO,
		PACIENTE_MORREU,
		NUM_COMANDOS, // valor implícito é igual ao número de comandos nas linhas acima
		COMANDO_INVALIDO = -1, // valor explícito -1
	} Comando;

	bool confirmar(void);
	/*
	A função confirmar retorna true caso o usuário tenha digitado 1, 11, 111, etc.
	Ela retorna false caso o usuário tenha digitado algo que tenha outros caracteres além de 1.
	Caso o usuário digite algo com mais de 10 letras, ela pede para que ele tente digitar novamente. Isso previne buffer overflow.
	Use essa função sempre que você precisar de confirmação do usuário para continuar com uma operação.
	*/
	
	
	Comando perguntar_comando();
	/*
	A função perguntar_comando é o menu central do sistema: ela recebe o input de um usuário, que deve ser uma sequência de números como:
	0, 00, 000
	1, 11, 111
	2, 22, 222
	e assim por diante.
	Caso o input não se encaixe em uma dessas sequências, pede-se o input novamente.
	Caso o input tenha mais de 10 letras, pede-se o input novamente. Isso previne buffer overflow.
	Essa função só aparece uma vez no código principal: no loop central no final da main.
	*/
	
	
	bool perguntar_string(char* buffer_saida, int tamanho_buffer);
	/*
	A função perguntar_string é usada para ler nomes que o usuário digita
	Ela retorna true em caso de sucesso e false em caso de fracasso
	O parâmetro tamanho_buffer deve corresponder ao tamanho da string buffer_saida, para previnir buffer overflow
	Essa função também tem um sistema de perguntar o input de novo caso o usuário digite demais
	*/
	
	bool perguntar_numero(int* numero_saida);
	/*
	A função perguntar_numero é usada para ler números (IDs) que o usuário digita
	Ela retorna true em caso de sucesso e false em caso de fracasso
	A variável apontada pelo ponteiro numero_saida recebe o valor do número digitado pelo usuário
	Essa função também tem um sistema de perguntar o input de novo caso o usuário digite demais
	*/
	
#endif
