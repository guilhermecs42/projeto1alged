#include<stdlib.h>
#include<string.h>
#include "tratamento.h"
#include "tabela_funcoes.h"

struct tratamento_{
	char medicamento[101];
};



TRATAMENTO* tratamento_criar(const char medicamento[]){
	if(strlen(medicamento)>100){ // se a string de input tiver mais de 99 caracteres, não dá para criar esse TAD
		return NULL;
	}
	TRATAMENTO* t = (TRATAMENTO*)malloc(sizeof(TRATAMENTO)); // alocando a struct
	if(t!=NULL){
		strcpy(t->medicamento, medicamento); // copia a string do input para o struct
	}
	return t;
}



bool tratamento_apagar(void** tratamento){
	if(tratamento == NULL){
		return false;
	}
	TRATAMENTO** t = (TRATAMENTO**) tratamento;
	free(*t);
	*t = NULL;
	return true;
}



bool tratamento_imprimir(void* tratamento){
	if(tratamento == NULL){
		return false;
	}
	TRATAMENTO* t = (TRATAMENTO*)tratamento;
	printf("%s\n", t->medicamento);
	return true;
}



int tratamento_comparar(void* tratamento1, void* tratamento2){
	TRATAMENTO* t1 = (TRATAMENTO*) tratamento1;
	TRATAMENTO* t2 = (TRATAMENTO*) tratamento2;
	
	return strcmp(t1->medicamento, t2->medicamento);
}



bool tratamento_salvar(void* tratamento, FILE* arquivo){
	if(tratamento == NULL || arquivo == NULL){
		return false;
	}
	TRATAMENTO* t = (TRATAMENTO*) tratamento;
	fprintf(arquivo, "===INICIO==TRATAMENTO===\n%s\n=====FIM=TRATAMENTO=====\n", t->medicamento);
	return true;
}



bool tratamento_carregar(void** tratamento, FILE* arquivo){ // aqui o "tratamento" é void** pois precisamos alterar um ponteiro externo para que ele receba o endereço do struct que alocaremos. Sempre que precisamos alterar uma variável externa dentro de uma função, precisamos passar o endereço dela
	if(tratamento == NULL || arquivo == NULL){
		return false;
	}
	char buffer[101];
	char medicamento[101];
	long int posicao_salva = ftell(arquivo); // é necessário salvar a posição do cursor no arquivo, caso dê errado é necessário retornar para essa posição
	fgets(buffer, 101, arquivo); // lendo o cabeçalho
	
	if(strcmp(buffer, "===INICIO==TRATAMENTO===\n")==0){ // se o cabeçalho está correto
	
		fgets(medicamento, 101, arquivo); // lendo o medicamento salvo, incluindo \n
		medicamento[strcspn(medicamento, "\n")] = '\0'; // eliminando o \n 
		posicao_salva = ftell(arquivo); // salvando a posição no início do rodapé, antes de lê-lo
		fgets(buffer, 101, arquivo); // lendo o rodapé
		
		if(strcmp(buffer, "=====FIM=TRATAMENTO=====\n")==0){ // se o rodapé está correto
			
			TRATAMENTO** t = (TRATAMENTO**) tratamento; // converte o ponteiro void** para TRATAMENTO**
			*t = tratamento_criar(medicamento); // cria um tratamento e coloca o medicamento lido nele. Então, modifica a variável original para receber o endereço desse medicamento
			if(*t == NULL){ // se tratamento_criar retornou nulo, significa que não conseguiu alocar
				// acho que aqui não devemos alterar a posição do cursor, pois o erro não foi na leitura de alguma linha, mas sim na alocação de memória
				return false;
			}
			return true;
		}
		// caso o rodapé esteja errado:
		fseek(arquivo, posicao_salva, SEEK_SET); // o cursor fica no começo da linha que gerou o erro
		return false;
	}
	// caso o cabeçalho esteja errado:
	fseek(arquivo, posicao_salva, SEEK_SET); // o cursor fica no começo da linha que gerou o erro
	return false;
}



static const TabelaFuncoes TratamentoFuncoes = { // estou criando uma struct do tipo TabelaFuncoes e fazendo cada atribuição
	.item_apagar = tratamento_apagar,
	.item_imprimir = tratamento_imprimir,
	.item_comparar = tratamento_comparar,
	.item_salvar = tratamento_salvar,
	.item_carregar = tratamento_carregar
};

const TabelaFuncoes* get_tratamento_funcoes(){
	return &TratamentoFuncoes;
}
