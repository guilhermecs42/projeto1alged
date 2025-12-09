#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "../../include/tad_estruturas/heap.h"
#include "../../include/utils/tabela_funcoes.h"
#define CAPACIDADE_INICIAL 100

typedef struct mheap_ HEAP;
typedef struct no_ NO;

struct no_{
    void* item;
    char priority;
    int ordem;
};

struct mheap_{
    NO** no;
    int tam;
    int capacidade;
    int ordem;
    const TabelaFuncoes* item_funcoes;
};

static void swap(HEAP* heap, int i, int j){
    void* temp = heap->no[i];
    heap->no[i] = heap->no[j];
    heap->no[j] = temp;
}

// Retorna true caso no a > no b
bool no_comparar(NO *no_a, NO *no_b){
    if(no_a->priority < no_b->priority) return true;

    if(no_a->priority == no_b->priority){
        if(no_a->ordem < no_b->ordem) return true;
    }
    return false;
}

int qsort_no_comparar(const void *a, const void *b){
    NO *no_a = *(NO **)a;
    NO *no_b = *(NO **)b;

    if(no_a->priority < no_b->priority) return -1; // A vem antes de B
    if(no_a->priority > no_b->priority) return 1;  // B vem antes de A

    if(no_a->ordem < no_b->ordem) return -1; // A (mais antigo) vem antes
    if(no_a->ordem > no_b->ordem) return 1;  // A (mais novo) vem depois

    return 0; // São iguais
}

static void fix_up(HEAP* heap, int i){
    while(i > 0){
        int pai = (i - 1) / 2;
        // Troca se o item atual for maior que o pai
        if(no_comparar(heap->no[i], heap->no[pai])){
            swap(heap, i, pai);
            i = pai;
        }else{
            break;
        }
    }
}

static void fix_down(HEAP* heap, int i){
    int maior = i;
    int esquerda = 2 * i + 1;
    int direita = 2 * i + 2;

    if(esquerda < heap->tam && no_comparar(heap->no[esquerda], heap->no[direita])){
        maior = esquerda;
    }
    if(direita < heap->tam && no_comparar(heap->no[direita], heap->no[maior])){
        maior = direita;
    }
    if(maior != i){
        swap(heap, i, maior);
        fix_down(heap, maior);
    }
}

static bool aumentar_capacidade(HEAP* heap){
    int nova_capacidade = heap->capacidade * 2;
    NO** novos_nos = (NO**)realloc(heap->no, nova_capacidade * sizeof(void*));
    if(novos_nos != NULL){
        heap->no = novos_nos;
        heap->capacidade = nova_capacidade;
        return true;
    }
    return false;
}

HEAP* heap_criar(const TabelaFuncoes* funcoes){
    if(funcoes == NULL) return NULL;

    HEAP* heap = (HEAP*)malloc(sizeof(HEAP));
    if(heap != NULL){
        heap->no = (NO**)malloc(CAPACIDADE_INICIAL * sizeof(NO*));
        if(heap->no == NULL){
            free(heap);
            return NULL;
        }
        heap->tam = 0;
        heap->ordem = 0;

        heap->capacidade = CAPACIDADE_INICIAL;
        heap->item_funcoes = funcoes;
    }
    return heap;
}

void heap_apagar(HEAP** heap){
    if(heap != NULL && *heap != NULL){
        if((*heap)->item_funcoes != NULL && (*heap)->item_funcoes->item_apagar != NULL){
            for(int i = 0; i < (*heap)->tam; i++){
                (*heap)->item_funcoes->item_apagar(&((*heap)->no[i]->item));
                free((*heap)->no[i]);
            }
        }
        free((*heap)->no);
        free(*heap);
        *heap = NULL;
    }
}

int heap_tamanho(HEAP* heap){
    return (heap != NULL) ? heap->tam : -1;
}

bool heap_vazia(HEAP* heap){
    if (heap == NULL) return true;
    return (heap->tam == 0);
}

void heap_imprimir(HEAP* heap){

    if(heap == NULL || heap_vazia(heap) || heap->item_funcoes->item_imprimir == NULL){
        printf("\nFila de Espera vazia.\n");
        return;
    }
    
	NO** copia = (NO**)malloc(heap->tam*sizeof(NO*)); // cria uma cópia dos nós da heap
    if(copia == NULL){
        printf("Erro de memória ao tentar imprimir a fila.\n");
        return;
    }
    memcpy(copia, heap->no, heap->tam*sizeof(NO*)); // faz as cópias dos nós

    qsort(copia, heap->tam, sizeof(NO *), qsort_no_comparar);

    printf("\n=== FILA DE ESPERA (ORDENADA POR PRIORIDADE) ===\n");
    for(int i = 0; i < heap->tam; i++){
        printf("------------------------------------------------\n");
        printf("Prioridade: %c | Ordem de Chegada: %d\n", copia[i]->priority, copia[i]->ordem);
        heap->item_funcoes->item_imprimir(copia[i]->item);
    }
    printf("------------------------------------------------\n");
    printf("Total na fila: %d\n", heap->tam);
    
    free(copia);
}

bool heap_inserir(HEAP* heap, void* item, char priority){
    if(heap == NULL || item == NULL) return false;

    if(heap->tam == heap->capacidade){
        if(!aumentar_capacidade(heap)){
            return false;
        }
    }
    NO* novo_no = (NO*)malloc(sizeof(NO));
    if(novo_no == NULL) {
        return false; 
    }
    novo_no->item = item;
    novo_no->priority = priority + 'A';
    novo_no->ordem = heap->ordem;
    heap->no[heap->tam] = novo_no;

    fix_up(heap, heap->tam);
    heap->tam++;
    heap->ordem++;
    return true;
}

void* heap_proximo(HEAP* heap){
    if(!heap_vazia(heap)){
        return heap->no[0]->item;
    }
    return NULL;
}

bool heap_remover_prox(HEAP* heap){
    if(heap_vazia(heap) || heap->item_funcoes->item_apagar == NULL){
        return false;
    }
    NO* no = heap->no[0];
    
    // Move o último elemento para a raiz
    heap->no[0] = heap->no[heap->tam - 1];
    heap->tam--;

    if(heap->tam > 0){
        fix_down(heap, 0);
    }
    heap->item_funcoes->item_apagar(&no->item);
    free(no);
    return true;
}

void* heap_buscar(HEAP* heap, void* item){
    if(item == NULL || heap == NULL || heap->item_funcoes->item_comparar == NULL){
        return NULL;
    }
    for(int i = 0; i < heap->tam; i++){
        if(heap->item_funcoes->item_comparar(heap->no[i]->item, item) == 0){
            return heap->no[i]->item;
        }
    }
    return NULL;
}

bool heap_salvar(HEAP* heap, FILE* arquivo){ 
    if(heap == NULL || arquivo == NULL || heap->item_funcoes == NULL || heap->item_funcoes->item_salvar == NULL){
        return false;
    }
    fprintf(arquivo, "======INICIO=HEAP======\n");

    fprintf(arquivo, "%d\n", heap->ordem);

    for(int i = 0; i < heap->tam; i++){
        fprintf(arquivo, "%c;%d\n", heap->no[i]->priority, heap->no[i]->ordem); // salva a prioridade antes do item
        heap->item_funcoes->item_salvar(heap->no[i]->item, arquivo); // salva o item
    }
    fprintf(arquivo, "======FIM=HEAP======\n");
    return true;
}

bool heap_carregar(HEAP** heap, FILE* arquivo){
    if(*heap == NULL || (*heap)->item_funcoes == NULL || (*heap)->item_funcoes->item_carregar == NULL){
        return false;
    }
    
    bool erro = false;
    char linha[100];
    long int ultima_pos = ftell(arquivo);
    
    if(fgets(linha, 100, arquivo) == NULL){
    	fseek(arquivo, ultima_pos, SEEK_SET);
        return false;
    }
    if(strcmp(linha, "======INICIO=HEAP======\n") != 0){
        fseek(arquivo, ultima_pos, SEEK_SET);
        return false;
    }
    HEAP* heap_temp = heap_criar((*heap)->item_funcoes);
    if(heap_temp == NULL){
        fseek(arquivo, ultima_pos, SEEK_SET);
        return false;
    }
    int ordem_global;
    if(fscanf(arquivo, "%d\n", &ordem_global) != 1){
         heap_apagar(&heap_temp);
         fseek(arquivo, ultima_pos, SEEK_SET);
         return false;
    }
    heap_temp->ordem = ordem_global;

    while(1){
        // Verifica se chegou no rodapé antes de tentar ler item
        long int pos_antes_item = ftell(arquivo);
        fgets(linha, 100, arquivo);
        if(strcmp(linha, "======FIM=HEAP======\n") == 0){
            break;
        }
        // Se não for o rodapé, volta para ler os dados do nó
        fseek(arquivo, pos_antes_item, SEEK_SET);

        char prioridade;
        int ordem_no;
        if(fscanf(arquivo, "%c;%d\n", &prioridade, &ordem_no) != 2){
        	erro = true;
            break;
        }

        void* item;
        if(heap_temp->item_funcoes->item_carregar(&item, arquivo) == false){
        	 erro = true;
             break;
        }

        // Inserção manual para preservar a ordem
        if(heap_temp->tam == heap_temp->capacidade){
            if(!aumentar_capacidade(heap_temp)){
                // Se falhar alocação, limpa item e sai
                if(heap_temp->item_funcoes->item_apagar) 
                    heap_temp->item_funcoes->item_apagar(&item);
                erro = true;
                break;
            }
        }

        NO* novo_no = (NO*)malloc(sizeof(NO));
        if(novo_no == NULL){
             if(heap_temp->item_funcoes->item_apagar) 
                heap_temp->item_funcoes->item_apagar(&item);
             erro = true;
             break;
        }

        novo_no->item = item;
        novo_no->priority = prioridade;
        novo_no->ordem = ordem_no; // Usa a ordem salva, não a global atual

        heap_temp->no[heap_temp->tam] = novo_no;
        heap_temp->tam++;
    }
    
    if(erro){
        heap_apagar(&heap_temp);
        fseek(arquivo, ultima_pos, SEEK_SET);
        return false;
    } else {
        heap_apagar(heap);
        *heap = heap_temp;
        return true;
    }
}
