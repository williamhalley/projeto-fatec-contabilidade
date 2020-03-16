/*
AUTOR:
WILLIAM HALLEY
*/

#include <stdio.h>
#include <locale.h> //para a função setlocale(LC_ALL, ""); --> corrigir os caracteres da l[ingua portuguesa
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include "estrutura.h"
#include "util.h"
#include "lancamentocontabil.h"
#include "conta.h"
#include "fatos.h"

//Menu de cadastro do lançamento contábil
void menu_lancamento_contabil(){
    int operacao;

    struct tm *data;
    time_t segundos;
    time(&segundos);
    data = localtime(&segundos);

    do{
        printf("\n-----------------------------------------------------------\n");
        printf("\n******************* LANCAMENTO CONTABIL *******************\n");
        printf("\n-----------------------------------------------------------\n");
        printf("\nCarapicuiba, %d/%d/%d\n", data->tm_mday, data->tm_mon+1, data->tm_year+1900);
        printf("\nDigite:\n");
        printf("1 - Debito\n");
        printf("2 - Credito\n");
        printf("3 - Voltar para MENU PRINCIPAL\n");
        scanf("%d", &operacao);

        switch(operacao){
            case 1: lancamento_contabil(operacao); break;
            case 2: lancamento_contabil(operacao); break;
            case 3: return; //break;
            default: printf("\n\nOpcao invalida.\n");
        }
    }while(operacao != 3);
}

//Passa o valor escolhido no menu para a função abaixo que realiza o lançamento
void lancamento_contabil(int operacao){
    FILE *f_contas;
    char nome_conta[51];
    double valor;
    int fato_digitado; //número do fato digitado
    estrutura estr;
    inicializa_estrutura(&estr);

    printf("\nDigite o nome da conta (sem espaco): ");
    scanf(" %50[^\n]", nome_conta);
	
	int i;
	for(i = 0; nome_conta[i] != '\0'; i++){
		toupper(nome_conta[i]);
	}
    //strupr(nome_conta);

    if(verifica_existe_conta(nome_conta) == 0){
        printf("\nConta nao existe.\n");
        return;
    }

    printf("\nDigite o valor para a conta %s: ", nome_conta);
    scanf("%lf", &valor);

    printf("\nDigite o numero do fato: ");
    scanf("%d", &fato_digitado);

    if((f_contas = fopen("contas.txt", "r+b")) == NULL){
        fprintf(stderr, "\nErro de abertura do arquivo.\n");
        return;
    }

    if(operacao == 1){
        while(fread(&estr, sizeof(estrutura), 1, f_contas) > 0){
            if(strcmp(estr.nome_conta_arq, nome_conta) == 0){
                estr.debito[estr.d] = valor;
                estr.total_debito += valor;
                estr.fato_debito[estr.d] = fato_digitado;
                estr.d += 1;
                fseek(f_contas, -sizeof(estrutura), SEEK_CUR);
                fwrite(&estr, sizeof(estrutura), 1, f_contas);
                fclose(f_contas);
                break;
            }
        }
    }

    if(operacao == 2){
        while(fread(&estr, sizeof(estrutura), 1, f_contas) > 0){
            if(strcmp(estr.nome_conta_arq, nome_conta) == 0){
                estr.credito[estr.c] = valor;
                estr.total_credito += valor;
                estr.fato_credito[estr.c] = fato_digitado;
                estr.c += 1;
                fseek(f_contas, -sizeof(estrutura), SEEK_CUR);
                fwrite(&estr, sizeof(estrutura), 1, f_contas);
                fclose(f_contas);
                break;
            }
        }
    }
}//fim da função void lancamento_contabil(int operacao)

//Função que exibe todos os lançamentos cadastrados
void mostrar_lancamento_contabil(){
    FILE *f_contas;
    estrutura estr[TAM_ESTRUTURA];
    int i, j;
    int maior_fato = -1;

    struct tm *data;
    time_t segundos;
    time(&segundos);
    data = localtime(&segundos);

    for(i = 0; i < TAM_ESTRUTURA; i++){
        inicializa_estrutura(&estr[i]);
    }

    if((f_contas = fopen("contas.txt", "rb")) == NULL){
        fprintf(stderr, "Erro ao abrir o arquivo.\n");
    }

    i = 0;
    while(fread(&estr[i], sizeof(estrutura), 1, f_contas) > 0){
        for(j = 0; j < 30; j++){
            if(maior_fato < estr[i].fato_credito[j]){
                maior_fato = estr[i].fato_credito[j];
            }
        }
        i++;
    }
    fclose(f_contas);

    int k = i;
    int p;

    printf("\n\n\n------------------------------------------------------------\n");
    printf("******************** LANCAMENTO DIARIO *********************");
    printf("\n------------------------------------------------------------\n\n");

    for(i = 0; i < maior_fato; i++){
        printf("Carapicuiba, %d/%d/%d\n", data->tm_mday, data->tm_mon+1, data->tm_year+1900);
        printf("Fato %d:\n", i+1);

        for(j = 0; j < k; j++){
            for(p = 0; p < 30; p++){
                if((estr[j].fato_debito[p]) == i+1){
                    printf("DEBITO  | %-30s | %12.2lf | %3d\n", estr[j].nome_conta_arq, estr[j].debito[p], estr[j].fato_debito[p]);
                }
            }
        }

        for(j = 0; j < k; j++){
            for(p = 0; p < 30; p++){
                if((estr[j].fato_credito[p]) == i+1){
                    printf("CREDITO | %-30s | %12.2lf | %3d\n", estr[j].nome_conta_arq, estr[j].credito[p], estr[j].fato_credito[p]);
               }
            }
        }
        printf("\n");
    }
}
