#include<stdlib.h>
#include<stdbool.h>

typedef struct pilha PILHA;
typedef struct no NO;

struct no{
	void* item; // item que este nó contém
	NO* anterior; // ponteiro para o nó anterior
};

struct pilha{
	NO* topo; // é um ponteiro para o elemento no topo da pilha
	int altura;
};



PILHA* pilha_criar(){
	PILHA* pilha = (PILHA*)malloc(sizeof(PILHA));
	if(pilha != NULL){
		pilha->topo = NULL;
		pilha->altura = 0;
	}
	return pilha;
}

bool pilha_vazia(PILHA* pilha){
	if(pilha != NULL){
		return (pilha->altura == 0) ? true : false;
	}
	return true;
}

bool pilha_cheia(PILHA* pilha){ // verifica se há espaço disponível na heap
	if(pilha != NULL){
		NO* novo = (NO*)malloc(sizeof(NO)); // tenta alocar espaço para mais um nó na heap
		if(novo != NULL){ // se a alocação der certo:
			free(novo); // desaloco o nó porque não vou usá-lo
			return false; // como pude alocar o nó, a heap ainda não está cheia
		}
		return true; // se a alocação der errado, a heap está cheia
	}
	return true;
}

int pilha_altura(PILHA* pilha){
	return (pilha!=NULL) ? pilha->altura : -1;
}

void* pilha_topo(PILHA* pilha){
	if(!pilha_vazia(pilha)){
		return pilha->topo->item;
	}
	return NULL;
}

bool pilha_empilhar(PILHA* pilha, void* item){
	if(!pilha_cheia(pilha)){
		NO* pnovo = (NO*)malloc(sizeof(NO));
		if(pnovo != NULL){
			pnovo->item = item;
			pnovo->anterior = pilha->topo;
			pilha->topo = pnovo;
			pilha->altura++;
			return true;
		}
	}
	return false;
}

void* pilha_desempilhar(PILHA* pilha){
	if(!pilha_vazia(pilha)){
		/* meu codigo
		NO* temp = pilha->topo->anterior;
		pilha->topo->anterior = NULL;
		void* retorno = pilha->topo->item;
		pilha->topo = temp;
		return retorno;
		*/
		void* retorno = pilha->topo->item;
		NO* topo_antigo = pilha->topo;
		pilha->topo = pilha->topo->anterior;
		topo_antigo->anterior = NULL; // por que essa linha é necessária? eu vou dar free no topo_antigo de qualquer jeito
		free(topo_antigo);
		topo_antigo = NULL;
		return retorno;
	}
	return NULL;
}




