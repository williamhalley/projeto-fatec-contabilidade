/*
AUTOR:
WILLIAM HALLEY
*/

#ifndef _ESTRUTURA_H
#define _ESTRUTURA_H

//ESTRUTURA PARA GUARDAR E LER NO ARQUIVO
typedef struct{
    int d; //�ndice do debito[30] - guarda o hist�rico de lan�amentos
    int c; //�ndice do credito[30] - guarda o hist�rico de lan�amentos
    int fato_debito[30]; //indica qual o n�mero do fato do d�bito no lan�amento di�rio
    int fato_credito[30]; //indica qual o n�mero do fato do d�bito no lan�amento di�rio
    char codigo[31];
    char nome_conta_arq[51];
    double debito[30];
    double credito[30];
    double total_debito;
    double total_credito;
}estrutura;

#endif // ESTRUTURA_H_INCLUDED
