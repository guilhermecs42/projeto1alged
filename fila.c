#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "tabela_funcoes.h"

typedef struct no_ NO;
typedef struct fila_ FILA;

struct no_{
    void* item;
    NO* prox;
};
 
struct fila_{
    NO* inicio;
    NO* fim;
    int tam;
    const TabelaFuncoes* item_funcoes;
};

FILA* fila_criar(const TabelaFuncoes* funcoes){
    FILA* f = (FILA*)malloc(sizeof(FILA));
    if(f != NULL){
        f->inicio = NULL;
        f->fim = NULL;
        f->tam = 0;
        f->item_funcoes = funcoes;
    }
    return f;
}

int fila_tamanho(FILA* fila){
	return (fila != NULL) ? fila->tam : -1;
}

bool fila_vazia(FILA* fila){
    if(fila == NULL){
        return true;
    }
    return (fila->tam == 0);
}

bool fila_cheia(FILA* fila){
    if(fila != NULL){
		NO* novo = (NO*)malloc(sizeof(NO)); // tenta alocar espaço para mais um nó na heap
		if(novo != NULL){                   // se a alocação der certo:
			free(novo);                     // desaloco o nó porque não vou usá-lo
			return false;                   // como pude alocar o nó, a heap ainda não está cheia
		}
		return true; // se a alocação der errado, a heap está cheia
	}
	return true;
}

void* fila_frente(FILA* fila){
    if(!fila_vazia(fila)){
        return fila->inicio->item;
    }
    return NULL;
}

bool fila_inserir(FILA* fila, void* item){
    if(!fila_cheia(fila)){
        NO* n = (NO*)malloc(sizeof(NO));
        if(n != NULL){
            n->item = item;
            n->prox = NULL;
            if(fila_vazia(fila)){     
                fila->inicio = n;    // Se a fila está vazia, o novo nó é também o início da fila
            }else{
                fila->fim->prox = n; // Se não, o antigo fim aponta para o novo nó
            }
            fila->fim = n;            // O novo nó é sempre o novo fim da fila
            fila->tam++;       
            return true;
        }
    }
    return false;
}

void fila_remover_frente(FILA* fila){
    if(!fila_vazia(fila)){
        void* item = fila->inicio->item;  // O ponteiro auxiliar 'item' recebe o endereço do item armazenado na fila
        NO* prox = fila->inicio->prox;    // O endereço do próximo nó após o início é armazenado num auxiliar 'prox'
        free(fila->inicio);               // A memória alocada pro nó de início é liberada e
        fila->inicio = prox;              // o início da fila recebe o endereço armazenado no auxiliar 'prox'
        fila->tam--;
        if(fila->tam == 0){               // Caso depois da remoção a fila fique vazia,
            fila->fim = NULL;             // o fim da fila também deve receber NULL
        }
        fila->item_funcoes->item_apagar(&item); // Apaga o item 
    }
}

void fila_apagar(FILA** fila){
    if(fila != NULL && *fila != NULL){
        NO* no_atual = (*fila)->inicio;
        NO* no_aux;

        while (no_atual != NULL) {
            no_aux = no_atual->prox;     // Guarda o próximo nó
            if ((*fila)->item_funcoes != NULL && (*fila)->item_funcoes->item_apagar != NULL) {
                (*fila)->item_funcoes->item_apagar(&(no_atual->item));
            }
            free(no_atual);              // Libera o nó atual
            no_atual = no_aux;           // Avança para o próximo
        }
        free(*fila);
        *fila = NULL; 
    }
}

void fila_imprimir(FILA* fila){
    if (fila == NULL || fila_vazia(fila) || fila->item_funcoes == NULL || fila->item_funcoes->item_imprimir == NULL) {
        return;
    }
    NO* no_atual = fila->inicio;
    while (no_atual != NULL) {
        fila->item_funcoes->item_imprimir(no_atual->item); // Chamada via tabela de funções
        no_atual = no_atual->prox;
    }
}

bool fila_salvar(FILA* fila, FILE* arquivo){
    if (fila == NULL || arquivo == NULL || fila->item_funcoes == NULL || fila->item_funcoes->item_salvar == NULL) {
        return false;
    }

    fprintf(arquivo, "======INICIO=FILA======\n");
    NO* no_atual = fila->inicio;
    while (no_atual != NULL) {
        fila->item_funcoes->item_salvar(no_atual->item, arquivo); // Chamada via tabela
        no_atual = no_atual->prox;
    }
    fprintf(arquivo, "======FIM=FILA======\n");
    return true;
}

bool fila_carregar(FILA** fila, FILE* arquivo){
    if(*fila == NULL || (*fila)->item_funcoes == NULL || (*fila)->item_funcoes->item_carregar == NULL){
		return false;
	}
	char linha[100];
    long int ultima_pos = ftell(arquivo);
	fgets(linha, 100, arquivo);                          // lê o cabeçalho
	if(strcmp(linha, "======INICIO=FILA======\n") != 0){ // se não for exatamente a linha esperada, dá erro
        fseek(arquivo, ultima_pos, SEEK_SET);
		return false;
	}

        // Cria fila temporária com as mesmas funções da original
	FILA* fila_temp = fila_criar((*fila)->item_funcoes);
    if(fila_temp == NULL){
        fseek(arquivo, ultima_pos, SEEK_SET);
        return false;
    }

	while(1){
		void* item;
		if(fila_temp->item_funcoes->item_carregar(&item, arquivo) == false){
            // lê do arquivo o próximo item da fila e guarda o endereço da struct na variável local.
			// se falhar, sai do loop: pode ser que tenha chegado no rodapé, ou que o arquivo esteja corrompido
			// quando a função carregar_elemento falha, o cursor deve estar no começo da linha que gerou o erro.
			break;
		} 
		fila_inserir(fila_temp, item); // coloca na fila temporária
	}

	fgets(linha, 100, arquivo);                        // lê a linha que parou o loop
	if(strcmp(linha, "======FIM=FILA======\n") != 0){  // se não for exatamente o rodapé, dá erro
		fila_apagar(&fila_temp);
		return false;
	}
	// caso dê certo:
	fila_apagar(fila); // apaga a fila antiga
	*fila = fila_temp; // substitui pela fila nova
	return true;
}
bool fila_buscar(FILA* fila, void* item){
    if(item == NULL || fila == NULL){
		return false;
	}
	NO* aux = fila->inicio;
	while(aux != NULL){
		if(fila->item_funcoes->item_comparar(aux->item, item) == 0){
			return true;
		}
		aux = aux->prox;
	}
	return false;
}