#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include "entrada.h"
#include "heap.h"
#include "pilha.h"
#include "lista.h"
#include "paciente.h"
#define FILA_TAM_MAX 50

typedef struct registro REGISTRO;

struct registro{
	int contagem_id;
	LISTA* lista_de_pacientes;
};

REGISTRO* registro_criar(){
	REGISTRO* registro = (REGISTRO*)malloc(sizeof(REGISTRO));
	if(registro!=NULL){
		registro->lista_de_pacientes = lista_criar(get_paciente_funcoes());
		if(registro->lista_de_pacientes == NULL){
			free(registro);
			return NULL;
		}
		registro->contagem_id=1;
	}
	return registro;
}

void registro_apagar(REGISTRO** registro) {
    if (registro == NULL || *registro == NULL) return;
    
    lista_apagar(&((*registro)->lista_de_pacientes));
    free(*registro);
    *registro = NULL;
}

bool registro_salvar(REGISTRO* registro, FILE* arquivo){
	if(registro == NULL || arquivo == NULL) return false;
	
	fprintf(arquivo, "===INICIO==REGISTRO===\n");
	fprintf(arquivo, "%d\n", registro->contagem_id);
	if(lista_salvar(registro->lista_de_pacientes, arquivo) == false) return false; // caso não consiga imprimir a lista de pacientes, encerra a função e deixa o arquivo em estado inválido
	fprintf(arquivo, "====FIM===REGISTRO====\n");
	return true;
}

bool registro_carregar(REGISTRO** registro, FILE* arquivo) {
    if (registro == NULL || arquivo == NULL) {
        return false;
    }

    long int pos_inicial = ftell(arquivo);
    char buffer[151];

    // Lendo o cabeçalho do registro
    if (fgets(buffer, 151, arquivo) == NULL || strcmp(buffer, "===INICIO==REGISTRO===\n") != 0) {
        fseek(arquivo, pos_inicial, SEEK_SET);
        return false;
    }

    // Lendo a contagem
    int id_lido;
    if (fgets(buffer, 151, arquivo) == NULL || sscanf(buffer, "%d", &id_lido) != 1) {
        fseek(arquivo, pos_inicial, SEEK_SET);
        return false;
    }

    // criando um registro para armazenar os dados lidos
    REGISTRO* registro_temp = registro_criar();
    if (registro_temp == NULL) {
        fseek(arquivo, pos_inicial, SEEK_SET);
        return false;
    }
    registro_temp->contagem_id = id_lido;

    // carregando a lista de pacientes
    if (lista_carregar(&(registro_temp->lista_de_pacientes), arquivo) == false) {
        registro_apagar(&registro_temp); // limpa o registro temporário se não conseguir ler a lista
        fseek(arquivo, pos_inicial, SEEK_SET);
        return false;
    }

    // lendo o rodapé do registro
    if (fgets(buffer, 151, arquivo) == NULL ||
        strcmp(buffer, "====FIM===REGISTRO====\n") != 0) {
        registro_apagar(&registro_temp); // Limpa o registro temporário se não conseguir ler o rodapé
        fseek(arquivo, pos_inicial, SEEK_SET);
        return false;
    }

    // apaga o registro antigo e o substitui pelo novo
    registro_apagar(registro);
    *registro = registro_temp;
    
    return true;
}

// GIOVANNI: AQUI VOCÊ TALVEZ PRECISE IMPLEMENTAR ALGUMAS STRUCTS OU FUNÇÕES RELACIONADAS À FILA DE ESPERA
// PODE SER QUE VOCÊ CONSIGA FAZER TUDO SÓ COM AS FUNÇÕES DO fila.h E paciente.h, SE POSSÍVEL, ÓTIMO.
// EMBORA A FILA GUARDE PACIENTE*, QUE POSSUI UMA PILHA* DE TRATAMENTO*, A FILA EM SI NÃO VAI SE IMPORTAR COM OS TRATAMENTOS, ENTÃO VOCÊ TEM ESSA FACILIDADE NA IMPLEMENTAÇÃO
// MAS JÁ ADIANTO QUE VOCÊ VAI PRECISAR IMPLEMENTAR UMA FUNÇÃO DE BUSCA NO fila.h






// AGORA VAMOS IMPLEMENTAR AS FUNÇÕES DO SISTEMA



bool sistema_sair(REGISTRO** registro_de_pacientes, char* nome_arquivo, HEAP** heap_de_espera, char* arquivo_fila){
	if(registro_de_pacientes == NULL || nome_arquivo == NULL || heap_de_espera == NULL || arquivo_fila == NULL){ // a função foi chamada sem ter um Registro de Pacientes definido, Fila de Espera, nome de arquivo
		registro_apagar(registro_de_pacientes);
		*registro_de_pacientes = NULL;
		heap_apagar(heap_de_espera);
		*heap_de_espera = NULL;
		return true;
	}
	FILE* arquivo = fopen(nome_arquivo, "w");
	if(arquivo == NULL){
		printf("Não foi possível acessar o arquivo de Registro de Pacientes. Não se pode sair do sistema.");
		return false;
	}
	if(registro_salvar(*registro_de_pacientes, arquivo)==false){
		printf("Não foi possível salvar o Registro de Pacientes. Não se pode sair do sistema.");
		return false;
	}
	if(fclose(arquivo)!=0){
		printf("Não foi possível fechar o arquivo de Registro de Pacientes. Não se pode sair do sistema.");
		return false;
	}
	
	FILE* heap_arquivo = fopen(arquivo_fila, "w");
	if(heap_arquivo == NULL){
		printf("Não foi possível acessar o arquivo da Fila de Espera. Não se pode sair do sistema.");
		return false;
	}
	if(heap_salvar(*heap_de_espera, heap_arquivo)==false){
		printf("Não foi possível salvar a Fila de Espera. Não se pode sair do sistema.");
		return false;
	}
	if(fclose(heap_arquivo)!=0){
		printf("Não foi possível fechar o arquivo da Fila de Espera. Não se pode sair do sistema.");
		return false;
	}

	return true;
}

void registrar_paciente(REGISTRO* registro_de_pacientes, HEAP* heap_de_espera){
	char nome[101];
	printf("Digite o nome do paciente: \n");
	if(perguntar_string(nome, 101)==false){
		printf("Erro ao ler o nome do paciente. Encerrando operação.\n");
		return;
	}
	if(strcmp(nome, "0") == 0){
		printf("Cancelando operação.\n");
		return;
	}
	char prioridade;
    char buffer_prioridade[5];

    while(1) {
        printf("Digite o nível de prioridade do paciente: \n");
		printf("\n=== NIVEIS DE PRIORIDADE ===\n");
		printf("[A] Emergência\n");
		printf("[B] Muito Urgente\n");
		printf("[C] Urgente\n");
		printf("[D] Pouco Urgente\n");
		printf("[E] Não Urgente\n");
        
        if(perguntar_string(buffer_prioridade, 5) == false){
            printf("Erro ao ler prioridade. Encerrando operação.\n");
            return;
        }

        if(strcmp(buffer_prioridade, "0") == 0){
            printf("Cancelando operação.\n");
            return;
        }

        if(strlen(buffer_prioridade) == 1){
            prioridade = buffer_prioridade[0]; // Pega o primeiro caractere
            break; // Sai do loop, prioridade válida
        } else {
            printf("Entrada inválida. Digite apenas um único caractere para a prioridade.\n");
        }
    }
	PACIENTE* chave_busca = (PACIENTE*)paciente_criar(-1, nome);
	if(chave_busca == NULL){
		printf("Erro na alocação da chave de busca.\n");
	}
	LISTA* resultado_busca;
	resultado_busca = lista_buscar(registro_de_pacientes->lista_de_pacientes, (void*)chave_busca);
	if(resultado_busca == NULL){
		printf("Erro no valor de retorno da busca.\n");
	}
	paciente_apagar((void**)&chave_busca);
	int id_escolhido=-1;
	if(!lista_vazia(resultado_busca)){
		printf("Já existem pacientes registrados com esse mesmo nome. Aqui estão eles: \n");
		lista_imprimir(resultado_busca);
		
		printf("Deseja registrar um novo paciente?\nSe sim, digite '1'. Caso queira apenas adicionar um paciente à Fila de Espera, digite outra coisa:\n");
		if(confirmar() == false){
			printf("Digite o ID do paciente a ser inserido na Fila de Espera.\n");
			if(perguntar_numero(&id_escolhido) == false){
				printf("Erro ao ler o ID do paciente.\n");
				return;
			}
		
			PACIENTE* chave_busca = (PACIENTE*)paciente_criar(id_escolhido, "");
			LISTA* resultado_busca2 = lista_buscar(resultado_busca, chave_busca);
			lista_imprimir(resultado_busca2);
			if(lista_vazia(resultado_busca2)){
				printf("O ID fornecido não corresponde ao nome dado.\n");
				printf("Cancelando operação\n");
				paciente_apagar((void**)&chave_busca);
				lista_apagar(&resultado_busca2);
				lista_apagar(&resultado_busca);
				return;
			}
			lista_apagar(&resultado_busca2);
			paciente_apagar((void**)&chave_busca);
		}
	}
	lista_apagar(&resultado_busca);
	PACIENTE* paciente_inserido;
	if(id_escolhido == -1){ // ISSO AQUI É CASO O PACIENTE NUNCA VISITOU O HOSPITAL ANTES, OU SEJA, NÃO ESTAVA NO REGISTRO
		id_escolhido = registro_de_pacientes->contagem_id;
		paciente_inserido = (PACIENTE*)paciente_criar(id_escolhido, nome);
		if(paciente_inserido == NULL){
			printf("Não foi possível criar um registro do paciente.\n");
			return;
		}
		registro_de_pacientes->contagem_id++;
		if(lista_inserir_fim(registro_de_pacientes->lista_de_pacientes, paciente_inserido)==false){
			printf("A inserção do paciente no Registro de Pacientes falhou.\n");
			paciente_apagar((void**)&paciente_inserido);
		}
		printf("Paciente registrado com ID %d\n", registro_de_pacientes->contagem_id-1);
	}
	
	if(heap_tamanho(heap_de_espera)>=FILA_TAM_MAX){
		printf("A fila de espera está cheia. Não foi possível inserir o paciente nela.\n");
		return;
	}
	
	PACIENTE* paciente_fila = (PACIENTE*)paciente_criar(id_escolhido, nome);
	PACIENTE* chave_busca_fila = (PACIENTE*)paciente_criar(id_escolhido, "");
	PACIENTE* paciente_talvez_na_fila = (PACIENTE*)heap_buscar(heap_de_espera, chave_busca_fila);
	if(paciente_talvez_na_fila == NULL){
		if(heap_inserir(heap_de_espera, paciente_fila, prioridade) == false){
			printf("Erro ao inserir o paciente na Fila de Espera.\n");
			paciente_apagar((void**)&chave_busca_fila);
			paciente_apagar((void**)&paciente_fila);
			return;
		}
		printf("Paciente inserido na fila de espera com sucesso.\n");
		paciente_apagar((void**)&chave_busca_fila);
	}else if (paciente_get_id(paciente_talvez_na_fila) == paciente_get_id(paciente_fila)){
		printf("Paciente já se encontra na fila de espera.\n");
		paciente_apagar((void**)&chave_busca_fila);
		paciente_apagar((void**)&paciente_fila);
	}else{
		if(heap_inserir(heap_de_espera, paciente_fila, prioridade) == false){
			printf("Erro ao inserir o paciente na Fila de Espera.\n");
			paciente_apagar((void**)&paciente_fila);
			paciente_apagar((void**)&chave_busca_fila);
			return;
		}
		printf("Paciente inserido na fila de espera com sucesso.\n");
		paciente_apagar((void**)&chave_busca_fila);
	}
	
	return;
}

void adicionar_procedimento(REGISTRO* registro_de_pacientes){
	int id_lido;
	printf("Qual é o ID do paciente a ser adicionado o procedimento?:\n");
	if(perguntar_numero(&id_lido)==false){
		printf("Não foi possível ler o ID.\n");
		return;
	}
	if(id_lido == 0){
		printf("Cancelando operação.\n");
		return;
	}
	PACIENTE* chave_busca = (PACIENTE*)paciente_criar(id_lido, "");
	if(chave_busca == NULL){
		printf("Erro na alocação da chave de busca.\n");
	}
	PACIENTE* paciente_encontrado = (PACIENTE*)lista_buscar_ponteiro(registro_de_pacientes->lista_de_pacientes, (void*)chave_busca);
	paciente_apagar((void**)&chave_busca);
	if(paciente_encontrado == NULL){
		printf("Esse ID não foi encontrado no Registro.\n");
		return;
	}
    if(paciente_historico_cheio(paciente_encontrado)){
        printf("O histórico de procedimentos do paciente está cheio.\n");
        return;
    }
    char nome_medicamento[101];
    printf("Digite o nome do medicamento/procedimento:\n");
    if(!perguntar_string(nome_medicamento, 101)){
        printf("Erro ao ler o nome do procedimento.\n");
        return;
    }
    if(strcmp(nome_medicamento, "0") == 0){
        printf("Operação cancelada pelo usuário.\n");
        return;
    }
    TRATAMENTO* novo_tratamento = tratamento_criar(nome_medicamento);
    if(novo_tratamento == NULL){
        printf("Erro ao criar o registro do tratamento.\n");
        return;
    }
    if(paciente_adicionar_tratamento(paciente_encontrado, novo_tratamento) != false){
        printf("Procedimento adicionado com sucesso!\n");
    }else{
        printf("Falha ao adicionar o procedimento ao histórico do paciente.\n");
        tratamento_apagar((void**)&novo_tratamento);
    }
	return;
}

void desfazer_procedimento(REGISTRO* registro_de_pacientes){
	int id_lido;
	printf("Qual é o ID do paciente a ser removido o procedimento?:\n");
	if(perguntar_numero(&id_lido)==false){
		printf("Não foi possível ler o ID.\n");
		return;
	}
	if(id_lido == 0){
		printf("Cancelando operação.\n");
		return;
	}
	PACIENTE* chave_busca = (PACIENTE*)paciente_criar(id_lido, "");
	if(chave_busca == NULL){
		printf("Erro na alocação da chave de busca.\n");
	}
	PACIENTE* paciente_encontrado = (PACIENTE*)lista_buscar_ponteiro(registro_de_pacientes->lista_de_pacientes, (void*)chave_busca);
	paciente_apagar((void**)&chave_busca);
	if(paciente_encontrado == NULL){
		printf("Esse ID não foi encontrado no Registro.\n");
		return;
	}
    if(paciente_historico_vazio(paciente_encontrado)){
        printf("O histórico de procedimentos do paciente está vazio.\n");
        return;
    }
    if(paciente_remover_tratamento(paciente_encontrado)){
        printf("Último procedimento desfeito com sucesso.\n");
    }else{
        printf("Não foi possível desfazer o procedimento.\n");
    }
	return;
}

void mostrar_historico(REGISTRO* registro_de_pacientes){
	int id_lido;
	printf("Qual é o ID do paciente a ser exibido o histórico?:\n");
	if(perguntar_numero(&id_lido)==false){
		printf("Não foi possível ler o ID.\n");
		return;
	}
	if(id_lido == 0){
		printf("Cancelando operação.\n");
		return;
	}
	PACIENTE* chave_busca = (PACIENTE*)paciente_criar(id_lido, "");
	if(chave_busca == NULL){
		printf("Erro na alocação da chave de busca.\n");
	}
	PACIENTE* paciente_encontrado = (PACIENTE*)lista_buscar_ponteiro(registro_de_pacientes->lista_de_pacientes, (void*)chave_busca);
	paciente_apagar((void**)&chave_busca);
	if(paciente_encontrado == NULL){
		printf("Esse ID não foi encontrado no Registro.\n");
		return;
	}
    if(paciente_historico_vazio(paciente_encontrado)){
        printf("O histórico de procedimentos do paciente está vázio.\n");
        return;
    }
	if(paciente_consultar_historico(paciente_encontrado) == false){
		printf("Erro ao tentar imprimir hitórico do paciente\n");
	}
	return;
}
void paciente_morreu(REGISTRO* registro_de_pacientes, HEAP* heap_de_espera){ // GIOVANNI: ADICIONE O PARÂMETRO FILA* heap_de_espera AQUI
	int id_lido;
	printf("Qual é o ID do paciente falecido?:\n");
	if(perguntar_numero(&id_lido)==false){
		printf("Não foi possível ler o ID.\n");
		return;
	}
	PACIENTE* chave_busca = paciente_criar(id_lido, "");
	if(heap_buscar(heap_de_espera, chave_busca)){ // GUILHERME: SERÁ QUE ESSA FUNÇÃO FILA_BUSCAR ESTÁ CORRETA?
		printf("Paciente está na Fila de Espera. Não é permitido apagá-lo do registro.\n");
		paciente_apagar((void**)&chave_busca);
		return;
	 }
	
	if(lista_remover_chave(registro_de_pacientes->lista_de_pacientes, (void*)chave_busca) == false){
		printf("Esse ID não foi encontrado no Registro.\n");
		paciente_apagar((void**)&chave_busca);
		return;
	}
	paciente_apagar((void**)&chave_busca);
	
	printf("Paciente removido do Registro com sucesso.\n");
	return;
}
	



// EIS A MAIN:




int main(){
	printf("\n\n======PRONTO=SOCORRO=SUS======\n\n");
	printf("Sempre que quiser cancelar uma operação, digite '0'\n");
	printf("Digite o nome do arquivo do Registro de Pacientes. \nDigite '1' se deseja criar um Registro novo durante a operação do sistema:\n");
	REGISTRO* registro_de_pacientes = NULL;
	char nome_arquivo[101];
	FILE* arquivo_aberto = NULL;
	while(1){
		perguntar_string(nome_arquivo, 101);
		//printf("Você digitou: %s\n", nome_arquivo); // linha para debug
		if(strcmp(nome_arquivo, "0")==0){
			printf("Saindo...\n");
			return 0;
		}
		if(strcmp(nome_arquivo, "1")==0){
			printf("Criando novo Registro.\nEm qual arquivo você quer guardá-lo?:\n");
			if(perguntar_string(nome_arquivo, 101)==false){
				printf("Falha ao ler o nome do arquivo. Tente novamente.");
			}
			registro_de_pacientes = registro_criar();
			if(registro_de_pacientes==NULL){
				printf("Falha ao criar o Registro. Tente novamente.");
				continue;
			}
			break;
		}
		// se o input não é nem "0" nem "1":
		arquivo_aberto = fopen(nome_arquivo, "r");
		if(arquivo_aberto == NULL){
			printf("Arquivo não encontrado. Tente novamente.\n");
			continue;
		}else{
			if(registro_carregar(&registro_de_pacientes, arquivo_aberto)==false){
				printf("Arquivo não corresponde a um registro válido. Tente novamente.\n");
			}else{ // conseguiu ler um registro
				if(fclose(arquivo_aberto)!=0){
				printf("Falha ao fechar o arquivo. Tente novamente.\n");
				}
				break; 
			}
		}
	}
	
	printf("Digite o nome do arquivo da Fila de Espera. \nDigite '1' se deseja criar uma Fila de Espera nova durante a operação do sistema:\n");
	HEAP* heap_de_espera = heap_criar(get_paciente_funcoes());
	if(heap_de_espera == NULL){
		printf("Erro ao criar a Fila de Espera\n");
	}
	char arquivo_fila[101];
	FILE* arquivo_aberto_fila = NULL;
	while(1){
		perguntar_string(arquivo_fila, 101);
		
		if(strcmp(arquivo_fila, "0")==0){
			printf("Saindo...\n");
			registro_apagar(&registro_de_pacientes);
        	heap_apagar(&heap_de_espera);
			return 0;
		}
		if(strcmp(arquivo_fila, "1")==0){
			printf("Criando nova Fila de Espera.\nEm qual arquivo você quer guardá-la?:\n");
			if(perguntar_string(arquivo_fila, 101)==false){
				printf("Falha ao ler o nome do arquivo. Tente novamente.");
			}
			break;
		}
		// se o input não é nem "0" nem "1":
		arquivo_aberto_fila = fopen(arquivo_fila, "r");
		if(arquivo_aberto_fila == NULL){
			printf("Arquivo não encontrado. Tente novamente.\n");
			continue;
		}else{
			if(heap_carregar(&heap_de_espera, arquivo_aberto_fila)==false){
				printf("Arquivo não corresponde a um registro válido. Tente novamente.\n");
			}else{ 
				if(fclose(arquivo_aberto_fila)!=0){
				printf("Falha ao fechar o arquivo. Tente novamente.\n");
				}
				break; 
			}
		}
	}
	 
	// ESSE AQUI É O LOOP CENTRAL DO CÓDIGO: É ONDE O USUÁRIO DECIDE A OPERAÇÃO QUE ELE VAI REALIZAR
	
	while(1){
		Comando acao;
		acao = perguntar_comando();
		switch(acao){
			case SAIR: 
				if(sistema_sair(&registro_de_pacientes, nome_arquivo, &heap_de_espera, arquivo_fila)){
					printf("Saindo...\n");
					registro_apagar(&registro_de_pacientes);
        			heap_apagar(&heap_de_espera);
					return 0; 
				}
				break;
			case REGISTRAR_PACIENTE:
				registrar_paciente(registro_de_pacientes, heap_de_espera);
				break;
			case CHAMAR_PACIENTE:
				if(heap_vazia(heap_de_espera)){
					printf("A Fila de Espera está vazia. Não é possível chamar um paciente.\n");
				}
				else{
					printf("Paciente chamado:\n");
					paciente_imprimir((PACIENTE*)heap_proximo(heap_de_espera));
					heap_remover_prox(heap_de_espera);
				}
				break;
			case ADD_PROCEDIMENTO:
				adicionar_procedimento(registro_de_pacientes);
				break;
			case DESFAZER_PROCEDIMENTO:
				desfazer_procedimento(registro_de_pacientes);
				break;
			case MOSTRAR_FILA:
				if(heap_vazia(heap_de_espera)){
					printf("\nA Fila de Espera está vazia.\n");
				}else{
					printf("\nFila de espera (pacientes mais antigos primeiro):\n");
					heap_imprimir(heap_de_espera);
				}	
				break;
			case MOSTRAR_HISTORICO:
				mostrar_historico(registro_de_pacientes);
				break;
			case PACIENTE_MORREU:
				paciente_morreu(registro_de_pacientes, heap_de_espera);
				break;
			case MOSTRAR_REGISTRO:
				printf("Eis o Registro de Pacientes: \n");
				lista_imprimir(registro_de_pacientes->lista_de_pacientes);
			default: break;
		}	
	}
}
