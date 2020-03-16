/*
AUTOR:
WILLIAM HALLEY
*/

#ifndef _CONTA_H
#define _CONTA_H

int verifica_existe_conta(char *); //Função para validar se a conta existe
void cadastrar_contas(); //Cadastrar uma conta
void mostrar_contas_cadastradas(); //mostra somente as contas
void definir_codigo(char *); //Função para definir o código da conta. Ex: 1.01.02

#endif // CONTA_H_INCLUDED
