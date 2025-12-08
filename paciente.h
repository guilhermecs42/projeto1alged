/**
 * @file paciente.h
 * @brief Interface para o TAD Paciente.
 * * Gerencia os dados de um paciente, incluindo ID, Nome e Histórico de Tratamentos.
 */

#ifndef PACIENTE_H
#define PACIENTE_H

#include <stdbool.h>
#include <stdio.h>
#include "tabela_funcoes.h"
#include "tratamento.h"

/**
 * @brief Estrutura opaca representando um Paciente.
 */
typedef struct paciente PACIENTE;

/**
 * @brief Cria um novo paciente.
 * * @param[in] id Identificador único do paciente.
 * @param[in] nome String com o nome do paciente.
 * @return PACIENTE* Ponteiro para o novo paciente.
 */
PACIENTE* paciente_criar(const int id, const char nome[]);

/**
 * @brief Destrói o paciente e libera seu histórico de tratamentos.
 * * @param[in,out] paciente Endereço do ponteiro para o paciente.
 * @return true Sucesso.
 */
bool paciente_apagar(void** paciente);

/**
 * @brief Verifica se o histórico de tratamentos atingiu o limite.
 * * @param[in] paciente Ponteiro para o paciente.
 * @return true Se cheio.
 */
bool paciente_historico_cheio(PACIENTE* paciente);

/**
 * @brief Verifica se o histórico de tratamentos está vazio.
 * * @param[in] paciente Ponteiro para o paciente.
 * @return true Se vazio.
 */
bool paciente_historico_vazio(PACIENTE* paciente);

/**
 * @brief Adiciona um novo tratamento ao histórico do paciente.
 * * @param[in,out] paciente Ponteiro para o paciente.
 * @param[in] tratamento Ponteiro para o tratamento a ser adicionado.
 * @return true Sucesso.
 * @return false Falha (histórico cheio ou erro de alocação).
 */
bool paciente_adicionar_tratamento(PACIENTE* paciente, TRATAMENTO* tratamento);

/**
 * @brief Remove o tratamento mais recente do histórico.
 * * @param[in,out] paciente Ponteiro para o paciente.
 * @return true Sucesso.
 * @return false Se o histórico já estiver vazio.
 */
bool paciente_remover_tratamento(PACIENTE* paciente);

/**
 * @brief Imprime os dados básicos do paciente (ID e Nome).
 * * @param[in] paciente Ponteiro genérico (void*) para o paciente.
 * @return true Sucesso.
 */
bool paciente_imprimir(void* paciente);

/**
 * @brief Retorna o ID do paciente.
 * * @param[in] paciente Ponteiro para o paciente.
 * @return int O ID do paciente.
 */
int paciente_get_id(PACIENTE* paciente);

/**
 * @brief Imprime todo o histórico de tratamentos do paciente.
 * * @param[in] paciente Ponteiro para o paciente.
 * @return true Sucesso.
 */
bool paciente_consultar_historico(PACIENTE* paciente);

/**
 * @brief Compara dois pacientes (usado para ordenação ou busca).
 * * @param[in] paciente1 Primeiro paciente.
 * @param[in] paciente2 Segundo paciente.
 * @return int 0 se iguais, valor negativo se p1 < p2, positivo se p1 > p2.
 */
int paciente_comparar(void* paciente1, void* paciente2);

/**
 * @brief Salva os dados do paciente e seu histórico em arquivo.
 * * @param[in] paciente Ponteiro genérico para o paciente.
 * @param[in,out] arquivo Ponteiro para arquivo de escrita.
 * @return true Sucesso.
 * @return false Se houver erro de I/O ou parâmetros nulos.
 */
bool paciente_salvar(void* paciente, FILE* arquivo);

/**
 * @brief Carrega um paciente e seu histórico do arquivo.
 * * @param[in,out] paciente Endereço do ponteiro para o paciente (será alocado).
 * @param[in,out] arquivo Ponteiro para arquivo de leitura.
 * @return true Sucesso.
 * @return false Se houver erro de leitura ou formato inválido.
 */
bool paciente_carregar(void** paciente, FILE* arquivo);

/**
 * @brief Cria uma cópia profunda de um paciente.
 * * @param[in] paciente Paciente original.
 * @return void* Ponteiro para a cópia.
 */
void* paciente_copiar(void* paciente);

/**
 * @brief Retorna a tabela de funções para manipulação de Pacientes.
 * * @return const TabelaFuncoes* Ponteiro para a struct estática de funções.
 */
const TabelaFuncoes* get_paciente_funcoes();

#endif