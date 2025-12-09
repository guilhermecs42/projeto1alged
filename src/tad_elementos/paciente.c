#include "../../include/tad_elementos/paciente.h"
#include "../../include/tad_estruturas/pilha.h"
#include<string.h>
#include<stdlib.h>

#define TAM_NOME 95

struct paciente{
	int id;
	char nome[TAM_NOME];
	PILHA* historico;
};


PACIENTE* paciente_criar(int id, char nome[]){
	if(strlen(nome)>100){
		return NULL;
	}
	PACIENTE* paciente = (PACIENTE*)malloc(sizeof(PACIENTE));
	if(paciente!=NULL){
		paciente->historico = pilha_criar(get_tratamento_funcoes());
		if(paciente->historico == NULL){
			free(paciente);
			return NULL;	
		}
		paciente->id = id;
		strcpy(paciente->nome, nome);
	}
	return paciente;
}

void paciente_set_id(PACIENTE* paciente, int id) {
    if (paciente != NULL) {
        paciente->id = id;
    }
}

void paciente_set_nome(PACIENTE* paciente, char* nome) {
    if (paciente != NULL && nome != NULL) {
        strncpy(paciente->nome, nome, TAM_NOME);
        paciente->nome[TAM_NOME] = '\0';
    }
}

bool paciente_apagar(void** paciente){
	if(paciente == NULL || *paciente == NULL){
		return false;
	}
	PACIENTE** p = (PACIENTE**) paciente;
	pilha_apagar(&((*p)->historico));
	free(*p);
	*p = NULL;
	return true;
}

bool paciente_historico_cheio(PACIENTE* paciente){
	if(paciente == NULL || pilha_altura(paciente->historico) >= 10){
		return true;
	}
	return false;
}

bool paciente_historico_vazio(PACIENTE* paciente){
	if(paciente == NULL || pilha_altura(paciente->historico) == 0){
		return true;
	}
	return false;
}

bool paciente_adicionar_tratamento(PACIENTE* paciente, TRATAMENTO* tratamento){
	if(tratamento == NULL || paciente_historico_cheio(paciente)){
		return false;
	}
	pilha_empilhar(paciente->historico, (void*)tratamento);
	return true;
}

bool paciente_remover_tratamento(PACIENTE* paciente){
	if(paciente_historico_vazio(paciente)){
		return false;
	}
	pilha_remover_topo(paciente->historico);
	return true;
}

bool paciente_imprimir(void* paciente){
	if(paciente==NULL) return false;
	PACIENTE* p = (PACIENTE*) paciente;
	printf("Nome: %s\nID: %d\n", p->nome, p->id);
	return true;
}

int paciente_get_id(PACIENTE* paciente){
	if(paciente==NULL){
		return -1;
	}else{
		return paciente->id;
	}
}

char* paciente_get_nome(PACIENTE* paciente){
	if(paciente == NULL){
		return NULL;
	}else{
		return paciente->nome;
	}
}

bool paciente_consultar_historico(PACIENTE* paciente){
	if(paciente==NULL) return false;
	return pilha_imprimir(paciente->historico); // retorna true se tudo deu certo, false se algum tratamento é nulo
}

int paciente_comparar(void* paciente1, void* paciente2){
	PACIENTE* p1 = (PACIENTE*) paciente1;
	PACIENTE* p2 = (PACIENTE*) paciente2;
	if(p1 == NULL && p2 == NULL) // se ambos forem nulos, são considerados iguais
		return 0;
	if(p2 == NULL) return 1; // se o segundo for nulo e o primeiro não, retorna positivo
	if(p1 == NULL) return -1; // se o primeiro for nulo e o segundo não, retorna negativo
	
	return (p1->id - p2->id);
}

bool paciente_salvar(void* paciente, FILE* arquivo){
	if(paciente == NULL || arquivo == NULL){
		return false;
	}
	
	PACIENTE* p = (PACIENTE*)paciente;
	fprintf(arquivo, "===INICIO==PACIENTE===\n");
	fprintf(arquivo, "%d\n", p->id);
	fprintf(arquivo, "%s\n", p->nome);
	if( pilha_salvar(p->historico, arquivo) == false)return false; // caso a pilha não consiga salvar todos os tratamentos, deixa o arquivo em formato corrompido
	fprintf(arquivo, "====FIM===PACIENTE====\n");
	return true;
}

bool paciente_carregar(void** paciente, FILE* arquivo){
    if (paciente == NULL || arquivo == NULL) {
        return false;
    }
    long int pos_inicial = ftell(arquivo); // salvando a posição inicial para retornar caso dê erro
    char buffer[151]; // Buffer para ler linhas do arquivo
    
    if (fgets(buffer, sizeof(buffer), arquivo) == NULL || strcmp(buffer, "===INICIO==PACIENTE===\n") != 0) { // se a leitura falhar ou a string lida não for o cabeçalho
        fseek(arquivo, pos_inicial, SEEK_SET);
        return false;
    }

    int id;
    char nome[TAM_NOME];

	// Lendo o id:
    if (fgets(buffer, sizeof(buffer), arquivo) == NULL || sscanf(buffer, "%d", &id) != 1) {
        fseek(arquivo, pos_inicial, SEEK_SET);
        return false;
    }

	// Lendo o nome:
    if (fgets(nome, sizeof(nome), arquivo) == NULL) {
        fseek(arquivo, pos_inicial, SEEK_SET);
        return false;
    }
    nome[strcspn(nome, "\n")] = '\0'; // Remove o '\n' do final da string
    // criando um paciente para armazenar os dados lidos
    PACIENTE* paciente_temp = paciente_criar(id, nome);
    if (paciente_temp == NULL) {
        fseek(arquivo, pos_inicial, SEEK_SET);
        return false;
    }
    // não é necessário apagar manualmente a pilha criada por paciente_criar, pois a função pilha_carregar fará isso automaticamente

    // Carregando o histórico
    if (pilha_carregar(&(paciente_temp->historico), arquivo) == false) {
        paciente_apagar((void**)&paciente_temp); // Limpa o paciente temporário
        fseek(arquivo, pos_inicial, SEEK_SET);
        return false;
    }

    // lendo o rodapé:
    if (fgets(buffer, sizeof(buffer), arquivo) == NULL || strcmp(buffer, "====FIM===PACIENTE====\n") != 0) {
        paciente_apagar((void**)&paciente_temp); // Limpa o paciente temporário
        fseek(arquivo, pos_inicial, SEEK_SET);
        return false;
    }

    // apaga o paciente antigo e substitui pelo novo

    *paciente = paciente_temp;
    return true;
}

void* paciente_copiar(void* paciente){
	if(paciente==NULL)return NULL;
	PACIENTE* p = (PACIENTE*) paciente;
	PACIENTE* copia = (PACIENTE*)paciente_criar(p->id, p->nome);
	return copia;
}

static const TabelaFuncoes PacienteFuncoes = { // estou criando uma struct do tipo TabelaFuncoes e fazendo cada atribuição
	.item_apagar = paciente_apagar,
	.item_imprimir = paciente_imprimir,
	.item_comparar = paciente_comparar,
	.item_salvar = paciente_salvar,
	.item_carregar = paciente_carregar,
	.item_copiar = paciente_copiar
};

const TabelaFuncoes* get_paciente_funcoes(){
	return &PacienteFuncoes;
}
