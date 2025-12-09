#ifndef HEAP_H
	#define HEAP_H
	#include <stdio.h>
	#include <stdbool.h>
    #include "../utils/tabela_funcoes.h"
	
	typedef struct mheap_ HEAP;
	
    HEAP* heap_criar(const TabelaFuncoes* funcoes);
    void heap_apagar(HEAP** heap);
    int heap_tamanho(HEAP* heap);
    bool heap_vazia(HEAP* heap);
    void heap_imprimir(HEAP* heap);
    bool heap_inserir(HEAP* heap, void* item, char priority);
    void* heap_proximo(HEAP* heap);
    bool heap_remover_prox(HEAP* heap);
    void* heap_buscar(HEAP* heap, void* item);
    bool heap_salvar(HEAP* heap, FILE* arquivo);
    bool heap_carregar(HEAP** heap, FILE* arquivo);

#endif
