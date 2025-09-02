#include <stdlib.h>
#include <stdbool.h>

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
};

FILA *fila_criar(void){

    FILA* f;
    f = (FILA*)malloc(sizeof(FILA));

    if(f != NULL){
        f->inicio = NULL;
        f->fim = NULL;
        f->tam = 0;
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

//reciclei do pilhadinamica.c
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

// Retorna o primeiro item da fila, sem removê-lo
void *fila_frente(FILA *fila){
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

void* fila_remover(FILA* fila){
    
    if(!fila_vazia(fila)){

        void* item = fila->inicio->item;  // O ponteiro auxiliar 'item' recebe o endereço do item armazenado na fila

        NO* prox = fila->inicio->prox;    // O endereço do próximo nó após o início é armazenado num auxiliar 'prox'
        free(fila->inicio);               // A memória alocada pro nó de início é liberada e
        fila->inicio = prox;              // o início da fila recebe o endereço armazenado no auxiliar 'prox'
        
        fila->tam--;

        if(fila->tam == 0){               // Caso depois da remoção a fila fique vazia,
            fila->fim = NULL;             // o fim da fila também deve receber NULL
        }
        return item;                      // Retorna o endereço do item 
    }
    return NULL;
}

void fila_apagar(FILA** fila){

    if(fila !=  NULL && *fila != NULL){

        NO* no_atual = (*fila)->inicio;
        NO* no_aux;

        while (no_atual != NULL) {
            no_aux = no_atual->prox; // Guarda o próximo nó
            free(no_atual);          // Libera o nó atual
            no_atual = no_aux;       // Avança para o próximo
        }

        free(*fila);
        *fila = NULL; 
    }
    return;
}
