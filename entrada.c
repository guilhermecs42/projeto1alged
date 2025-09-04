#include "entrada.h"

#define TAM_INPUT 10

Comando decifrar_comando(char input[]){
	int i=0;
	while(input[i]!='\0' && input[i+1]!='\0'){ // enquanto não chegar no final da linha
		if(input[i]=='\n' || input[i+1]=='\n'){ // não faça comparação com \n !!!
			i++; 
			continue; 
		} 
		if(input[i]!=input[i+1]){ // se os caracteres NÃO forem diferentes
			printf("Tente novamente com um comando válido.\n");
			return COMANDO_INVALIDO;
		}
		i++;
	}
	switch(input[0]){ // ESSE SWITCH TEM QUE SER ATUALIZADO CONFORME OS COMANDOS MUDAM!!!
		case '0': return SAIR;
		case '1': return REGISTRAR_PACIENTE;
		case '2': return ALTA_PACIENTE;
		case '3': return ADD_PROCEDIMENTO;
		case '4': return DESFAZER_PROCEDIMENTO;
		case '5': return CHAMAR_PACIENTE;
		case '6': return MOSTRAR_FILA;
		case '7': return MOSTRAR_HISTORICO;
		default: return COMANDO_INVALIDO;
	}
}

Comando perguntar_comando(){
	char input[TAM_INPUT];
	Comando acao;
	bool overflow;
	
	printf("\n[1] Registrar paciente\n[2] Alta paciente\n[3] Adicionar procedimento\n[4] Desfazer procedimento\n[5] Chamar paciente\n[6] Mostrar fila\n[7] Mostrar histórico\n\n");
	
	do{
		overflow = false;
		
		printf("> ");
		
		if(fgets(input, TAM_INPUT, stdin) == NULL) continue;
		acao = decifrar_comando(input);
		while(strcspn(input, "\n")==strlen(input)){ // enquanto o input não tiver um \n, quer dizer que o buffer está sendo preenchido
			overflow = true; // a flag de overflow é ativada
			fgets(input, TAM_INPUT, stdin); // vai lendo até encontrar um \n, isto é, limpa o buffer
		}
		if(overflow){
			printf("Tente novamente com menos caracteres.\n");
			continue;
		}
		
		if(acao!=COMANDO_INVALIDO){
			return acao;
		}
	}while(acao != SAIR);
}
