/**
 * @file pilha.h
 * @brief Interface para o TAD Pilha Genérica.
 * * Implementa uma estrutura de dados LIFO (Last In, First Out).
 */

#ifndef PILHA_H
#define PILHA_H

#include <stdio.h>
#include <stdbool.h>
#include "tabela_funcoes.h"

/**
 * @brief Estrutura opaca que representa a Pilha.
 */
typedef struct pilha_ PILHA;

/**
 * @brief Cria uma nova pilha vazia.
 * * @param[in] funcoes Tabela de funções para manipular os itens da pilha.
 * @return PILHA* Ponteiro para a nova pilha.
 */
PILHA* pilha_criar(const TabelaFuncoes* funcoes);

/**
 * @brief Verifica se a pilha está vazia.
 * * @param[in] pilha Ponteiro para a pilha.
 * @return true Se vazia ou NULL.
 */
bool pilha_vazia(PILHA* pilha);

/**
 * @brief Verifica se a pilha está cheia (memória esgotada).
 * * @param[in] pilha Ponteiro para a pilha.
 * @return true Se não houver memória para novos nós.
 */
bool pilha_cheia(PILHA* pilha);

/**
 * @brief Retorna a altura (número de elementos) da pilha.
 * * @param[in] pilha Ponteiro para a pilha.
 * @return int Quantidade de itens.
 */
int pilha_altura(PILHA* pilha);

/**
 * @brief Retorna o item no topo da pilha sem removê-lo (peek).
 * * @param[in] pilha Ponteiro para a pilha.
 * @return void* Ponteiro para o item no topo, ou NULL se vazia.
 */
void* pilha_topo(PILHA* pilha);

/**
 * @brief Adiciona (empilha) um item no topo da pilha.
 * * @param[in,out] pilha Ponteiro para a pilha.
 * @param[in] item Ponteiro para o dado a ser guardado.
 * @return true Sucesso.
 * @return false Falha (pilha cheia ou parâmetros NULL).
 */
bool pilha_empilhar(PILHA* pilha, void* item);

/**
 * @brief Remove (desempilha) o item do topo.
 * * O item removido é liberado da memória via `item_apagar`.
 * * @param[in,out] pilha Ponteiro para a pilha.
 */
void pilha_remover_topo(PILHA* pilha);

/**
 * @brief Destrói a pilha e libera todos os seus elementos.
 * * @param[in,out] pilha_ptr Endereço do ponteiro da pilha (será setado para NULL).
 */
void pilha_apagar(PILHA** pilha_ptr);

/**
 * @brief Imprime o conteúdo da pilha.
 * * @param[in] pilha Ponteiro para a pilha.
 * @return true Sucesso.
 * @return false Falha na impressão de algum elemento.
 */
bool pilha_imprimir(PILHA* pilha);

/**
 * @brief Verifica se um item existe na pilha.
 * * @param[in] pilha Ponteiro para a pilha.
 * @param[in] item Chave de busca.
 * @return true Se encontrado.
 * @return false Se não encontrado.
 */
bool pilha_buscar(PILHA* pilha, void* item);

/**
 * @brief Salva a pilha em um arquivo.
 * * @param[in] pilha Ponteiro para a pilha.
 * @param[in,out] arquivo Arquivo para escrita.
 * @return true Sucesso.
 */
bool pilha_salvar(PILHA* pilha, FILE* arquivo);

/**
 * @brief Carrega a pilha de um arquivo.
 * * @param[in,out] pilha Endereço do ponteiro da pilha.
 * @param[in,out] arquivo Arquivo para leitura.
 * @return true Sucesso.
 */
bool pilha_carregar(PILHA** pilha, FILE* arquivo);

#endif