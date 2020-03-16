/*
AUTOR:
WILLIAM HALLEY
*/

#ifndef _ESTRUTURA_H
#define _ESTRUTURA_H

//ESTRUTURA PARA GUARDAR E LER NO ARQUIVO
typedef struct{
    int d; //índice do debito[30] - guarda o histórico de lançamentos
    int c; //índice do credito[30] - guarda o histórico de lançamentos
    int fato_debito[30]; //indica qual o número do fato do débito no lançamento diário
    int fato_credito[30]; //indica qual o número do fato do débito no lançamento diário
    char codigo[31];
    char nome_conta_arq[51];
    double debito[30];
    double credito[30];
    double total_debito;
    double total_credito;
}estrutura;

#endif // ESTRUTURA_H_INCLUDED
