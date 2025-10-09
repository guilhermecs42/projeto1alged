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
	if(tratamento == NULL || *tratamento == NULL){
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
	if(tratamento1 == NULL && tratamento2 == NULL) // se ambos forem nulos, são considerados iguais
		return 0;
	if(tratamento2 == NULL) return 1; // se o segundo for nulo e o primeiro não, retorna positivo
	if(tratamento1 == NULL) return -1; // se o primeiro for nulo e o segundo não, retorna negativo
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
	char buffer[151];
	char medicamento[101];
	long int posicao_salva = ftell(arquivo); // é necessário salvar a posição do cursor no arquivo NO INÍCIO DO REGISTRO, caso dê errado é necessário retornar para essa posição
	if(fgets(buffer, 151, arquivo)==NULL){ // lendo o cabeçalho
		fseek(arquivo, posicao_salva, SEEK_SET); // o cursor fica no começo da linha que gerou o erro
		return false;
	}
	if(strcmp(buffer, "===INICIO==TRATAMENTO===\n")==0){ // se o cabeçalho está correto
	
		if(fgets(medicamento, 101, arquivo)==NULL){ // lendo o medicamento salvo, incluindo \n
			fseek(arquivo, posicao_salva, SEEK_SET); // o cursor fica no começo da linha que gerou o erro
			return false; 
		}
		medicamento[strcspn(medicamento, "\n")] = '\0'; // eliminando o \n 
		if(fgets(buffer, 151, arquivo)==NULL){ // lendo o rodapé
			fseek(arquivo, posicao_salva, SEEK_SET); // o cursor fica no começo da linha que gerou o erro
			return false; 
		}
		
		if(strcmp(buffer, "=====FIM=TRATAMENTO=====\n")==0){ // se o rodapé está correto
			
			*tratamento = (void*)tratamento_criar(medicamento); // cria um tratamento e coloca o medicamento lido nele. Então, modifica a variável original para receber o endereço desse medicamento
			if(*tratamento == NULL){ // se tratamento_criar retornou nulo, significa que não conseguiu alocar
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

void* tratamento_copiar(void* tratamento){
	if(tratamento == NULL) return NULL;
	TRATAMENTO* copia = tratamento_criar(((TRATAMENTO*)tratamento)->medicamento);
	return copia;
}



static const TabelaFuncoes TratamentoFuncoes = { // estou criando uma struct do tipo TabelaFuncoes e fazendo cada atribuição
	.item_apagar = tratamento_apagar,
	.item_imprimir = tratamento_imprimir,
	.item_comparar = tratamento_comparar,
	.item_salvar = tratamento_salvar,
	.item_carregar = tratamento_carregar,
	.item_copiar = tratamento_copiar
};

const TabelaFuncoes* get_tratamento_funcoes(){
	return &TratamentoFuncoes; // como a variável é global, ela sempre existe
}
