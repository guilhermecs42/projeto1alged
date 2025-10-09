#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "tabela_funcoes.h"

typedef struct no_ NO;
typedef struct lista_ LISTA;

struct no_{
    void* item;
    NO* prox;
};
 
struct lista_{
    NO* inicio;
    NO* fim;
    int tam;
    const TabelaFuncoes* item_funcoes;
};

LISTA* lista_criar(const TabelaFuncoes* funcoes){
	if(funcoes==NULL)return NULL;
	
    LISTA* lista = (LISTA*)malloc(sizeof(LISTA));

    if(lista != NULL){
        lista->inicio = NULL;
        lista->fim = NULL;
        lista->tam = 0;
        lista->item_funcoes = funcoes;
    }

    return lista;
}

void lista_apagar(LISTA** lista){
    if(lista !=  NULL && *lista != NULL){

        NO* no_atual = (*lista)->inicio;
        NO* no_aux;

        while (no_atual != NULL) {
            no_aux = no_atual->prox;                                 // Guarda o próximo nó
            if((*lista)->item_funcoes->item_apagar != NULL){
            	(*lista)->item_funcoes->item_apagar(&(no_atual->item));  // Apaga o item armazenado no nó
            }
            free(no_atual);                                          // Libera o nó atual
            no_atual = no_aux;                                       // Avança para o próximo
        }

        free(*lista);
        *lista = NULL; 
    }
}

int lista_tamanho(LISTA* lista){
	return (lista != NULL) ? lista->tam : -1;
}

bool lista_vazia(LISTA* lista){
    if(lista == NULL){
        return true;
    }
    return (lista->tam == 0);
}

bool lista_cheia(LISTA* lista){
    if(lista != NULL){
		NO* novo = (NO*)malloc(sizeof(NO)); // tenta alocar espaço para mais um nó na heap
		if(novo != NULL){                   // se a alocação der certo:
			free(novo);                     // desaloco o nó porque não vou usá-lo
			return false;                   // como pude alocar o nó, a heap ainda não está cheia
		}
		return true; // se a alocação der errado, a heap está cheia
	}
	return true;
}

bool lista_inserir_inicio(LISTA* lista, void* item){
    if(!lista_cheia(lista)){
        NO* no = (NO*)malloc(sizeof(NO));
        if(no != NULL){
            no->item = item;
            no->prox = lista->inicio;
            lista->inicio = no;

            if(lista->fim == NULL){
                lista->fim = no;
            }

            lista->tam++;
            return true;
        }
    }
    return false;
}

bool lista_inserir_fim(LISTA* lista, void* item){
    if(!lista_cheia(lista)){
        NO* n = (NO*)malloc(sizeof(NO));
        if(n != NULL){
            n->item = item;
            n->prox = NULL;
            if(lista_vazia(lista)){     
                lista->inicio = n;    // Se a lista está vazia, o novo nó é também o início da lista
            }else{
                lista->fim->prox = n; // Se não, o antigo fim aponta para o novo nó
            }
            lista->fim = n;            // O novo nó é o novo fim da lista
            lista->tam++;        
            return true;
        }
    }
    return false;
}

bool lista_inserir_posicao(LISTA* lista, void* item, int n){
    if(lista_cheia(lista) || n < 0 || n > lista->tam){
        return false;
    }
    if(n == 0){
        return lista_inserir_inicio(lista, item);
    }
    if(n == lista->tam){
        return lista_inserir_fim(lista, item);
    }
    NO* no_novo = (NO*)malloc(sizeof(NO));
    if(no_novo == NULL) return false;

    no_novo->item = item;
    NO* no_anterior = lista->inicio;

    for(int i = 0; i < n - 1; i++){
        no_anterior = no_anterior->prox;
    }

    no_novo->prox = no_anterior->prox;
    no_anterior->prox = no_novo;
    lista->tam++;
    
    return true;
}

void lista_remover_inicio(LISTA* lista){
    if(lista == NULL || lista->item_funcoes->item_apagar == NULL){
    	return;
    }
    if(!lista_vazia(lista)){
        void* item = lista->inicio->item; 		 // O ponteiro auxiliar 'item' recebe o endereço do item armazenado na lista

        NO* prox = lista->inicio->prox;    	     // O endereço do próximo nó após o início é armazenado num auxiliar 'prox'
        free(lista->inicio);               		 // A memória alocada pro nó de início é liberada e
        lista->inicio = prox;           	     // o início da lista recebe o endereço armazenado no auxiliar 'prox'
        lista->tam--;

        if(lista->tam == 0){               		 // Caso depois da remoção a lista fique vazia,
            lista->fim = NULL;             		 // o fim da lista também deve receber NULL
        }
        lista->item_funcoes->item_apagar(&item); // Apaga o item
    }
}

void lista_remover_posicao(LISTA* lista, int n){
    if(lista_vazia(lista) || n < 0 || n >= lista->tam || lista->item_funcoes->item_apagar == NULL){
        return;
    }
    if(n == 0){
        lista_remover_inicio(lista);
        return;
    }

    NO* no_anterior = lista->inicio;

    for(int i = 0; i < n - 1; i++){
        no_anterior = no_anterior->prox;
    }
    NO* no_remover = no_anterior->prox;
    void* item = no_remover->item;
    no_anterior->prox = no_remover->prox;

    if(no_anterior->prox == NULL){
        lista->fim = no_anterior;
    }

    free(no_remover);
    lista->tam--;
    lista->item_funcoes->item_apagar(&item); // Apaga o item
}

void lista_remover_fim(LISTA* lista){
    if(lista_vazia(lista)){
        return;
    }
    lista_remover_posicao(lista, lista->tam - 1);
}

void lista_imprimir(LISTA* lista){
    if (lista == NULL || lista->item_funcoes->item_imprimir == NULL) {
        return;
    }
    if (lista_vazia(lista)) {
        return;
    }

    NO* no_atual = lista->inicio;
    while (no_atual != NULL) {
        lista->item_funcoes->item_imprimir(no_atual->item); // Chamada via tabela de funções
        no_atual = no_atual->prox;
    }
}


bool lista_remover_chave(LISTA* lista, void* chave) {
    if (lista == NULL || chave == NULL || lista_vazia(lista) || lista->item_funcoes->item_comparar == NULL || lista->item_funcoes->item_apagar == NULL) {
        return false;
    }

 	// lógica de busca:
    NO* no_atual = lista->inicio;
    NO* no_anterior = NULL;

    while (no_atual != NULL) {
        // Compara o item do nó atual com a chave de busca
        if (lista->item_funcoes->item_comparar(no_atual->item, chave) == 0) { // se encontrou o item:

            if (no_anterior == NULL) {
                lista->inicio = no_atual->prox;
            } else {
                no_anterior->prox = no_atual->prox;
            }

            if (no_atual == lista->fim) {
                lista->fim = no_anterior;
            }

            lista->item_funcoes->item_apagar(&(no_atual->item));
            free(no_atual);
            
            lista->tam--;
            return true; // sucesso
        }

        // se não encontrou, avança os ponteiros para a próxima iteração
        no_anterior = no_atual;
        no_atual = no_atual->prox;
    }

    // se o laço terminar, o item não foi encontrado na lista
    return false;
}

LISTA* lista_buscar(LISTA* lista, void* item){
	if(item == NULL || lista == NULL || lista->item_funcoes->item_comparar == NULL || lista->item_funcoes->item_copiar == NULL){
		return NULL;
	}
	NO* aux = lista->inicio;
	LISTA* resultados = lista_criar(lista->item_funcoes);
	if(resultados==NULL) return NULL;
	while(aux != NULL){
		if(lista->item_funcoes->item_comparar(aux->item, item) == 0){
			void* copia_item = lista->item_funcoes->item_copiar(aux->item);
			lista_inserir_fim(resultados, copia_item);
		}
		aux = aux->prox;
	}
	return resultados;
}

void* lista_buscar_ponteiro(LISTA* lista, void* chave){
    if(lista == NULL || chave == NULL || lista->item_funcoes->item_comparar == NULL){
        return NULL;
    }
    NO* no_atual = lista->inicio;

    while(no_atual != NULL){
        if(lista->item_funcoes->item_comparar(no_atual->item, chave) == 0){
            return no_atual->item; // Se encontrou, retorna o ponteiro para o dado.
        }
        no_atual = no_atual->prox; // Avança para o próximo nó
    }
    // Se o loop terminar, o item não foi encontrado na lista
    return NULL;
}

bool lista_salvar(LISTA* lista, FILE* arquivo){
    if (lista == NULL || arquivo == NULL || lista->item_funcoes->item_salvar == NULL) {
        return false;
    }

    fprintf(arquivo, "======INICIO=LISTA======\n");
    NO* no_atual = lista->inicio;
    while (no_atual != NULL) {
        lista->item_funcoes->item_salvar(no_atual->item, arquivo); // Chamada via tabela de funções
        no_atual = no_atual->prox;
    }
    fprintf(arquivo, "======FINAL=LISTA======\n");
    return true;
}

bool lista_carregar(LISTA** lista, FILE* arquivo){
	if(arquivo == NULL || lista == NULL || *lista == NULL || (*lista)->item_funcoes->item_carregar == NULL){
		return false;
	}
	char linha[100];
    long int ultima_pos = ftell(arquivo);
	fgets(linha, 100, arquivo);  // lê o cabeçalho
	if(strcmp(linha, "======INICIO=LISTA======\n") != 0){  // se não for exatamente a linha esperada, dá erro
        fseek(arquivo, ultima_pos, SEEK_SET);
		return false;
	}

    // Cria lista temporária com as mesmas funções da original
	LISTA* lista_temp = lista_criar((*lista)->item_funcoes); 
    if (lista_temp == NULL) {
        fseek(arquivo, ultima_pos, SEEK_SET);
        return false;
    }
	while(1){
		void* item = NULL; // cria uma variável local que vai receber o endereço do próximo item a ser inserido na lista
		if(lista_temp->item_funcoes->item_carregar(&item, arquivo) == false){
			// lê do arquivo o próximo item da lista e guarda o endereço da struct na variável local.
			// se falhar, sai do loop: pode ser que tenha chegado no rodapé, ou que o arquivo esteja corrompido
			// quando a função carregar_elemento falha, o cursor deve estar no começo da linha que gerou o erro.
			break;
		} 
		lista_inserir_fim(lista_temp, item); // coloca na lista temporária
	}

	fgets(linha, 100, arquivo);                           // lê a linha que parou o loop
	if(strcmp(linha, "======FINAL=LISTA======\n") != 0){  // se não for exatamente o rodapé, dá erro
		lista_apagar(&lista_temp);
		return false;
	}
	// caso dê certo:
	lista_apagar(lista);  // apaga a lista antiga
	*lista = lista_temp;  // substitui pela lista nova
	return true;
}
