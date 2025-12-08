/**
 * @file heap.h
 * @brief Interface para o TAD Max Heap.
 * * Este arquivo define a interface para manipulação de uma fila de prioridade
 * implementada como uma Max Heap binária.
 */

#ifndef HEAP_H
#define HEAP_H

#include <stdio.h>
#include <stdbool.h>
#include "tabela_funcoes.h"

/**
 * @brief Estrutura opaca que representa a Heap.
 * * Os detalhes da implementação estão ocultos no arquivo .c.
 */
typedef struct mheap_ HEAP;

/**
 * @brief Cria uma nova instância de Heap.
 * * Aloca memória para a estrutura da heap e inicializa seus campos.
 * * @param[in] funcoes Ponteiro para a tabela de funções (comparar, liberar, imprimir)
 * que será usada para manipular os itens genéricos (void*).
 * @return HEAP* Ponteiro para a nova heap criada, ou NULL se houver erro de alocação.
 */
HEAP* heap_criar(const TabelaFuncoes* funcoes);

/**
 * @brief Destrói a heap e libera toda a memória associada.
 * * Libera a memória dos nós, dos itens (usando a função de apagar da tabela)
 * e da estrutura da heap. O ponteiro original é setado para NULL para evitar dangling pointers.
 * * @param[in,out] heap Endereço do ponteiro para a heap. O ponteiro será atualizado para NULL.
 */
void heap_apagar(HEAP** heap);

/**
 * @brief Retorna a quantidade de elementos na heap.
 * * @param[in] heap Ponteiro para a heap.
 * @return int Número de itens armazenados, ou -1 se o ponteiro for NULL.
 */
int heap_tamanho(HEAP* heap);

/**
 * @brief Verifica se a heap está vazia.
 * * @param[in] heap Ponteiro para a heap.
 * @return true Se a heap estiver vazia ou for NULL.
 * @return false Se houver pelo menos um item.
 */
bool heap_vazia(HEAP* heap);

/**
 * @brief Imprime o conteúdo da heap.
 * * Esta função utiliza a função de impressão fornecida na criação da heap.
 * A ordem de impressão segue a ordem de prioridade.
 * Pacientes com a mesma prioridade devem ser listados de acordo com a ordem de chegada.
 * * @param[in] heap Ponteiro para a heap.
 */
void heap_imprimir(HEAP* heap);

/**
 * @brief Insere um novo item na heap com uma prioridade associada.
 * * A função realiza o "sift-up" (subida) para manter a propriedade de Max Heap.
 * * @param[in,out] heap Ponteiro para a heap.
 * @param[in] item Ponteiro genérico para o dado a ser armazenado. A heap não faz cópia, apenas guarda o ponteiro.
 * @param[in] priority Caractere representando a prioridade (ex: 'A' para não urgente, 'E' para emergência).
 * @return true Se a inserção for bem-sucedida.
 * @return false Se houver erro de alocação ou parâmetros inválidos.
 */
bool heap_inserir(HEAP* heap, void* item, char priority);

/**
 * @brief Recupera o item de maior prioridade (raiz) sem removê-lo.
 * * @param[in] heap Ponteiro para a heap.
 * @return void* Ponteiro para o item no topo, ou NULL se a heap estiver vazia.
 */
void* heap_proximo(HEAP* heap);

/**
 * @brief Remove o item de maior prioridade da heap.
 * * Remove a raiz, move o último elemento para o topo e realiza o "sift-down".
 * O item removido é liberado da memória usando a função de apagar definida na criação.
 * * @param[in,out] heap Ponteiro para a heap.
 * @return true Se a remoção for bem-sucedida.
 * @return false Se a heap estiver vazia ou for NULL.
 */
bool heap_remover_prox(HEAP* heap);

/**
 * @brief Busca um item específico na heap.
 * * @param[in] heap Ponteiro para a heap.
 * @param[in] item Ponteiro contendo a chave de busca (comparação feita pela TabelaFuncoes).
 * @return void* Ponteiro para o item encontrado na heap, ou NULL se não encontrado.
 */
void* heap_buscar(HEAP* heap, void* item);

/**
 * @brief Salva o estado atual da heap em um arquivo.
 * * Serializa a estrutura da heap e seus itens.
 * * @param[in] heap Ponteiro para a heap.
 * @param[in,out] arquivo Ponteiro para um arquivo já aberto em modo de escrita.
 * @return true Se a escrita for bem-sucedida.
 * @return false Se houver erro de I/O ou parâmetros nulos.
 */
bool heap_salvar(HEAP* heap, FILE* arquivo);

/**
 * @brief Carrega uma heap a partir de um arquivo.
 * * Lê os dados do arquivo e reconstrói a estrutura da heap em memória.
 * Se já houver uma heap no ponteiro, ela será apagada antes do carregamento.
 * * @param[in,out] heap Endereço do ponteiro para a heap (será alocada/substituída).
 * @param[in,out] arquivo Ponteiro para um arquivo já aberto em modo de leitura.
 * @return true Se o carregamento for bem-sucedido.
 * @return false Se houver erro de leitura ou formato inválido.
 */
bool heap_carregar(HEAP** heap, FILE* arquivo);

#endif