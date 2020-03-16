/*
AUTOR:
WILLIAM HALLEY
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "estrutura.h"
#include "util.h"

//Fun��o util para zerar os arquivos
void limpar_arquivos(){
    FILE *f_limpa;
    fclose(fopen("contas.txt", "wb")); //arquivo bin�rio
    fclose(fopen("fatos.txt", "w")); //arquivo texto
}

//Fun��o para inicializar a estrutura definida em estrutura.h
void inicializa_estrutura(estrutura *estr){
    int i;
    (*estr).d = 0;
    (*estr).c = 0;
    (*estr).codigo[0] = '0';
    strcpy((*estr).nome_conta_arq, "Vazio");
    (*estr).total_debito = 0.00;
    (*estr).total_credito = 0.00;

    for(i = 0; i < 30; i++){
        (*estr).debito[i] = -1.00;
        (*estr).credito[i] = -1.00;
        (*estr).fato_debito[i] = -1; //indica qual o n�mero do fato do d�bito no lan�amento di�rio
        (*estr).fato_credito[i] = -1; //indica qual o n�mero do fato do d�bito no lan�amento di�rio
    }
}
