#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include "entrada.h"
#include "fila.h"
#include "pilha.h"
#include "lista.h"
#include "paciente.h"

//CLEYTON
//GIOVANNI
//O NOSSO SISTEMA TEM UMA FEATURE PARA SER MAIS AMIGÁVEL COM O USUÁRIO: QUANDO O USUÁRIO DIGITAR '0' EM ALGUM MOMENTO, É COMO SE ELE ESTIVESSE PEDINDO PARA CANCELAR A OPERAÇÃO.
// NÃO PRECISA IMPLEMENTAR ESSA FEATURE EM TODOS OS INPUTS QUE VOCÊS IMPLEMENTAREM, MAS TENTEM PELO MENOS ALGUMAS VEZES
// OLHEM BEM AS FUNÇÕES DE entrada.h PARA ENTENDER COMO VOCÊS VÃO RECEBER OS INPUTS!!!
// PROGRAMEM NA MÃO E CORRIJAM COM GPT!!! NÃO DEIXEM PASSAR VAZAMENTOS DE MEMÓRIA!!!
// NESSA SEÇÃO DO CÓDIGO VAMOS DEFINIR O REGISTRO DE PACIENTES E ALGUMAS DE SUAS OPERAÇÕES



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
		registro->contagem_id=0;
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



bool sistema_sair(REGISTRO** registro_de_pacientes, char* nome_arquivo){ // GIOVANNI: INCLUIR A FILA DE ESPERA NOS PARÂMETROS
	if(registro_de_pacientes == NULL || nome_arquivo == NULL){ // a função foi chamada sem ter um registro de pacientes definido ou um nome de arquivo
		registro_apagar(registro_de_pacientes);
		*registro_de_pacientes;
		return true;
	}
	FILE* arquivo = fopen(nome_arquivo, "w");
	if(arquivo == NULL){
		printf("Não foi possível acessar o arquivo de registro de pacientes. Não se pode sair do sistema.");
		return false;
	}
	if(registro_salvar(*registro_de_pacientes, arquivo)==false){
		printf("Não foi possível salvar o registro de pacientes. Não se pode sair do sistema.");
		return false;
	}
	if(fclose(arquivo)!=0){
		printf("Não foi possível fechar o arquivo de registro de pacientes. Não se pode sair do sistema.");
		return false;
	}
	
	// GIOVANNI: AQUI VOCÊ VAI PRECISAR PERGUNTAR O NOME DE UMA ARQUIVO PARA SALVAR A FILA DE ESPERA E IMPLEMENTAR A FUNÇÃO DE SALVAR!!!

	return true;
}

void registrar_paciente(REGISTRO* registro_de_pacientes){
	char nome[101];
	printf("Digite o nome do paciente: \n");
	if(perguntar_string(nome, 101)==false){
		printf("Erro ao ler o nome do paciente. Encerrando operação.\n");
		return;
	}
	if(nome == "0"){
		printf("Cancelando operação.\n");
		return;
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
		
		printf("Deseja registrar um novo paciente?\nSe sim, digite '1'. Caso contrário, digite outra coisa:\n");
		if(confirmar() == false){
			printf("Deseja inserir um dos pacientes encontrados na lista de espera?\nSe sim, digite '1'. Caso contrário, digite outra coisa:\n");
			if(confirmar() == true){
				printf("Digite o ID do paciente:\n");
				if(perguntar_numero(&id_escolhido) == false){
					lista_apagar(&resultado_busca);
					printf("Erro ao ler o ID do paciente.\n");
					return;
				}
				PACIENTE* chave_busca = (PACIENTE*)paciente_criar(id_escolhido, "");
				if(lista_buscar(resultado_busca, chave_busca)==NULL){
					printf("O ID fornecido não corresponde ao nome dado.\n");
					printf("Cancelando operação\n");
					paciente_apagar((void**)&chave_busca);
					lista_apagar(&resultado_busca);
					return;
				}
				paciente_apagar((void**)&chave_busca);
			}else{
				lista_apagar(&resultado_busca);
				printf("Cancelando operação.\n");
				return;
			}
		}
	}
	lista_apagar(&resultado_busca);
	PACIENTE* paciente_inserido;
	if(id_escolhido == -1){ // ISSO AQUI É CASO O PACIENTE NUNCA VISITOU O HOSPITAL ANTES, OU SEJA, NÃO ESTAVA NO REGISTRO
		paciente_inserido = (PACIENTE*)paciente_criar(registro_de_pacientes->contagem_id, nome);
		registro_de_pacientes->contagem_id++;
		if(lista_inserir_fim(registro_de_pacientes->lista_de_pacientes, paciente_inserido)==false){
			printf("A inserção do paciente no Registro de Pacientes falhou.");
		}
		printf("Paciente registrado com ID %d\n", registro_de_pacientes->contagem_id-1);
	}else{ // ISSO AQUI É CASO O PACIENTE JÁ TENHA VISITADO O HOSPITAL ANTES
		paciente_inserido = (PACIENTE*)paciente_criar(id_escolhido, nome);
	}
	
	// GIOVANNI: ABAIXO DESSE COMENTÁRIO VOCÊ SÓ PRECISA INSERIR O PACIENTE paciente_inserido NA FILA DE ESPERA
	// ACHO QUE SERIA LEGAL VERIFICAR SE O PACIENTE JÁ ESTÁ NA FILA DE ESPERA, CASO SIM, NÃO ADICIONAR ELE DE NOVO
	
	
	return;
}

void paciente_morreu(REGISTRO* registro_de_pacientes){ // GIOVANNI: ADICIONE O PARÂMETRO FILA* fila_de_espera AQUI
	int id_lido;
	printf("Qual é o ID do paciente falecido?:\n");
	if(perguntar_numero(&id_lido)==false){
		printf("Não foi possível ler o ID.\n");
		return;
	}
	
	// GIOVANNI: DEVE TER UM IF PARA VERIFICAR SE O PACIENTE ESTÁ NA FILA DE ESPERA. SERÁ NECESSÁRIO IMPLEMENTAR "fila_buscar". 
	// SE O PACIENTE ESTIVER NA FILA, ELE NÃO PODE MORRER
	
	PACIENTE* chave_busca = paciente_criar(id_lido, "");
	if(lista_remover_chave(registro_de_pacientes->lista_de_pacientes, (void*)chave_busca) == false){
		printf("Esse ID não foi encontrado no Registro.");
		return;
	}
	printf("Paciente removido do Registro com sucesso.");
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
	 
	// ESSE AQUI É O LOOP CENTRAL DO CÓDIGO: É ONDE O USUÁRIO DECIDE A OPERAÇÃO QUE ELE VAI REALIZAR
	
	while(1){
		printf("Eis o registro de pacientes (para debug): \n");
		lista_imprimir(registro_de_pacientes->lista_de_pacientes);
		Comando acao;
		acao = perguntar_comando();
		switch(acao){
			case SAIR: 
				if(sistema_sair(&registro_de_pacientes, nome_arquivo)){
					printf("Saindo...\n");
					return 0; 
				}
				break;
			case REGISTRAR_PACIENTE:
				registrar_paciente(registro_de_pacientes);
				break;
			case CHAMAR_PACIENTE: // GIOVANNI: VOCÊ DEVE IMPLEMENTAR ESSA FUNÇÃO. O PACIENTE DEVE SER RETIRADO SOMENTE DA FILA! LEMBRE DE FAZER AS VERIFICAÇÕES NECESSÁRIAS
				break;
			case ADD_PROCEDIMENTO: // CLEYTON: VOCÊ DEVE IMPLEMENTAR ESSA FUNÇÃO. PERGUNTE O ID DO PACIENTE, O NOME DO TRATAMENTO E USE A FUNÇÃO paciente_adicionar_tratamento. USE AS FUNÇÕES DE entrada.h PARA OBTER OS INPUTS DE FORMA SEGURA.
			// CLEYTON: POR FAVOR, TESTE SE A FEATURE DE SALVAR O REGISTRO E CARREGAR ELE AINDA FUNCIONA APÓS IMPLEMENTAR OS TRATAMENTOS. OLHE DENTRO DO ARQUIVO!!! SE ALGUMA COISA QUEBRAR, DEBUGUE O SEU CÓDIGO PARA VER SE TEM ALGO ERRADO. SE AINDA NÃO ESTIVER FUNCIONANDO, ME AVISE!!!
				break;
			case DESFAZER_PROCEDIMENTO: // CLEYTON: OS MESMOS AVISOS QUE A FUNÇÃO DE CIMA. TESTE O SALVAMENTO DO REGISTRO!!! OLHE DENTRO DO ARQUIVO!!!
				break;
			case MOSTRAR_FILA: // GIOVANNI: MOSTRE A FILA DE PACIENTES, MAS NÃO MOSTRE O HISTÓRICO (PILHA DE TRATAMENTOS) DE CADA UM! SÓ NOME E ID! NÃO DEVE SER DIFÍCIL!
				break;
			case MOSTRAR_HISTORICO: // CLEYTON: PERGUNTE UM ID, E DEVOLVA O HISTÓRICO DO PACIENTE. USE A FUNÇÃO paciente_consultar_historico. NÃO DEVE SER DIFÍCIL!
				break;
			case PACIENTE_MORREU:
				paciente_morreu(registro_de_pacientes); // GIOVANNI: AQUI PRECISA ALTERAR, É NECESSÁRIO PASSAR A FILA COMO ARGUMENTO TAMBÉM PARA VERIFICAR SE O PACIENTE ESTÁ NA FILA DE ESPERA
				break;
			default: break;
		}	
	}
}
