#include "../../include/utils/entrada.h"
#include<stdlib.h>
#include <errno.h>
#include <limits.h>
#include <ctype.h>

#define TAM_INPUT 10

bool confirmar(){
	char input[TAM_INPUT];
	bool overflow;
	
	do{
		overflow = false;
		
		printf("> ");
		
		if(fgets(input, TAM_INPUT, stdin) == NULL) return false; // lê no máximo 10 caracteres do input do usuário. Caso falhe, tenta de novo.

		if(input[0]=='\n' || input[0]=='\r')continue; // se o usuário apenas digitou enter, pergunte de novo

		while(strchr(input, '\n') == NULL){ // enquanto o input não tiver um \n, quer dizer que o buffer está sendo preenchido
			overflow = true; // a flag de overflow é ativada
			fgets(input, TAM_INPUT, stdin); // continua lendo os próximos 10 caracteres até encontrar um \n
		}
		
		if(overflow){
			printf("Tente novamente com menos caracteres.\n");
			continue;
		}
		
		input[strcspn(input, "\n")] = '\0'; // retirando o \n no final
		
		for(int i=0;input[i]!='\0';i++){ // checando se cada letra do input é igual a '1'
			if(input[i]!='1') return false;
		}
		
		return true;
		
	}while(1);

}

Comando decifrar_comando(char input[]){
	int i=0;
	while(input[i]!='\0' && input[i+1]!='\0'){ // percorre a linha toda
		if(input[i]=='\n' || input[i+1]=='\n'){ // não faça a próxima comparação com \n !!!
			i++; 
			continue; 
		} 
		if(input[i]!=input[i+1]){ // se os caracteres analisados forem diferentes
			printf("Tente novamente com um comando válido.\n");
			return COMANDO_INVALIDO;
		}
		i++;
	}
	
	int comando_valor = input[0] - '0'; // converte o caractere de um número para o inteiro de um número
	if(comando_valor >= NUM_COMANDOS){
		return COMANDO_INVALIDO;
	}else{
		return (Comando)comando_valor;
	}
}

Comando perguntar_comando(){
	char input[TAM_INPUT];
	Comando acao;
	bool overflow;
	
	
	printf("\n[1] Registrar paciente\n[2] Chamar paciente\n[3] Adicionar procedimento\n[4] Desfazer procedimento\n[5] Mostrar fila\n[6] Mostrar histórico\n[7] Registrar óbito de paciente\n[8] Mostrar Registro de Pacientes\n[0] Sair do sistema\n\n");
	
	
	do{
		overflow = false;
		
		printf("> ");
		
		if(fgets(input, TAM_INPUT, stdin) == NULL) continue; // lê no máximo 10 caracteres do input do usuário. Caso falhe, tenta de novo.

		while(strcspn(input, "\n")==strlen(input)){ // enquanto o input não tiver um \n, quer dizer que o buffer está sendo preenchido
			overflow = true; // a flag de overflow é ativada
			fgets(input, TAM_INPUT, stdin); // continua lendo os próximos 10 caracteres até encontrar um \n
		}
		if(overflow){
			printf("Tente novamente com menos caracteres.\n");
			continue;
		}
		
		acao = decifrar_comando(input);
		
		if(acao!=COMANDO_INVALIDO){
			return acao;
		}
	}while(acao != SAIR);

	return SAIR;
}

bool perguntar_string(char* buffer_saida, int tamanho_buffer) {
    bool overflow;

   	while(1){
   		overflow = false;
   	
   		printf("> ");
   		
   		if(fgets(buffer_saida, tamanho_buffer, stdin) == NULL) continue;
   		if(buffer_saida[0] == '\n' || buffer_saida[0] == '\r') continue; // se o usuário só apertou enter, continuar lendo
   		while(strcspn(buffer_saida, "\n")==strlen(buffer_saida)){ // enquanto o buffer não tiver um \n, quer dizer que ainda restou coisas para ler
			overflow = true;
			if(fgets(buffer_saida, tamanho_buffer, stdin)==NULL) return false;
		}
		if(overflow){
			printf("Tente novamente com menos caracteres.\n");
			continue;
		}
		
		buffer_saida[strcspn(buffer_saida, "\n")] = '\0'; // elimina o \n no fim da string
		return true;
	}
}

bool perguntar_numero(int* numero_saida) {
    char buffer[51];
    bool overflow;

    while (true) {

        overflow = false;
        printf("> ");

        if (fgets(buffer, sizeof(buffer), stdin) == NULL) return SAIR;
     
        while(strchr(buffer, '\n') == NULL) { // vê se estourou o buffer, ou seja, não tem um endline
            overflow = true;
            if(fgets(buffer, sizeof(buffer), stdin)==NULL) return false;
        }

        if (overflow) {
            printf("Tente novamente com menos caracteres.\n");
            continue;
        }

        // Limpa o '\n' da string lida
        buffer[strcspn(buffer, "\n")] = '\0';

        char* endptr = NULL; // Ponteiro para o primeiro caractere inválido
        errno = 0;    // Reseta a variável de erro global

        long numero_lido = strtol(buffer, &endptr, 10);

        // Realiza três verificações de validade do strtol
        if (endptr == buffer) {
            // A string não continha nenhum dígito no início (ex: "abc")
            printf("Entrada inválida. Por favor, digite um número.\n");
            continue;
        }
        if (*endptr != '\0') {
            // A string tinha caracteres extras após o número (ex: "123xyz")
            printf("Entrada inválida. Não são permitidos letras ou espaços.\n");
            continue;
        }
        if (errno == ERANGE || numero_lido > INT_MAX || numero_lido < INT_MIN) {
            // O número era muito grande ou muito pequeno para caber em um long/int
            printf("Número fora do intervalo permitido. Tente novamente.\n");
            continue;
        }
		
		// se o número passou as três verificações, retorna true:
		*numero_saida = numero_lido;
        return true;
    }
}

Prioridade decifrar_prioridade(char input[]){

	if(input[0] >= 'a' && input[0] <= 'z') {
        input[0] = toupper(input[0]); // converte o primeiro para maiúscula, por segurança
    }
	
	int i=0;
	while(input[i]!='\0' && input[i+1]!='\0'){ // percorre a linha toda
	
		char atual = toupper(input[i]);
        char proximo = toupper(input[i+1]);
        
		if(input[i]=='\n' || input[i+1]=='\n'){ // não faça a próxima comparação com \n !!!
			i++; 
			continue; 
		}
		if(input[i]!=input[i+1]){ // se os caracteres analisados forem diferentes
			printf("Tente novamente com um nível de prioridade válido.\n");
			return PRIORIDADE_INVALIDA;
		}
		i++;
	}
	
	int comando_valor = input[0] - 'A'; // converte o caractere de um número para o inteiro de um número
		
	if(comando_valor >= NUM_PRIORIDADES || comando_valor <= -1){
		return PRIORIDADE_INVALIDA;
	}else{
		if(comando_valor == '0' - 'A'){ // se o comando é 0, retornar que a operação de inserir paciente deve ser cancelada.
			return SAIR_PRIORIDADE;
		}
		return (Prioridade)comando_valor;
	}
}

Prioridade perguntar_prioridade(){
	char input[TAM_INPUT];
	Prioridade acao;
	bool overflow;
	
	
	printf("Digite o nível de prioridade do paciente:\n[A] Emergência\n[B] Muito Urgente\n[C] Urgente\n[D] Pouco Urgente\n[E] Não Urgente\n");
	
	do{
		overflow = false;
		
		printf("> ");
		
		if(fgets(input, TAM_INPUT, stdin) == NULL) return SAIR_PRIORIDADE; // lê no máximo 10 caracteres do input do usuário. Caso falhe, tenta de novo.
		
		while(strcspn(input, "\n")==strlen(input)){ // enquanto o input não tiver um \n, quer dizer que o buffer está sendo preenchido
			overflow = true; // a flag de overflow é ativada
			fgets(input, TAM_INPUT, stdin); // continua lendo os próximos 10 caracteres até encontrar um \n
		}
		if(overflow){
			printf("Tente novamente com menos caracteres.\n");
			continue;
		}
		
		acao = decifrar_prioridade(input);
		
		if(acao!=PRIORIDADE_INVALIDA){
			return acao;
		}
	}while(1);

	return SAIR_PRIORIDADE;
}

unsigned char perguntar_inserir(){
	char input[TAM_INPUT];
	bool overflow;
	
	
	printf("Informe sobre a situação do paciente:\n[1] Paciente JÁ está cadastrado\n[2] Primeira vez aqui\n[0] Sair\n");
	
	do{
		loop:
		
		overflow = false;
		
		printf("> ");
		
		if(fgets(input, TAM_INPUT, stdin) == NULL) return 0; // lê no máximo 10 caracteres do input do usuário. Caso falhe, cancela a operação.
		
		while(strcspn(input, "\n")==strlen(input)){ // enquanto o input não tiver um \n, quer dizer que o buffer está sendo preenchido
			overflow = true; // a flag de overflow é ativada
			fgets(input, TAM_INPUT, stdin); // continua lendo os próximos 10 caracteres até encontrar um \n
		}
		if(overflow){
			printf("Tente novamente com menos caracteres.\n");
			continue;
		}
		
		int i=0;
		while(input[i]!='\0' && input[i+1]!='\0'){ // percorre a linha toda
		
			if(input[i]=='\n' || input[i+1]=='\n'){ // não faça a próxima comparação com \n !!!
				i++; 
				continue; 
			}
			if(input[i]!=input[i+1]){ // se os caracteres analisados forem diferentes
				printf("Tente novamente com uma entrada válida.\n");
				goto loop;
			}
			i++;
		}
		unsigned char acao = input[0];
		
		if(acao == '0' || acao == '1' || acao == '2'){
			return acao - '0';
		}else{
			printf("Digite 1, 2 ou 0.\n");
		}
	}while(1);
}

