#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "tabela_funcoes.h"

typedef struct no NO;
typedef struct avl_ AVL;

struct no{
    void* item;
    NO* esquerda;
    NO* direita;
    int altura;
};

struct avl_{
    NO* raiz;
    int tam;
    const TabelaFuncoes* item_funcoes;
};

// Funções auxiliares
static int max(int a, int b){
    return (a > b) ? a : b;
}

static int no_altura(NO* no){
    return (no == NULL) ? 0 : no->altura;
}

static int no_fator_balanceamento(NO* no){
    if(no == NULL) return 0;
    return no_altura(no->esquerda) - no_altura(no->direita);
}

static void no_atualizar_altura(NO* no){
    if(no != NULL){
        no->altura = 1 + max(no_altura(no->esquerda), no_altura(no->direita));
    }
}

// Rotações
static NO* rotacao_direita(NO* y){
    NO* x = y->esquerda;
    NO* T2 = x->direita;
    
    x->direita = y;
    y->esquerda = T2;
    
    no_atualizar_altura(y);
    no_atualizar_altura(x);
    
    return x;
}

static NO* rotacao_esquerda(NO* x){
    NO* y = x->direita;
    NO* T2 = y->esquerda;
    
    y->esquerda = x;
    x->direita = T2;
    
    no_atualizar_altura(x);
    no_atualizar_altura(y);
    
    return y;
}

// Balanceamento
static NO* balancear(NO* no){
    if(no == NULL) return NULL;
    
    no_atualizar_altura(no);
    int fb = no_fator_balanceamento(no);
    
    // Caso Esquerda-Esquerda
    if(fb > 1 && no_fator_balanceamento(no->esquerda) >= 0){
        return rotacao_direita(no);
    }
    
    // Caso Esquerda-Direita
    if(fb > 1 && no_fator_balanceamento(no->esquerda) < 0){
        no->esquerda = rotacao_esquerda(no->esquerda);
        return rotacao_direita(no);
    }
    
    // Caso Direita-Direita
    if(fb < -1 && no_fator_balanceamento(no->direita) <= 0){
        return rotacao_esquerda(no);
    }
    
    // Caso Direita-Esquerda
    if(fb < -1 && no_fator_balanceamento(no->direita) > 0){
        no->direita = rotacao_direita(no->direita);
        return rotacao_esquerda(no);
    }
    
    return no;
}

// Criar novo nó
static NO* no_criar(void* item){
    NO* no = (NO*)malloc(sizeof(NO));
    if(no != NULL){
        no->item = item;
        no->esquerda = NULL;
        no->direita = NULL;
        no->altura = 1;
    }
    return no;
}

// Inserção recursiva
static NO* no_inserir(NO* raiz, void* item, const TabelaFuncoes* funcoes, bool* sucesso){
    if(raiz == NULL){
        *sucesso = true;
        return no_criar(item);
    }
    
    int cmp = funcoes->item_comparar(item, raiz->item);
    
    if(cmp < 0){
        raiz->esquerda = no_inserir(raiz->esquerda, item, funcoes, sucesso);
    }else if(cmp > 0){
        raiz->direita = no_inserir(raiz->direita, item, funcoes, sucesso);
    }else{
        *sucesso = false; // Item já existe
        return raiz;
    }
    
    return balancear(raiz);
}

// Buscar nó com menor valor
static NO* no_minimo(NO* no){
    NO* atual = no;
    while(atual->esquerda != NULL){
        atual = atual->esquerda;
    }
    return atual;
}

// Remoção recursiva
static NO* no_remover(NO* raiz, void* chave, const TabelaFuncoes* funcoes, bool* sucesso){
    if(raiz == NULL){
        *sucesso = false;
        return NULL;
    }
    
    int cmp = funcoes->item_comparar(chave, raiz->item);
    
    if(cmp < 0){
        raiz->esquerda = no_remover(raiz->esquerda, chave, funcoes, sucesso);
    }else if(cmp > 0){
        raiz->direita = no_remover(raiz->direita, chave, funcoes, sucesso);
    }else{
        // Nó encontrado
        *sucesso = true;
        
        // Caso 1: Nó folha ou com um filho
        if(raiz->esquerda == NULL || raiz->direita == NULL){
            NO* temp = raiz->esquerda ? raiz->esquerda : raiz->direita;
            
            if(temp == NULL){ // Nó folha
                temp = raiz;
                raiz = NULL;
            }else{ // Um filho
                *raiz = *temp; // Copia o conteúdo
            }
            
            funcoes->item_apagar(&(temp->item));
            free(temp);
        }else{
            // Caso 2: Nó com dois filhos
            NO* temp = no_minimo(raiz->direita);
            
            // Troca o item (mas não apaga ainda)
            void* temp_item = raiz->item;
            raiz->item = temp->item;
            temp->item = temp_item;
            
            // Remove o sucessor
            raiz->direita = no_remover(raiz->direita, temp->item, funcoes, sucesso);
        }
    }
    
    if(raiz == NULL) return NULL;
    
    return balancear(raiz);
}

// Busca recursiva
static void* no_buscar(NO* raiz, void* chave, const TabelaFuncoes* funcoes){
    if(raiz == NULL) return NULL;
    
    int cmp = funcoes->item_comparar(chave, raiz->item);
    
    if(cmp == 0){
        return raiz->item;
    }else if(cmp < 0){
        return no_buscar(raiz->esquerda, chave, funcoes);
    }else{
        return no_buscar(raiz->direita, chave, funcoes);
    }
}

// Impressão em ordem
static void no_imprimir_em_ordem(NO* raiz, const TabelaFuncoes* funcoes){
    if(raiz != NULL){
        no_imprimir_em_ordem(raiz->esquerda, funcoes);
        funcoes->item_imprimir(raiz->item);
        no_imprimir_em_ordem(raiz->direita, funcoes);
    }
}

// Apagar todos os nós
static void no_apagar_recursivo(NO* raiz, const TabelaFuncoes* funcoes){
    if(raiz != NULL){
        no_apagar_recursivo(raiz->esquerda, funcoes);
        no_apagar_recursivo(raiz->direita, funcoes);
        funcoes->item_apagar(&(raiz->item));
        free(raiz);
    }
}

// Salvar em pré-ordem
static bool no_salvar(NO* raiz, FILE* arquivo, const TabelaFuncoes* funcoes){
    if(raiz == NULL){
        fprintf(arquivo, "#\n"); // Marcador de nó NULL
        return true;
    }
    
    if(!funcoes->item_salvar(raiz->item, arquivo)) return false;
    if(!no_salvar(raiz->esquerda, arquivo, funcoes)) return false;
    if(!no_salvar(raiz->direita, arquivo, funcoes)) return false;
    
    return true;
}

// Carregar em pré-ordem
static NO* no_carregar(FILE* arquivo, const TabelaFuncoes* funcoes, bool* sucesso){
    char linha[100];
    long int pos_antes = ftell(arquivo);
    
    if(fgets(linha, 100, arquivo) == NULL){
        *sucesso = false;
        return NULL;
    }
    
    // Verifica se é um nó NULL
    if(strcmp(linha, "#\n") == 0){
        return NULL;
    }
    
    // Volta para ler o item
    fseek(arquivo, pos_antes, SEEK_SET);
    
    void* item;
    if(!funcoes->item_carregar(&item, arquivo)){
        *sucesso = false;
        return NULL;
    }
    
    NO* no = no_criar(item);
    if(no == NULL){
        funcoes->item_apagar(&item);
        *sucesso = false;
        return NULL;
    }
    
    no->esquerda = no_carregar(arquivo, funcoes, sucesso);
    if(!(*sucesso)){
        funcoes->item_apagar(&(no->item));
        free(no);
        return NULL;
    }
    
    no->direita = no_carregar(arquivo, funcoes, sucesso);
    if(!(*sucesso)){
        no_apagar_recursivo(no->esquerda, funcoes);
        funcoes->item_apagar(&(no->item));
        free(no);
        return NULL;
    }
    
    no_atualizar_altura(no);
    return no;
}

// Calcular tamanho
static int no_contar(NO* raiz){
    if(raiz == NULL) return 0;
    return 1 + no_contar(raiz->esquerda) + no_contar(raiz->direita);
}

// Verificar balanceamento
static bool no_verificar_balanceamento(NO* raiz){
    if(raiz == NULL) return true;
    
    int fb = no_fator_balanceamento(raiz);
    if(fb < -1 || fb > 1) return false;
    
    return no_verificar_balanceamento(raiz->esquerda) && 
           no_verificar_balanceamento(raiz->direita);
}

// FUNÇÕES PÚBLICAS




AVL* avl_criar(const TabelaFuncoes* funcoes){
    if(funcoes == NULL) return NULL;
    
    AVL* avl = (AVL*)malloc(sizeof(AVL));
    if(avl != NULL){
        avl->raiz = NULL;
        avl->tam = 0;
        avl->item_funcoes = funcoes;
    }
    return avl;
}

void avl_apagar(AVL** avl){
    if(avl != NULL && *avl != NULL){
        no_apagar_recursivo((*avl)->raiz, (*avl)->item_funcoes);
        free(*avl);
        *avl = NULL;
    }
}

int avl_tamanho(AVL* avl){
    return (avl != NULL) ? avl->tam : -1;
}

bool avl_vazia(AVL* avl){
    if(avl == NULL) return true;
    return (avl->tam == 0);
}

bool avl_inserir(AVL* avl, void* item){
    if(avl == NULL || item == NULL) return false;
    
    bool sucesso = false;
    avl->raiz = no_inserir(avl->raiz, item, avl->item_funcoes, &sucesso);
    
    if(sucesso){
        avl->tam++;
    }
    
    return sucesso;
}

bool avl_remover(AVL* avl, void* chave){
    if(avl == NULL || chave == NULL) return false;
    
    bool sucesso = false;
    avl->raiz = no_remover(avl->raiz, chave, avl->item_funcoes, &sucesso);
    
    if(sucesso){
        avl->tam--;
    }
    
    return sucesso;
}

void* avl_buscar(AVL* avl, void* chave){
    if(avl == NULL || chave == NULL) return NULL;
    return no_buscar(avl->raiz, chave, avl->item_funcoes);
}

void avl_imprimir(AVL* avl){
    if(avl == NULL || avl->item_funcoes->item_imprimir == NULL){
        return;
    }
    
    if(avl_vazia(avl)){
        printf("Árvore vazia.\n");
        return;
    }
    
    no_imprimir_em_ordem(avl->raiz, avl->item_funcoes);
}

bool avl_salvar(AVL* avl, FILE* arquivo){
    if(avl == NULL || arquivo == NULL || avl->item_funcoes->item_salvar == NULL){
        return false;
    }
    
    fprintf(arquivo, "======INICIO=AVL======\n");
    fprintf(arquivo, "%d\n", avl->tam);
    
    if(!no_salvar(avl->raiz, arquivo, avl->item_funcoes)){
        return false;
    }
    
    fprintf(arquivo, "======FIM=AVL======\n");
    return true;
}

bool avl_carregar(AVL** avl, FILE* arquivo){
    if(avl == NULL || *avl == NULL || arquivo == NULL || 
       (*avl)->item_funcoes->item_carregar == NULL){
        return false;
    }
    
    char linha[100];
    long int pos_inicial = ftell(arquivo);
    
    if(fgets(linha, 100, arquivo) == NULL || 
       strcmp(linha, "======INICIO=AVL======\n") != 0){
        fseek(arquivo, pos_inicial, SEEK_SET);
        return false;
    }
    
    int tam_lido;
    if(fscanf(arquivo, "%d\n", &tam_lido) != 1){
        fseek(arquivo, pos_inicial, SEEK_SET);
        return false;
    }
    
    AVL* avl_temp = avl_criar((*avl)->item_funcoes);
    if(avl_temp == NULL){
        fseek(arquivo, pos_inicial, SEEK_SET);
        return false;
    }
    
    bool sucesso = true;
    avl_temp->raiz = no_carregar(arquivo, avl_temp->item_funcoes, &sucesso);
    
    if(!sucesso){
        avl_apagar(&avl_temp);
        fseek(arquivo, pos_inicial, SEEK_SET);
        return false;
    }
    
    if(fgets(linha, 100, arquivo) == NULL || 
       strcmp(linha, "======FIM=AVL======\n") != 0){
        avl_apagar(&avl_temp);
        fseek(arquivo, pos_inicial, SEEK_SET);
        return false;
    }
    
    avl_temp->tam = no_contar(avl_temp->raiz);
    
    avl_apagar(avl);
    *avl = avl_temp;
    
    return true;
}

int avl_altura(AVL* avl){
    if(avl == NULL || avl->raiz == NULL) return -1;
    return no_altura(avl->raiz) - 1;
}

bool avl_balanceada(AVL* avl){
    if(avl == NULL) return true;
    return no_verificar_balanceamento(avl->raiz);
}