#include <stdio.h>
#include <stdlib.h> //utilizada para funções  rand() e srand()
#include <string.h>
#include <time.h> // utilizada para função time()
#include <conio.h>
#include <stdbool.h>
#include <locale.h>
#define MAX_LEN 100

/* UNIVERSIDADE SÃO FRANCISCO */
/* DISCIPLINA: PRÁTICA PROFISSIONAL FERRAMENTAS E TÉCNICAS DE PROGRAMAÇÃO */
/* PROFESSOR AUTOR: FABIO ANDRIJAUSKAS */
/* TUTOR ONLINE: PEDRO FASCINA CASARIN */
/* TUTOR PRESENCIAL: GABRIEL BINOTTI */
/* NOME PROGRAMA: SENSOR - VERSÃO: 0003 */
/* DATA: 11/10/2022 */

void registro_coleta();
void acessar_coletas();

void alterar_inspecao(linha, pos_linha);
void mudar_dado(linha, pos_linha);
void remover_inspecao(linha, pos_linha);

void comp(nome, codigo, temp, vel_nominal, vel_rampa);
const char* verificar_newline(str);
void iniciar_struct();

struct Inspecoes {
    char nome[100];
    char data[11];
    char codigo[11];
    int temp;
    int vel_nominal;
    int vel_rampa;
};
struct Inspecoes inspecao[MAX_LEN];

void main() {
    setlocale(LC_ALL, "Portuguese");
    iniciar_struct();
    int coleta;
    while (1) {
        printf("\n\n*************************************************SISTEMA DE MONITORAMENTO********************************************** \n");
        printf("Para efetuar a Inspeção, digite 1\nPara consultar as Inspeções, digite 2\nPara sair, digite 3.\n");
        fflush(stdin);
        scanf(" %i", &coleta);
        printf(" \n");
        if (coleta == 1) {
            registro_coleta();
        } else if (coleta == 2) {
            acessar_coletas();
        } else if (coleta == 3) {
            break;
        }
    }
}

void iniciar_struct() {
    int i;

    for (i=0; i < MAX_LEN; i++) {
        strcpy(inspecao[i].nome, "NULL");
        strcpy(inspecao[i].data, "NULL");
        strcpy(inspecao[i].codigo, "NULL");
        inspecao[i].temp = 0;
        inspecao[i].vel_nominal = 0;
        inspecao[i].vel_rampa = 0;
    }
    FILE *fp;
    if((fp = fopen("Coletas.csv","r"))!=NULL) { // Checar existência do arquivo Coletas.csv

        // COLOCAR DADOS DO ARQUIVO NA STRUCT
        char buffer[255] = {0}; // Armazenar uma linha do arquivo de cada vez
        int count_linha = 0; // index para marcar em qual linha estamos
        int esta_na_primeira_linha = 1; // para pular a primeira linha do arquivo, que contém os títulos das colunas

        while ((fgets (buffer, 255, fp)) != NULL) { // Enquanto o arquivo tiver linhas...
            if (esta_na_primeira_linha == 1) {
                esta_na_primeira_linha = 0;
                continue;
            }
            char *p;            // pointer que contém a linha do arquivo, com os dados separados
            int fldidx = 0;     // index para marcar qual o dado que estamos no momento do loop

            for (p = strtok (buffer, ","); p != NULL; p = strtok(NULL, ",")) { // Iterar sobre cada dado da linha
                if (fldidx == 0) strncpy (inspecao[count_linha].nome, p, strlen (p)+1);
                else if (fldidx == 1) strncpy (inspecao[count_linha].data, p, strlen (p)+1);
                else if (fldidx == 2) strncpy (inspecao[count_linha].codigo, p, strlen (p)+1);
                else if (fldidx == 3) inspecao[count_linha].temp = atoi(p);
                else if (fldidx == 4) inspecao[count_linha].vel_nominal = atoi(p);
                else if (fldidx == 5) inspecao[count_linha].vel_rampa = atoi(p);
                fldidx++;
            }
            count_linha++;
        }
    } else {
        fp = fopen("Coletas.csv", "a");
        fprintf(fp, "Nome,Data,Codigo,Temp,Vel_nominal,Vel_rampa\n");
      }
    fclose(fp);
}

void registro_coleta() {
    int temp, vel_nominal, vel_rampa;
	char nome[100], data[11], codigo[11];
    while (1) {
        fflush(stdin);
        printf("Nome e sobrenome do responsável pela inspeção: ");
        fgets(nome,100,stdin);

        printf("\nData da inspeção (dd/mm/aaaa): ");
        fgets(data,11,stdin);
        fflush(stdin);

        printf("\nNúmero de inventário do equipamento: ");
        fgets(codigo,11,stdin);

        if (codigo[0] == '\n') {
            printf("%s", codigo);
            printf("Erro: O número de inventário não pode estar vazio!\n\n");
            continue;
        }

        // Verificar se o fgets() deixou newline nas variáveis
        verificar_newline(&nome);
        verificar_newline(&data);
        verificar_newline(&codigo);

        printf("\n\n");

        // gerando valores aleatórios na faixa de 0 a 100
        printf("Gerando valores aleatórios de Coleta:\n\n");
        srand(time(NULL));

        temp = rand() % 100;
        printf("Temp: %d\n", temp);
        vel_nominal = rand() % 100;
        printf("Vol: %d\n", vel_nominal);
        vel_rampa = rand() % 100;
        printf("Ramp: %d\n", vel_rampa);

        FILE *fp;
        fp = fopen("Coletas.csv", "a");
        // Pondo o diagnóstico da coleta no arquivo Registros.txt
        // Colocando dados na struct na posição adequada
        comp(nome, codigo, temp, vel_nominal, vel_rampa);
        fprintf(fp, "%s,%s,%s,%i,%i,%i\n", nome, data, codigo, temp, vel_nominal, vel_rampa);
        fclose(fp);
        iniciar_struct();
        break;
    }
}

void acessar_coletas() {

		// PROCURAR COLETAS
        int procurar;
        printf("1 - Ver todas as Inspeções\n2 - Ver (e alterar) uma Inspeção pelo n. do inventário\n3 - Voltar\n");
        fflush(stdin);
        scanf(" %i", &procurar);
        if (procurar == 3) {
            return;
        }

        // OPÇÕES DO MENU
    	if (procurar == 1) {
            int i;
            for (i=0; i<MAX_LEN; i++) {
                if (strcmp(inspecao[i].codigo, "NULL") == 0) break;
                printf("%s,%s,%s,%i,%i,%i\n\n", inspecao[i].nome, inspecao[i].data, inspecao[i].codigo,
                    inspecao[i].temp, inspecao[i].vel_nominal, inspecao[i].vel_rampa);
            }
            printf("\n");

    	} else if (procurar == 2) {
            char proc_codigo[21];
            printf("\n");
            printf("Digite o código da coleta:\n");
            fflush(stdin);
            fgets(proc_codigo,21,stdin);
            verificar_newline(&proc_codigo);

            //// Procurar a inspeção que está o código e printar a inspeção
            // Variável para copiarmos a linha da coleta que estamos procurando
            char dados[255];
            // Contar posição da linha no arquivo
            int pos_linha = 1;
            // Variável para checar existência do código da inspeção
            int tem_inspecao = 0;

            FILE *fp2;
            fp2 = fopen("Coletas.csv", "r");

            int i;
            for (i=0; i<MAX_LEN; i++) {
                pos_linha++;
                if (strcmp(inspecao[i].codigo, proc_codigo) == 0) {
                    tem_inspecao = 1;
                    snprintf(dados, sizeof(dados), "%s,%s,%s,%i,%i,%i\n", inspecao[i].nome, inspecao[i].data,
                        inspecao[i].codigo, inspecao[i].temp, inspecao[i].vel_nominal, inspecao[i].vel_rampa);
                    printf("Resultado da pesquisa: %s\n\n", dados);
                    break;
                }
            }
            if (tem_inspecao == 0) {
                printf("Erro: Número de inventário não existente!");
                return;
            }
            fclose(fp2);
            alterar_inspecao(dados, pos_linha);
        }
}

void alterar_inspecao(linha, pos_linha) {
    int menu;

    printf("\n1 - Alterar dado\n2 - Excluir inspeção\n3 - Voltar\n");
    fflush(stdin);
    scanf(" %i", &menu);

    if (menu == 1) {
        mudar_dado(linha, pos_linha);

    } else if (menu == 2) {
        // Excluir
        remover_inspecao(linha, pos_linha);
    } else if (menu == 3) {
        return;
    }
}

void mudar_dado(linha, pos_linha) {
    printf("\n");
    int escolha;
    char valor_mod[100];

    printf("Mudar qual dado da Inspeção?\n1 - Nome\n2 - Data\n3 - N. do inventário\n\n4 - Voltar\n");
    fflush(stdin);
    scanf(" %i", &escolha);

    if (escolha == 4) return;

    printf("Novo valor:\n");
    fflush(stdin);
    fgets(valor_mod,100,stdin);
    verificar_newline(&valor_mod);

    // Criar um arquivo novo
    FILE *fp1, *fr1;
    fp1 = fopen("Coletas.csv", "r");
    fr1 = fopen("Replica.c", "w");

    // Construir string contendo o dado novo
    char linha_mod[255] = "";
    char *div_linha;
    int count = 0;
    div_linha = strtok(linha, ",");

    while ( div_linha != NULL ) {
        count++;
        if (count == escolha) {
            strcat(linha_mod, valor_mod);
        } else {
            strcat(linha_mod, div_linha);
        }
        if (count != 6) {
            strcat(linha_mod, ",");
        }
        div_linha = strtok(NULL, ",");
    }

    printf("Inspeção modificada: %s", linha_mod);

    // Ler conteúdo do arquivo e passar para a réplica (substituindo a string antiga pela nova)
    count = 0;
    char buffer[255] = {0};
    while ((fgets(buffer, 255, fp1)) != NULL) {
        count++;
        if (count == pos_linha) {
            fprintf(fr1, linha_mod);
        } else {
            fprintf(fr1, buffer);
        }
    }
    fclose(fp1);
    fclose(fr1);


    //Renomear o arquivo réplica para o nome original
    remove("Coletas.csv");
    rename("replica.c", "Coletas.csv");
    iniciar_struct();
    printf("\n");
}

void remover_inspecao(linha, pos_linha)
{
    FILE *fp1, *fp2;
    char ch;
    int pos = 1;

    //Abrir arquivo de coletas e criar uma réplica
    fp1 = fopen("Coletas.csv", "r");
    fp2 = fopen("replica.c", "w");

    // Copiar todas as linhas para o arquivo replica.c, exceto a inspeção a ser deletada
    while ((ch = getc(fp1)) != EOF)
    {
        if (ch == '\n')
            pos++;
            if (pos != pos_linha) putc(ch, fp2);
    }
    fclose(fp1);
    fclose(fp2);

    //Renomear o arquivo réplica para o nome original
    remove("Coletas.csv");
    rename("replica.c", "Coletas.csv");
    iniciar_struct();
    printf("\n Inspeção excluída!");
}

const char* verificar_newline(char *str) {
    // Verificar se o fgets() deixou newline nas variáveis
    char *tmp = str;
    if (tmp[strlen(tmp)-1] == '\n') {
        tmp[strlen(tmp)-1]='\0';
    }
    strcpy(str, tmp);
    return str;
}

void comp(nome, codigo, temp, vel_nominal, vel_rampa) {
	int erro = 0;
	FILE *fp3;
	fp3 = fopen("Diagnósticos.txt", "a");
	fprintf(fp3, "*************************************\n");
	fprintf(fp3, "Coleta de código %s\n", codigo);
	fprintf(fp3, "Nome do responsável: %s\n", nome);
	fprintf(fp3, "Temperatura: %i\n", temp);
	fprintf(fp3, "Vel. nominal: %i\n", vel_nominal);
	fprintf(fp3, "Vel. da rampa: %i\n", vel_rampa);

	if (temp < 22 || temp > 38) {

		printf("A temperatura está fora dos padrões (padrão: entre 22 e 38)! Mande para a manutenção!\n");
		fprintf(fp3, "A temperatura está fora dos padrões (padrão: entre 22 e 38)! Mande para a manutenção!\n");
		erro++;
	}
	if (vel_nominal < 55 || vel_nominal > 120) {
		printf("A vel. nominal está fora dos padrões (padrão: entre 55 e 120)!  Mande para a manutenção!\n");
		fprintf(fp3, "A vel. nominal está fora dos padrões (padrão: entre 55 e 120)!  Mande para a manutenção!\n");
		erro++;
	}
	if (vel_rampa < 60 || vel_rampa > 135) {
		printf("A vel. da rampa está fora dos padrões (padrão: entre 60 e 135)!  Mande para a manutenção!\n");
		fprintf(fp3, "A vel. da rampa está fora dos padrões (padrão: entre 60 e 135)!  Mande para a manutenção!\n");
		erro++;
	}
	if (erro == 0) {
	   printf("Está dentro dos padrões!\n");
	   fprintf(fp3, "Está dentro dos padrões!\n");
	}
	fprintf(fp3, "\n");
	fclose(fp3);
}
