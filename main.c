/*
AUTOR:
WILLIAM HALLEY
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include "estrutura.h"
#include "util.h"
#include "conta.h"
#include "fatos.h"
#include "lancamentocontabil.h"

//PROTÓTIPOS DAS FUNÇÕES
void mostrar_razonete();
void mostrar_balancete_verificacao();
void mostrar_balanco_patrimonial();


int main(){
   int opcao = 0;

    //MENU PRINCIPAL
    do{
        printf("\n----------------------------------------------------\n");
        printf("********************** MENU ***********************|\n");
        printf("----------------------------------------------------\n");
        printf(" 1 - Cadastrar contas (CAIXA, CAPITAL SOCIAL, ETC) |\n");
        printf(" 2 - Cadastrar fatos                               |\n");
        printf(" 3 - Cadastrar lancamento contabil                 |\n");
        printf(" 4 - Mostrar contas cadastradas                    |\n");
        printf(" 5 - Mostrar fatos                                 |\n");
        printf(" 6 - Mostrar lancamento contabil                   |\n");
        printf(" 7 - Mostrar razonete                              |\n");
        printf(" 8 - Mostrar balancete de verificacao              |\n");
        printf(" 9 - Mostrar balanco patrimonial                   |\n");
        printf("10 - Apagar arquivos                               |\n");
        printf("11 - Finalizar programa                            |\n");
        printf("----------------------------------------------------\n");

        scanf("%d", &opcao);

        switch(opcao){
            case 1: cadastrar_contas(); break;
            case 2: cadastrar_fatos(); break;
            case 3: menu_lancamento_contabil(); break;
            case 4: mostrar_contas_cadastradas(); break; //mostrar somente contas
            case 5: mostrar_fatos(); break;
            case 6: mostrar_lancamento_contabil(); break;
            case 7: mostrar_razonete(); break;
            case 8: mostrar_balancete_verificacao(); break;
            case 9: mostrar_balanco_patrimonial(); break;
            case 10: limpar_arquivos(); break;
            case 11: return 0; //encerrar o programa
            default: printf("Opcao invalida.\n");
        }
    }while(opcao != 11);

    return 0;
}

//Função que exibe os razonetes que são gerados a partir das contas cadastradas
void mostrar_razonete(){
    FILE *f_contas;
    estrutura estr;
    int i;

    if((f_contas = fopen("contas.txt", "rb")) == NULL){
        fprintf(stderr, "Erro ao abrir o arquivo.\n");
    }

    printf("\n\n ----------------------------------------\n");
    printf(" *************** RAZONETES **************\n");
    printf(" ----------------------------------------\n");
    while(fread(&estr, sizeof(estrutura), 1, f_contas) > 0){

        printf("\n\n %s\n", estr.nome_conta_arq);
        printf(" ________________________________________\n");
        i = 0;
        for(i = 0; i < 30; i++){

            if(estr.debito[i] == -1.00 && estr.credito[i] == -1.00){
                break;
            }

            if(estr.debito[i] != -1.00 && estr.credito[i] != -1.00){
                printf("%14.2lf [%2d] | %14.2lf [%2d]\n", estr.debito[i], estr.fato_debito[i], estr.credito[i], estr.fato_credito[i]);
                continue;
            }

            if(estr.debito[i] != -1.00 && estr.credito[i] == -1.00){
                printf("%14.2lf [%2d] |                   -\n", estr.debito[i], estr.fato_debito[i]);
                continue;
            }

            if(estr.debito[i] == -1.00 && estr.credito[i] != -1.00){
                printf("                  - | %14.2lf [%2d]\n", estr.credito[i], estr.fato_credito[i]);
            }
        }
        printf(" ________________________________________\n");
        if(estr.total_debito >= estr.total_credito)
            printf("     %14.2lf |        *******\n", estr.total_debito - estr.total_credito);
        else
            printf("            ******* | %14.2lf\n", estr.total_credito - estr.total_debito);
        printf(" ________________________________________\n");
    }
    printf("\n\n");
    fclose(f_contas);
}

//Função para exibir o balancete de verificação de acordo com as contas cadastras e os saldos das mesmas
void mostrar_balancete_verificacao(){
    FILE *f_contas;
    char ch;
    double total_geral_debito = 0.00, total_geral_credito = 0.00;
    estrutura estr;
    inicializa_estrutura(&estr);

    if((f_contas = fopen("contas.txt", "rb")) == NULL){
        fprintf(stderr, "Erro de abertura do arquivo.\n");
        return;
    }

    printf("\n\n\n  -----------------------------------------------------------------------------------------------\n");
    printf(" |********************************** BALANCETE DE VERIFICACAO ***********************************|");
    printf("\n |-----------------------------------------------------------------------------------------------|\n");
    printf(" | CODIGO              | NOMA DA CONTA                            | SALDO DEVEDOR | SALDO CREDOR |\n");
    printf(" |-----------------------------------------------------------------------------------------------|\n");

    while((fread(&estr, sizeof(estrutura), 1, f_contas)) > 0){
        if(estr.total_debito >= estr.total_credito){
            printf(" |%-20s | %-40s | %13.2lf |            - |\n", estr.codigo, estr.nome_conta_arq, estr.total_debito - estr.total_credito);
            total_geral_debito += (estr.total_debito - estr.total_credito);
        }else{
            printf(" |%-20s | %-40s |             - | %12.2lf |\n", estr.codigo, estr.nome_conta_arq, estr.total_credito - estr.total_debito);
            total_geral_credito += (estr.total_credito - estr.total_debito);
        }
    }
    printf(" |-----------------------------------------------------------------------------------------------|\n");
    printf(" |                     | TOTAL                                    | %13.2lf | %12.2lf |\n", total_geral_debito, total_geral_credito);
    printf("  -----------------------------------------------------------------------------------------------\n\n\n");

    fclose(f_contas);
}

//Função para exibir o balanço patrimonial que é feito com base nas contas e nos seus saldos de débito e créditos
void mostrar_balanco_patrimonial(){
    FILE *f_contas;
    int i, j;
    estrutura estr[TAM_ESTRUTURA], aux;

    for(i = 0; i < TAM_ESTRUTURA; i++){
        inicializa_estrutura(&estr[i]);
    }

    if((f_contas = fopen("contas.txt", "rb")) == NULL){
        fprintf(stderr, "Erro ao abrir o arquivo.\n");
    }

    i = 0;
    while(fread(&estr[i], sizeof(estrutura), 1, f_contas) > 0){
        i++;
    }
    fclose(f_contas);

    int k = i;
    //ordena por codigo
    for(i = 1; i < k; i++){
        for(j = 1; j < k; j++){
            if((strcmp(estr[j-1].codigo, estr[j].codigo)) > 0){
                aux = estr[j-1];
                estr[j-1] = estr[j];
                estr[j] = aux;
            }
        }
    }

    double soma_ac = 0.00; //soma dos ativos circulante
    double soma_anc_geral = 0.00; //soma dos ativos não circulante
    double soma_anc_rlp = 0.00; //soma dos ativos não circulante R.L.P
    double soma_anc_inv = 0.00; //soma dos ativos não circulante investimento
    double soma_anc_imob = 0.00; //soma dos ativos não circulante imobilizado
    double soma_anc_intang = 0.00; //soma dos ativos não circulante imobilizados intangível
    double soma_pc = 0.00; //soma dos passivos circulante
    double soma_pnc = 0.00; //soma dos passivos não circulante
    double soma_passivos_patrimonio = 0.00; //soma do patrimonio liquido
    double soma_ativos = 0.00; //soma de todos ativos
    double soma_passivos = 0.00; //soma de todos os passivos

    for(i = 0; i < k; i++){
        if(estr[i].total_debito >= estr[i].total_credito){
            if(strncmp(estr[i].codigo, "1.01", 4) == 0){
                soma_ac += estr[i].total_debito - estr[i].total_credito;
            }

            if(strncmp(estr[i].codigo, "1.02.01", 7) == 0){
                soma_anc_rlp += estr[i].total_debito - estr[i].total_credito;
            }

            if(strncmp(estr[i].codigo, "1.02.02", 7) == 0){
                soma_anc_inv += estr[i].total_debito - estr[i].total_credito;
            }

            if(strncmp(estr[i].codigo, "1.02.03", 7) == 0){
                soma_anc_imob += estr[i].total_debito - estr[i].total_credito;
            }

            if(strncmp(estr[i].codigo, "1.02.04", 7) == 0){
                soma_anc_intang += estr[i].total_debito - estr[i].total_credito;
            }

            if(strncmp(estr[i].codigo, "2.01", 4) == 0){
                soma_pc += estr[i].total_debito - estr[i].total_credito;
            }

            if(strncmp(estr[i].codigo, "2.02", 4) == 0){
                soma_pnc += estr[i].total_debito - estr[i].total_credito;
            }

            if(strncmp(estr[i].codigo, "2.03", 4) == 0){
                soma_passivos_patrimonio += estr[i].total_debito - estr[i].total_credito;
            }
        }else{
            if(strncmp(estr[i].codigo, "1.01", 4) == 0){
                soma_ac += estr[i].total_credito - estr[i].total_debito;
            }

            if(strncmp(estr[i].codigo, "1.02.01", 7) == 0){
                soma_anc_rlp += estr[i].total_credito - estr[i].total_debito;
            }

            if(strncmp(estr[i].codigo, "1.02.02", 7) == 0){
                soma_anc_inv += estr[i].total_credito - estr[i].total_debito;
            }

            if(strncmp(estr[i].codigo, "1.02.03", 7) == 0){
                soma_anc_imob += estr[i].total_credito - estr[i].total_debito;
            }

            if(strncmp(estr[i].codigo, "1.02.04", 7) == 0){
                soma_anc_intang += estr[i].total_credito - estr[i].total_debito;
            }

            if(strncmp(estr[i].codigo, "2.01", 4) == 0){
                soma_pc += estr[i].total_credito - estr[i].total_debito;
            }

            if(strncmp(estr[i].codigo, "2.02", 4) == 0){
                soma_pnc += estr[i].total_credito - estr[i].total_debito;
            }

            if(strncmp(estr[i].codigo, "2.03", 4) == 0){
                soma_passivos_patrimonio += estr[i].total_credito - estr[i].total_debito;
            }
        }//fim do else
    }

    soma_anc_geral = soma_anc_rlp + soma_anc_inv + soma_anc_imob + soma_anc_intang;
    soma_ativos += soma_ac + soma_anc_geral,
    soma_passivos += soma_pc + soma_pnc + soma_passivos_patrimonio;

    printf("\n\n\n ------------------------------------------------------------------------\n");
    printf(" ************************* BALANCO PATRIMONIAL **************************\n");
    printf(" ------------------------------------------------------------------------\n");
    printf("\n ------------------------------------------------------------------------\n");
    printf(" 1          | ATIVOS\n");
    printf(" ------------------------------------------------------------------------\n");
    printf(" 1.01       | ATIVO CIRCULANTE                         |     %12.2lf\n", soma_ac);
    printf(" ------------------------------------------------------------------------\n");
    for(i = 0; i < k; i++){
        if(strncmp(estr[i].codigo, "1.01", 4) == 0){
            printf(" %-10s | %-40s |     %12.2lf\n", estr[i].codigo, estr[i].nome_conta_arq, estr[i].total_debito);
        }
    }
    printf(" ------------------------------------------------------------------------\n");
    printf(" 1.02       | ATIVO NAO CIRCULANTE                     |     %12.2lf\n", soma_anc_geral);
    printf(" ------------------------------------------------------------------------\n");
    printf(" 1.02.01    | ATIVO NAO CIRCULANTE - R.L.P             |     %12.2lf\n", soma_anc_rlp);
    printf(" ------------------------------------------------------------------------\n");
    for(i = 0; i < k; i++){
        if(strncmp(estr[i].codigo, "1.02.01", 7) == 0){
            printf(" %-10s | %-40s | %12.2lf\n", estr[i].codigo, estr[i].nome_conta_arq, estr[i].total_debito);
        }
    }
    printf(" ------------------------------------------------------------------------\n");
    printf(" 1.02.02    | ATIVO NAO CIRCULANTE - INVESTIMENTO      |     %12.2lf\n", soma_anc_rlp);
    printf(" ------------------------------------------------------------------------\n");
    for(i = 0; i < k; i++){
        if(strncmp(estr[i].codigo, "1.02.02", 7) == 0){
            soma_ac += estr[i].total_debito;
            printf(" %-10s | %-40s |     %12.2lf\n", estr[i].codigo, estr[i].nome_conta_arq, estr[i].total_debito);
        }
    }
    printf(" ------------------------------------------------------------------------\n");
    printf(" 1.02.03    | ATIVO NAO CIRCULANTE - IMOBILIZADO       |     %12.2lf\n", soma_anc_imob);
    printf(" ------------------------------------------------------------------------\n");
    for(i = 0; i < k; i++){
        if(strncmp(estr[i].codigo, "1.02.03", 7) == 0){
            printf(" %-10s | %-40s |     %12.2lf\n", estr[i].codigo, estr[i].nome_conta_arq, estr[i].total_debito);
        }
    }
    printf(" ------------------------------------------------------------------------\n");
    printf(" 1.02.04    | ATIVO NAO CIRCULANTE - INTANGIVEL        |     %12.2lf\n", soma_anc_intang);
    printf(" ------------------------------------------------------------------------\n");
    for(i = 0; i < k; i++){
        if(strncmp(estr[i].codigo, "1.02.04", 7) == 0){
            printf(" %-10s | %-40s | %12.2lf\n", estr[i].codigo, estr[i].nome_conta_arq, estr[i].total_debito);
        }
    }
    printf(" ------------------------------------------------------------------------\n");
    printf("              TOTAL - ATIVOS                           |     %12.2lf\n", soma_ativos);
    printf(" ------------------------------------------------------------------------\n");
    printf("\n ************************************************************************\n");

    //PASSIVOS
    printf("\n ------------------------------------------------------------------------\n");
    printf(" 2          | PASSIVOS\n");
    printf(" ------------------------------------------------------------------------\n");
    printf(" 2.01       | PASSIVO CIRCULANTE                       |     %12.2lf\n", soma_pc);
    printf(" ------------------------------------------------------------------------\n");
    for(i = 0; i < k; i++){
        if(strncmp(estr[i].codigo, "2.01", 4) == 0){
            printf(" %-10s | %-40s |     %12.2lf\n", estr[i].codigo, estr[i].nome_conta_arq, estr[i].total_debito);
        }
    }
    printf(" ------------------------------------------------------------------------\n");
    printf(" 2.02       | PASSIVO NAO CIRCULANTE                   |     %12.2lf\n", soma_pnc);
    printf(" ------------------------------------------------------------------------\n");
    for(i = 0; i < k; i++){
        if(strncmp(estr[i].codigo, "2.02", 4) == 0){
            printf(" %-10s | %-40s |     %12.2lf\n", estr[i].codigo, estr[i].nome_conta_arq, estr[i].total_debito);
        }
    }
    printf(" ------------------------------------------------------------------------\n");
    printf(" 2.03       | PATRIMONIO LIQUIDO                       |     %12.2lf\n", soma_passivos_patrimonio);
    printf(" ------------------------------------------------------------------------\n");
    for(i = 0; i < k; i++){
        if(strncmp(estr[i].codigo, "2.03", 4) == 0){
            printf(" %-10s | %-40s |     %12.2lf\n", estr[i].codigo, estr[i].nome_conta_arq, estr[i].total_debito);
        }
    }
    printf(" ------------------------------------------------------------------------\n");
    printf("              TOTAL - PASSIVOS                         |     %12.2lf\n", soma_passivos);
    printf(" ------------------------------------------------------------------------\n");
    printf("\n\n");
}

