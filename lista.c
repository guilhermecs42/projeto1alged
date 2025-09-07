#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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
    void (*apagar_elemento)(void* elemento);
    void (*imprimir_elemento)(void* elemento);
    void (*comparar_elemento)(void* item1, void* item2);
	void (*salvar_elemento)(void* elemento, FILE* arquivo);
	bool (*carregar_elemento)(void** elemento, FILE* arquivo);
};

LISTA* lista_criar(void (*item_apagar)(void* item), void (*item_imprimir)(void* item), void (*item_comparar)(void* item1, void* item2), void (*item_salvar)(void* item, FILE* arquivo), bool (*item_carregar)(void** item, FILE* arquivo)){

    LISTA* lista;
    lista = (LISTA*)malloc(sizeof(LISTA));

    if(lista != NULL){
        lista->inicio = NULL;
        lista->fim = NULL;
        lista->tam = 0;
        lista->apagar_elemento = item_apagar;
        lista->imprimir_elemento = item_imprimir;
        lista->comparar_elemento = item_comparar;
		lista->salvar_elemento = item_salvar;
		lista->carregar_elemento = item_carregar;
    }

    return lista;
}

void lista_apagar(LISTA** lista){

    if(lista !=  NULL && *lista != NULL){

        NO* no_atual = (*lista)->inicio;
        NO* no_aux;

        while (no_atual != NULL) {
            no_aux = no_atual->prox;                   // Guarda o próximo nó
            (*lista)->apagar_elemento(no_atual->item); // Apaga o elemento armazenado no nó
            free(no_atual);                            // Libera o nó atual
            no_atual = no_aux;                         // Avança para o próximo
        }

        free(*lista);
        *lista = NULL; 
    }
    return;
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
        return false;
    }
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

/*
bool lista_inserir_posicao(LISTA* lista, void* item, int n){

    if(!lista_cheia(lista) && n <= lista->tam){

        int i = 0;
        NO* no_novo = (NO*)malloc(sizeof(NO));
        
        if(no_novo != NULL){
            NO* no_aux = lista->inicio;

            while(i < n){
                no_aux = no_aux->prox;
                i++;
                if(no_aux == NULL){
                    return NULL;
                }
            }
            no_novo->prox = no_aux->prox;
            no_novo->item = item;
            lista->tam++;

            return true;
        }
    }
    return false;
}
*/

//versao da funcao corrigida pelo gemini
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
    int i = 0;
    while(i < n - 1){
        no_anterior = no_anterior->prox;
        i++;
    }

    no_novo->prox = no_anterior->prox;
    no_anterior->prox = no_novo;
    lista->tam++;

    return true;
}

void* lista_remover_inicio(LISTA* lista){
    
    if(!lista_vazia(lista)){

        void* item = lista->inicio->item;  // O ponteiro auxiliar 'item' recebe o endereço do item armazenado na lista

        NO* prox = lista->inicio->prox;    // O endereço do próximo nó após o início é armazenado num auxiliar 'prox'
        free(lista->inicio);               // A memória alocada pro nó de início é liberada e
        lista->inicio = prox;              // o início da lista recebe o endereço armazenado no auxiliar 'prox'
        
        lista->tam--;

        if(lista->tam == 0){               // Caso depois da remoção a lista fique vazia,
            lista->fim = NULL;             // o fim da lista também deve receber NULL
        }
        return item;                       // Retorna o endereço do item
    }
    return NULL;
}

void* lista_remover_posicao(LISTA* lista, int n){

    if(n == 0){
            return lista_remover_inicio(lista);
        }

    if(!lista_vazia(lista) && n < lista->tam && n >= 0){

        int i = 0;

        NO* no = lista->inicio;
        NO* no_anterior = NULL;
        void* item;

        while(i < n){
            no_anterior = no;
            no = no->prox;
            i++;
            if(no == NULL){
                return NULL;
            }
        }
        no_anterior->prox = no->prox;
        item = no->item;
        free(no);
        no = NULL;

        lista->tam--;
        
        if(no_anterior->prox == NULL){
            lista->fim = no_anterior;
        }

        return item;
    }
    return NULL;
}

void* lista_remover_fim(LISTA* lista){
    return lista_remover_posicao(lista, lista->tam - 1);
}

void lista_imprimir(LISTA* lista){

    if (lista_vazia(lista)) {
        printf("Lista vazia ou nula.\n");
        return;
    }

    NO* no_atual = lista->inicio;

    printf("======INICIO=LISTA======\n");
    while (no_atual != NULL) {
        lista->imprimir_elemento(no_atual->item);
        no_atual = no_atual->prox;
    }
    printf("======FINAL=LISTA======\n");
}

bool lista_salvar(LISTA* lista, FILE* arquivo){

    if (lista_vazia(lista)) {
        printf("Lista vazia ou nula.\n");
        return false;
    }

    if(arquivo == NULL){
        printf("Erro ao abrir arquivo.\n");
        return false;
    }

    NO* no_atual = lista->inicio;

    fprintf(arquivo, "======INICIO=LISTA======\n");
    while (no_atual != NULL) {
        lista->salvar_elemento(no_atual->item, arquivo);
        no_atual = no_atual->prox;
    }

    fprintf(arquivo, "======FINAL=LISTA======\n");

    return true;
}

// Reciclado do pilhadinamica.c
bool lista_carregar(LISTA** lista, FILE* arquivo){
	if(*lista == NULL || (*lista)->carregar_elemento == NULL){
		return false;
	}
	char linha[100];
	LISTA* lista_temp = lista_criar((*lista)->apagar_elemento, (*lista)->imprimir_elemento, (*lista)->salvar_elemento, (*lista)->carregar_elemento);
	fgets(linha, 100, arquivo); // lê o cabeçalho
	if(strcmp(linha, "======INICIO=LISTA======\n")!=0){ // se não for exatamente a linha esperada, dá erro
		lista_apagar(&lista_temp); // apagar a lista temporária
		return false; // retorna erro
	}
	while(1){
		void* item; // cria uma variável local que vai receber o endereço do próximo item a ser inserido na lista
		if(lista_temp->carregar_elemento(&item, arquivo) == false){
			// lê do arquivo o próximo item da lista e guarda o endereço da struct na variável local.
			// se falhar, sai do loop: pode ser que tenha chegado no rodapé, ou que o arquivo esteja corrompido
			// quando a função carregar_elemento falha, o cursor deve estar no começo da linha que gerou o erro.
			break;
		} 
		lista_inserir_fim(lista_temp, item); // coloca na lista temporária
	}
	fgets(linha, 100, arquivo); // lê a linha que parou o loop
	if(strcmp(linha, "======FINAL=LISTA======\n") != 0){ // se não for exatamente o rodapé, dá erro
		lista_apagar(&lista_temp);
		return false;
	}
	// caso dê certo:
	lista_apagar(lista); // apaga a lista antiga
	*lista = lista_temp; // substitui pela lista nova
	return true;
}
