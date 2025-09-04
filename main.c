#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include "entrada.h"
#include "fila.h"
#include "pilhadinamica.h"

void sistema_sair(){
	printf("Saindo...\n");
	return;
}

void teste(){
	printf("Comando válido!!\n");
	return;
}

int main(){

	Comando acao;
	acao = perguntar_comando();
	switch(acao){
		case SAIR: 
			sistema_sair(); 
			return 0;
			break;
		default: teste(); break;
	}
	
}
