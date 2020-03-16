/*
AUTOR:
WILLIAM HALLEY
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "estrutura.h"
#include "fatos.h"

//Função para cadastrar os fatos
void cadastrar_fatos(){
    FILE *f_fatos;
    int opcao;

    f_fatos = fopen("fatos.txt", "ab");

    if(f_fatos == NULL){
        fprintf(stderr, "\nErro de abertura do aquivo.\n");
        return;
    }

    do{
        char texto[301];
        printf("Digite o texto do fato: ");
        scanf(" %300[^\n]", texto);

        fprintf(f_fatos, "Fato @:\n");
        fprintf(f_fatos, "%s\n\n", texto);

        printf("\nDigite:\n");
        printf("1 - Cadastrar outro fato.\n");
        printf("2 - Voltar ao menu principal.\n");
        scanf("%d", &opcao);
        printf("\n");
    }while(opcao != 2);

    fclose(f_fatos);
}

//Exibe todos os fatos cadastrados
void mostrar_fatos(){
    FILE *f_fatos;
    int i = 1;
    char ch, c[5];

    if((f_fatos = fopen("fatos.txt", "r")) == NULL){
        fprintf(stderr, "Erro de abertura do arquivo.\n");
        return;
    }

    printf("\n\n\n--------------------------------------------------\n");
    printf("*************** FATOS CADASTRADOS ****************");
    printf("\n--------------------------------------------------\n\n");
    while((ch = fgetc(f_fatos)) != EOF){
        if(ch == '@'){
            printf("%d", i);
            i++;
            continue;
        }
        fputc(ch, stdout);
    }

    printf("--------------------------------------------------\n\n\n\n");
    fclose(f_fatos);
}
