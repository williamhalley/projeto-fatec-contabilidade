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
#include "conta.h"

//CADASTRAR CONTAS (CONTAS PAI E FILHO)
void cadastrar_contas(){
    FILE *f_contas;
    int opcao, opcao_conta;

    char cod[31];
    char nome_conta_digitado[51];
    estrutura estr;
    inicializa_estrutura(&estr);

    do{
        printf("\nEscreva o nome da conta que deseja cadastrar (SEM ESPACO): ");
        scanf(" %50[^\n]", nome_conta_digitado);
		
		int i;
		for(i = 0; nome_conta_digitado[i] != '\0'; i++){
			toupper(nome_conta_digitado[i]);
		}
        //strupr(nome_conta_digitado); //CONVERTE A STRING PARA MAIÚSCULO

        if(verifica_existe_conta(nome_conta_digitado) == 1){
            printf("\n[AVISO]: Conta ja existe no cadastro.\n");
            return;
        }

        strcpy(estr.nome_conta_arq, nome_conta_digitado);

        do{
            printf("\n________________________________________________________________________________\n");
            printf("1 - Cadastrar conta do tipo: 1.01    - ATIVO CIRCULANTE\n");
            printf("2 - Cadastrar conta do tipo: 1.02.01 - ATIVO NAO CIRCULANTE (R.L.P)\n");
            printf("3 - Cadastrar conta do tipo: 1.02.02 - ATIVO NAO CIRCULANTE (INVESTIMENTO)\n");
            printf("4 - Cadastrar conta do tipo: 1.02.03 - ATIVO NAO CIRCULANTE (IMOBILIZADO)\n");
            printf("5 - Cadastrar conta do tipo: 1.02.04 - ATIVO NAO CIRCULANTE (INTANGIVEL)\n");
            printf("6 - Cadastrar conta do tipo: 2.01    - PASSIVO CIRCULANTE\n");
            printf("7 - Cadastrar conta do tipo: 2.02    - PASSIVO NAO CIRCULANTE\n");
            printf("8 - Cadastrar conta do tipo: 2.03    - PATRIMONIO LIQUIDO\n");
            //printf("9 - Voltar ao MENU PRINCIPAL\n");
            printf("________________________________________________________________________________\n");

            scanf("%d", &opcao_conta);

            switch(opcao_conta){
                case 1: strcpy(cod, "1.01"); break;
                case 2: strcpy(cod, "1.02.01"); break;
                case 3: strcpy(cod, "1.02.02"); break;
                case 4: strcpy(cod, "1.02.03"); break;
                case 5: strcpy(cod, "1.02.04"); break;
                case 6: strcpy(cod, "2.01"); break;
                case 7: strcpy(cod, "2.02"); break;
                case 8: strcpy(cod, "2.03"); break;
                //case 9: return; break;
                default: printf("\nOpcao invalida.\n");
            }
        }while(opcao_conta < 1 || opcao_conta > 8);

        definir_codigo(cod);
        strcpy(estr.codigo, cod);

        if((f_contas = fopen("contas.txt", "a+b")) == NULL){
            fprintf(stderr, "\nErro de abertura do arquivo123.\n");
            return;
        }

        fwrite(&estr, sizeof(estrutura), 1, f_contas);
        fclose(f_contas);

        printf("\nDigite:\n");
        printf("1 - Cadastrar outra conta.\n");
        printf("2 - Voltar para MENU PRINCIPAL.\n");
        scanf("%d", &opcao);
        printf("\n");
    }while(opcao != 2);
}

//Exibir contas cadastradas
void mostrar_contas_cadastradas(){
    FILE *f_contas;
    estrutura estr;
    //ordenar contas em ordem alfabética

    if((f_contas = fopen("contas.txt", "rb")) == NULL){
        fprintf(stderr, "Erro ao abrir o arquivo.\n");
		return;
    }

    printf("\n\n\n---------------------------------------------------\n");
    printf("*************** CONTAS CADASTRADAS ****************");
    printf("\n---------------------------------------------------\n");

    while(fread(&estr, sizeof(estrutura), 1, f_contas) > 0){
        printf("%s\n", estr.nome_conta_arq);
    }
    printf("---------------------------------------------------\n\n\n");
}


//Função para validar se a conta existe, se existir retorna 1 senão retorna 0
int verifica_existe_conta(char *nome_conta){
    FILE *f_contas;
    estrutura estr;

    if((f_contas = fopen("contas.txt", "rb")) == NULL){
        fprintf(stderr, "Erro ao abrir o arquivo1.\n");
		return 0;
    }

    while(fread(&estr, sizeof(estrutura), 1, f_contas) > 0){
        if((strncmp(estr.nome_conta_arq, nome_conta, strlen(nome_conta))) == 0){
            //printf("\nAVISO: Conta já existe no cadastro.\n");
            fclose(f_contas);
            return 1;
        }
    }
    fclose(f_contas);
    return 0;
}

//Função para definir o código da conta. Ex: 1.01.02
void definir_codigo(char *cod){
    FILE *f_contas;
    estrutura estr;
    char copia[3];
    int tam = strlen(cod) + 1;
    int num = -1;

    if((f_contas = fopen("contas.txt", "rb")) == NULL){
        fprintf(stderr, "Erro na abertura do arquivo222.\n");
		return;
    }

    while(fread(&estr, sizeof(estrutura), 1, f_contas) > 0){
        if(strncmp(estr.codigo, cod, strlen(cod)) == 0){
            strcpy(copia, &estr.codigo[tam]);
            if(atoi(copia) > num)
                num = atoi(copia);
        }
    }
    num++;
	sprintf(copia, "%d", num);	
    //itoa(num, copia, 10);

    if(num <= 0)
        strcat(cod, ".01");
    else if(num < 10){
        strcat(cod, ".0");
        strcat(cod, copia);
    }else
        strcat(cod, copia);

    //printf("%s -- %d", cod, num);
    fclose(f_contas);
}

