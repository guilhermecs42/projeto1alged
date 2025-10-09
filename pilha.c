#include<stdlib.h>
#include<string.h>
#include "pilha.h"
#include "tabela_funcoes.h"


typedef struct no_ NO;

struct no_{
	void* item; // item que este nó contém
	NO* anterior; // ponteiro para o nó anterior
};

struct pilha_{
	NO* topo; // é um ponteiro para o elemento no topo da pilha
	int altura;
	const TabelaFuncoes* item_funcoes;
};



PILHA* pilha_criar(const TabelaFuncoes* funcoes){
	PILHA* pilha = (PILHA*)malloc(sizeof(PILHA));
	if(pilha != NULL){
		pilha->topo = NULL;
		pilha->altura = 0;
		pilha->item_funcoes = funcoes;
	}
	return pilha;
}

bool pilha_vazia(PILHA* pilha){
	if(pilha != NULL){
		return (pilha->altura == 0) ? true : false;
	}
	return true;
}

bool pilha_cheia(PILHA* pilha){ // verifica se há espaço disponível na heap
	if(pilha != NULL){
		NO* novo = (NO*)malloc(sizeof(NO)); // tenta alocar espaço para mais um nó na heap
		if(novo != NULL){ // se a alocação der certo:
			free(novo); // desaloco o nó porque não vou usá-lo
			novo = NULL;
			return false; // como pude alocar o nó, a heap ainda não está cheia
		}
		return true; // se a alocação der errado, a heap está cheia
	}
	return true;
}

int pilha_altura(PILHA* pilha){
	return (pilha!=NULL) ? pilha->altura : -1;
}

void* pilha_topo(PILHA* pilha){
	if(!pilha_vazia(pilha)){
		return pilha->topo->item;
	}
	return NULL;
}

bool pilha_empilhar(PILHA* pilha, void* item){
	if(!pilha_cheia(pilha)){
		NO* pnovo = (NO*)malloc(sizeof(NO));
		if(pnovo != NULL){
			pnovo->item = item;
			pnovo->anterior = pilha->topo;
			pilha->topo = pnovo;
			pilha->altura++;
			return true;
		}
	}
	return false;
}

void pilha_remover_topo(PILHA* pilha){
	if(!pilha_vazia(pilha)){
		void* item = pilha->topo->item;
		NO* topo_antigo = pilha->topo;
		pilha->topo = pilha->topo->anterior;
		pilha->altura--;
		
		topo_antigo->anterior = NULL; // isola o nó antes de dar free nele, evita bugs de dangling pointer
		free(topo_antigo);
		topo_antigo = NULL;
		
		pilha->item_funcoes->item_apagar(&item);
	}
}

void pilha_apagar(PILHA** pilha_ptr){
	NO* no_aux;
	if(*pilha_ptr != NULL && !pilha_vazia(*pilha_ptr)){
		while((*pilha_ptr)->topo != NULL){
			no_aux = (*pilha_ptr)->topo;
			(*pilha_ptr)->item_funcoes->item_apagar(&(no_aux->item));
			(*pilha_ptr)->topo = no_aux->anterior;
			free(no_aux);
		}
	}
	free(*pilha_ptr);
	*pilha_ptr = NULL;
}


bool pilha_imprimir(PILHA* pilha){
	if(pilha!=NULL){
		NO* no_aux = pilha->topo;
		while(no_aux!=NULL){
			if(pilha->item_funcoes->item_imprimir(no_aux->item) == false){ // chama a função imprimir_elemento e se ela falhar, retorna false
				return false;
			}
			no_aux = no_aux->anterior;
		}
		return true;
	}
	return false;
}

bool pilha_buscar(PILHA* pilha, void* item){ // uma função que verifica se um determinado item já está na pilha
	if(pilha != NULL && item != NULL){
		NO* no_aux = pilha->topo;
		while(no_aux != NULL){
			if(pilha->item_funcoes->item_comparar(no_aux->item, item) == 0){ // faz a verificação e se forem iguais:
				return true;
			}
		}
		return false;
	}
	return false;
}

void pilha_salvar_aux(PILHA* pilha, FILE* arquivo, NO* no_atual){
	if(no_atual == NULL){ // se eu passei da base da pilha, volta
		return;
	}
	pilha_salvar_aux(pilha, arquivo, no_atual->anterior); // chama a função para o próximo elemento da pilha
	pilha->item_funcoes->item_salvar(no_atual->item, arquivo); // essa linha só roda depois de chegar na base da pilha pela linha anterior, a partir daí vai voltando de chamada em chamada salvando cada elemento até o topo
	return;
}

bool pilha_salvar(PILHA* pilha, FILE* arquivo){
	if(pilha==NULL){
		return false;
	}
	fprintf(arquivo, "======BASE=PILHA======\n"); // esse cabeçalho indica que começamos a escrever a pilha pela base
	pilha_salvar_aux(pilha, arquivo, pilha->topo); // chamo uma função recursiva auxiliar começando pelo topo da pilha
	fprintf(arquivo, "======TOPO=PILHA======\n"); // esse rodapé indica que o final do arquivo é o topo da pilha
	return true;
}

bool pilha_carregar(PILHA** pilha, FILE* arquivo){
	if(pilha == NULL || *pilha == NULL || (*pilha)->item_funcoes->item_carregar == NULL){
		return false;
	}
	char linha[100];
	long int ultima_pos;
	ultima_pos = ftell(arquivo);
	fgets(linha, 100, arquivo); // lê o cabeçalho
	if(strcmp(linha, "======BASE=PILHA======\n")!=0){ // se não for exatamente a linha esperada, dá erro
		fseek(arquivo, ultima_pos, SEEK_SET); // retorna à posição antes do erro
		return false; // retorna erro
	}
	PILHA* pilha_temp = pilha_criar((*pilha)->item_funcoes); // cria uma pilha temporária com as mesmas funções da pilha recebida
	while(1){
		void* item; // cria uma variável local que vai receber o endereço do próximo item a ser inserido na pilha
		if(pilha_temp->item_funcoes->item_carregar(&item, arquivo) == false){
			// lê do arquivo o próximo item da pilha e guarda o endereço da struct na variável local.
			// se falhar, sai do loop: pode ser que tenha chegado no rodapé, ou que o arquivo esteja corrompido
			// quando a função item_carregar falha, o cursor deve estar no começo da linha que gerou o erro
			break;
		} 
		pilha_empilhar(pilha_temp, item); // coloca na pilha temporária
	}
	fgets(linha, 100, arquivo); // lê a linha que parou o loop
	if(strcmp(linha, "======TOPO=PILHA======\n") != 0){ // se não for exatamente o rodapé, dá erro
		pilha_apagar(&pilha_temp); // apaga a pilha temporária e todos os itens dela
		return false;
	}
	// caso dê certo:
	pilha_apagar(pilha); // apaga a pilha antiga
	*pilha = pilha_temp; // substitui pela pilha nova
	return true;
}
