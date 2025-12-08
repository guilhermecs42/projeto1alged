/**
 * @file tratamento.h
 * @brief Interface para o TAD Tratamento.
 * * Encapsula a descrição de um medicamento ou procedimento médico.
 */

#ifndef TRATAMENTO_H
#define TRATAMENTO_H

#include <stdbool.h>
#include <stdio.h>
#include "tabela_funcoes.h"

/**
 * @brief Estrutura opaca representando um Tratamento.
 */
typedef struct tratamento_ TRATAMENTO;

/**
 * @brief Cria um novo tratamento.
 * * @param[in] medicamento String descrevendo o medicamento ou procedimento.
 * @return TRATAMENTO* Ponteiro para o novo tratamento.
 */
TRATAMENTO* tratamento_criar(const char medicamento[]);

/**
 * @brief Destrói o tratamento e libera memória.
 * * @param[in,out] tratamento Endereço do ponteiro genérico para o tratamento.
 * @return true Sucesso.
 */
bool tratamento_apagar(void** tratamento);

/**
 * @brief Imprime a descrição do tratamento.
 * * @param[in] tratamento Ponteiro genérico para o tratamento.
 * @return true Sucesso.
 */
bool tratamento_imprimir(void* tratamento);

/**
 * @brief Compara dois tratamentos (lexicograficamente).
 * * @param[in] tratamento1 Primeiro tratamento.
 * @param[in] tratamento2 Segundo tratamento.
 * @return int Resultado da comparação de strings (strcmp).
 */
int tratamento_comparar(void* tratamento1, void* tratamento2);

/**
 * @brief Salva o tratamento em um arquivo.
 * * @param[in] tratamento Ponteiro genérico para o tratamento.
 * @param[in,out] arquivo Arquivo de escrita.
 * @return true Sucesso.
 * @return false Se houver erro de I/O ou parâmetros nulos.
 */
bool tratamento_salvar(void* tratamento, FILE* arquivo);

/**
 * @brief Carrega um tratamento do arquivo.
 * * @param[in,out] tratamento Endereço do ponteiro onde o tratamento será alocado.
 * @param[in,out] arquivo Arquivo de leitura.
 * @return true Sucesso.
 * @return false Se houver erro de leitura ou formato inválido.
 */
bool tratamento_carregar(void** tratamento, FILE* arquivo);

/**
 * @brief Cria uma cópia do tratamento.
 * * @param[in] tratamento Tratamento original.
 * @return void* Cópia do tratamento.
 */
void* tratamento_copiar(void* tratamento);

/**
 * @brief Retorna a tabela de funções para manipulação de Tratamentos.
 * * @return const TabelaFuncoes* Ponteiro para a tabela.
 */
const TabelaFuncoes* get_tratamento_funcoes();

#endif