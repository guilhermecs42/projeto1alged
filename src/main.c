#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include "../include/utils/entrada.h"
#include "../include/tad_estruturas/heap.h"
#include "../include/tad_estruturas/pilha.h"
#include "../include/tad_estruturas/lista.h"
#include "../include/tad_elementos/paciente.h"
#include "../include/tad_estruturas/avl.h"
#define FILA_TAM_MAX 50
#define TAM_NOME 95

typedef struct registro REGISTRO;

struct registro{
	int contagem_id;
	AVL* arvore_de_pacientes;
};

REGISTRO* registro_criar(){
	REGISTRO* registro = (REGISTRO*)malloc(sizeof(REGISTRO));
	if(registro!=NULL){
		registro->arvore_de_pacientes = avl_criar(get_paciente_funcoes());
		if(registro->arvore_de_pacientes == NULL){
			free(registro);
			return NULL;
		}
		registro->contagem_id=1;
	}
	return registro;
}

void registro_apagar(REGISTRO** registro) {
    if (registro == NULL || *registro == NULL) return;
    
    avl_apagar(&((*registro)->arvore_de_pacientes));
    free(*registro);
    *registro = NULL;
}

bool registro_salvar(REGISTRO* registro, FILE* arquivo){
	if(registro == NULL || arquivo == NULL) return false;
	
	fprintf(arquivo, "===INICIO==REGISTRO===\n");
	fprintf(arquivo, "%d\n", registro->contagem_id);
	if(avl_salvar(registro->arvore_de_pacientes, arquivo) == false) return false; // caso não consiga imprimir a lista de pacientes, encerra a função e deixa o arquivo em estado inválido
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
    if (avl_carregar(&(registro_temp->arvore_de_pacientes), arquivo) == false) {
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



// AGORA VAMOS IMPLEMENTAR AS FUNÇÕES DO SISTEMA



bool sistema_sair(REGISTRO** registro_de_pacientes, char* nome_arquivo, HEAP** heap_de_espera, char* arquivo_fila){
	if(registro_de_pacientes == NULL || nome_arquivo == NULL || heap_de_espera == NULL || arquivo_fila == NULL){ // a função foi chamada sem ter um Registro de Pacientes definido, Fila de Espera, nome de arquivo
		if(registro_de_pacientes != NULL){
			registro_apagar(registro_de_pacientes);
			*registro_de_pacientes = NULL;
		}
		if(heap_de_espera){
			heap_apagar(heap_de_espera);
			*heap_de_espera = NULL;
		}
		return true;
	}
	FILE* arquivo = fopen(nome_arquivo, "w");
	if(arquivo == NULL){
		printf("Não foi possível acessar o arquivo de Registro de Pacientes. Não se pode sair do sistema.");
		return false;
	}
	if(registro_salvar(*registro_de_pacientes, arquivo)==false){
		printf("Não foi possível salvar o Registro de Pacientes. Não se pode sair do sistema.");
		fclose(arquivo);
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
		fclose(heap_arquivo);
		return false;
	}
	if(fclose(heap_arquivo)!=0){
		printf("Não foi possível fechar o arquivo da Fila de Espera. Não se pode sair do sistema.");
		return false;
	}

	return true;
}

void registrar_paciente(REGISTRO* registro_de_pacientes, HEAP* heap_de_espera){

	// Todos os ponteiros que serão utilizados:
	PACIENTE* paciente_fila = NULL;
	PACIENTE* duplicado_fila = NULL;
	PACIENTE* novo_registro = NULL;
	PACIENTE* chave_busca = NULL;
	PACIENTE* resultado_busca = NULL;
	int id_paciente;
	char nome[TAM_NOME];
	
	chave_busca = (PACIENTE*)paciente_criar(-1, "");
	if(chave_busca == NULL){
		printf("Erro ao alocar a chave de busca.");
		goto fim;
	}
	paciente_fila = (PACIENTE*)paciente_criar(-1, "");
	if(paciente_fila == NULL){
		printf("Erro ao alocar a cópia para a fila.");
		goto fim;
	}
	
	unsigned char opcao;
	while(1){
		opcao = perguntar_inserir();  
	
		if(opcao == 0){ // Cancelando a operação
			printf("Cancelando inserção de paciente...\n");
			goto fim;
		}
		
		if(opcao == 1){ // Paciente já existe
			// Perguntando o ID
			printf("Digite o ID do paciente.\n");
			if(perguntar_numero(&id_paciente) == false){
				printf("Erro ao ler o ID do paciente.\n");
				goto fim;
			}
			
			paciente_set_id(chave_busca, id_paciente);
			resultado_busca = (PACIENTE*)avl_buscar(registro_de_pacientes->arvore_de_pacientes, (void*)chave_busca);
			
			if(resultado_busca == NULL){
				printf("O ID inserido não existe.\n");
				continue;
			}
			
			printf("Paciente encontrado: %s\n", paciente_get_nome(resultado_busca));
			printf("Digite 1 para continuar com o cadastro.\n");
            if(confirmar() == false){
                continue;
            }
			
			// Preparando a inserção na fila:
			paciente_set_id(paciente_fila, id_paciente);
			paciente_set_nome(paciente_fila, paciente_get_nome(resultado_busca));
			break;
		}
		if(opcao == 2){ // Paciente novo
		
			printf("Digite o nome do novo paciente:\n");
			if(perguntar_string(nome, TAM_NOME) == false){
				printf("Erro ao ler o nome.\n");
                goto fim; 
            }
            
            id_paciente = registro_de_pacientes->contagem_id;
            novo_registro = (PACIENTE*)paciente_criar(id_paciente, nome); // criando o novo registro
            if(novo_registro == NULL) goto fim;

            
            if(avl_inserir(registro_de_pacientes->arvore_de_pacientes, novo_registro) == false){ // inserindo na árvore
                printf("Erro ao salvar no registro.\n");
                goto fim;
            }
            
            printf("Paciente inserido no Registro com ID: %d\n", id_paciente);
            registro_de_pacientes->contagem_id++; // incrementa contador de ids
            novo_registro = NULL; // seta para NULL para não apagar
            
            // Prepara o objeto da fila com os dados novos
            paciente_set_id(paciente_fila, id_paciente);
            paciente_set_nome(paciente_fila, nome);
            
            break;
			
		}
	}
	
	// Vamos inserir na fila:
	
	if(heap_tamanho(heap_de_espera)>=FILA_TAM_MAX){
		printf("A fila de espera está cheia. Não foi possível inserir o paciente nela.\n");
		goto fim;
	}	
	
	Prioridade prioridade = perguntar_prioridade();
	if(prioridade == SAIR_PRIORIDADE){
		printf("Cancelando inserção de paciente...\n");
		goto fim;
	}
	
	paciente_set_id(chave_busca, id_paciente); // Realizando a busca para saber se o paciente já está na fila:
    paciente_set_nome(chave_busca, "");
	
	duplicado_fila = (PACIENTE*)heap_buscar(heap_de_espera, chave_busca);
	if(duplicado_fila != NULL){
		printf("Paciente já se encontra na fila de espera.\n");
		goto fim;
	}
	
	if(heap_inserir(heap_de_espera, paciente_fila, prioridade) == false){
		printf("Erro ao inserir o paciente na Fila de Espera.\n");
		goto fim;
	}
	
	printf("Paciente inserido na fila de espera com sucesso.\n");
	paciente_fila = NULL; // muda a variável local para não apagar o paciente
	
	fim:
	if(paciente_fila != NULL) paciente_apagar((void**)&paciente_fila);
	if(novo_registro != NULL) paciente_apagar((void**)&novo_registro);
	if(chave_busca != NULL) paciente_apagar((void**)&chave_busca);
	
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
	PACIENTE* paciente_encontrado = (PACIENTE*)avl_buscar(registro_de_pacientes->arvore_de_pacientes, (void*)chave_busca);
	paciente_apagar((void**)&chave_busca);
	if(paciente_encontrado == NULL){
		printf("Esse ID não foi encontrado no Registro.\n");
		return;
	}
    if(paciente_historico_cheio(paciente_encontrado)){
        printf("O histórico de procedimentos do paciente está cheio.\n");
        return;
    }
    char nome_medicamento[TAM_NOME];
    printf("Digite o nome do medicamento/procedimento:\n");
    if(!perguntar_string(nome_medicamento, TAM_NOME)){
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
	PACIENTE* paciente_encontrado = (PACIENTE*)avl_buscar(registro_de_pacientes->arvore_de_pacientes, (void*)chave_busca);
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
	PACIENTE* paciente_encontrado = (PACIENTE*)avl_buscar(registro_de_pacientes->arvore_de_pacientes, (void*)chave_busca);
	paciente_apagar((void**)&chave_busca);
	if(paciente_encontrado == NULL){
		printf("Esse ID não foi encontrado no Registro.\n");
		return;
	}
    if(paciente_historico_vazio(paciente_encontrado)){
        printf("O histórico de procedimentos do paciente está vazio.\n");
        return;
    }
	if(paciente_consultar_historico(paciente_encontrado) == false){
		printf("Erro ao tentar imprimir hitórico do paciente\n");
	}
	return;
}
void paciente_morreu(REGISTRO* registro_de_pacientes, HEAP* heap_de_espera){
	int id_lido;
	printf("Qual é o ID do paciente falecido?:\n");
	if(perguntar_numero(&id_lido)==false){
		printf("Não foi possível ler o ID.\n");
		return;
	}
	PACIENTE* chave_busca = paciente_criar(id_lido, "");
	if(heap_buscar(heap_de_espera, chave_busca) != NULL){ 
		printf("Paciente está na Fila de Espera. Não é permitido apagá-lo do registro.\n");
		paciente_apagar((void**)&chave_busca);
		return;
	 }
	
	if(avl_remover(registro_de_pacientes->arvore_de_pacientes, (void*)chave_busca) == false){
		printf("Esse ID não foi encontrado no Registro.\n");
		paciente_apagar((void**)&chave_busca);
		return;
	}
	paciente_apagar((void**)&chave_busca);
	
	printf("Paciente removido do Registro com sucesso.\n");
	return;
}

REGISTRO* aux_main_gerar_registro(bool* novo, char* nome_arquivo){
	REGISTRO* registro_de_pacientes = NULL;
	FILE* arquivo_aberto = NULL;
	while(1){
		perguntar_string(nome_arquivo, TAM_NOME);
		//printf("Você digitou: %s\n", nome_arquivo); // linha para debug
		if(strcmp(nome_arquivo, "0")==0){
			return NULL;
		}
		if(strcmp(nome_arquivo, "1")==0){
			printf("Criando novo Registro.\nEm qual arquivo você quer guardá-lo?:\n");
			if(perguntar_string(nome_arquivo, TAM_NOME)==false){
				printf("Falha ao ler o nome do arquivo. Tente novamente.");
				continue;
			}
			registro_de_pacientes = registro_criar();
			if(registro_de_pacientes==NULL){
				printf("Falha ao criar o Registro. Tente novamente.");
				continue;
			}
			
			*novo = true;
			break; // NOVO  REGISTRO CRIADO
		}
		// se o input não é nem "0" nem "1":
		arquivo_aberto = fopen(nome_arquivo, "r");
		if(arquivo_aberto == NULL){
			printf("Arquivo não encontrado. Tente novamente.\n");
			continue;
		}else{
			if(registro_carregar(&registro_de_pacientes, arquivo_aberto)==false){
				printf("Arquivo não corresponde a um registro válido. Tente novamente.\n");
				if(fclose(arquivo_aberto) != 0){ // fecha o arquivo
					printf("Falha ao fechar o arquivo. Tente novamente.\n");
				}
			}else{ // conseguiu ler um registro
				if(fclose(arquivo_aberto)!=0){
					printf("Falha ao fechar o arquivo. Tente novamente.\n");
				}
				
				*novo = false;
				break;  // REGISTRO CARREGADO
			}
		}
	}
	
	return registro_de_pacientes;
}
	



// EIS A MAIN:




int main(){
	REGISTRO* registro_de_pacientes = NULL;
	HEAP* heap_de_espera = NULL;
	
	printf("\n\n======PRONTO=SOCORRO=SUS======\n\n");
	printf("Sempre que quiser cancelar uma operação, digite '0'\n");
	printf("Digite o nome do arquivo do Registro de Pacientes. \nDigite '1' se deseja criar um Registro novo durante a operação do sistema:\n");
	
	char nome_arquivo[TAM_NOME];
	bool novo;
	registro_de_pacientes = aux_main_gerar_registro(&novo, nome_arquivo);
	if(registro_de_pacientes == NULL){
		printf("Saindo...");
		registro_apagar(&registro_de_pacientes);
        heap_apagar(&heap_de_espera);
		return 0;
	}
	
	// agora vamos criar a fila:
	
	heap_de_espera = heap_criar(get_paciente_funcoes());
	if(heap_de_espera == NULL){
		printf("Erro ao criar a Fila de Espera\n");
	}
	FILE* arquivo_aberto_fila = NULL;
		
	char arquivo_fila[TAM_NOME];
	if(novo == false){
		printf("Digite o nome do arquivo da Fila de Espera. \nDigite '1' se deseja criar uma Fila de Espera nova durante a operação do sistema:\n");
		
		while(1){
			perguntar_string(arquivo_fila, TAM_NOME);
			
			if(strcmp(arquivo_fila, "0")==0){
				printf("Saindo...\n");
				registro_apagar(&registro_de_pacientes);
		    	heap_apagar(&heap_de_espera);
				return 0;
			}
			if(strcmp(arquivo_fila, "1")==0){
				printf("Criando nova Fila de Espera.\nEm qual arquivo você quer guardá-la?:\n");
				if(perguntar_string(arquivo_fila, TAM_NOME)==false){
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
	}else{
		printf("Criando nova Fila de Espera.\nEm qual arquivo você quer guardá-la?:\n");
		while(1) { // Adicione um loop para garantir que ele digite um nome válido
		    if(perguntar_string(arquivo_fila, TAM_NOME) == true){
		        break;
		    }
		    printf("Falha ao ler o nome. Tente novamente.\n");
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
				if(avl_vazia(registro_de_pacientes->arvore_de_pacientes)){
					printf("Registro vazio.\n");
				}else{
					printf("Eis o Registro de Pacientes: \n");
					avl_imprimir(registro_de_pacientes->arvore_de_pacientes);
				}
			default: break;
		}	
	}
}
