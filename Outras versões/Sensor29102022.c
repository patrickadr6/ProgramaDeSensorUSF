#include <stdio.h>
#include <stdlib.h> //utilizada para fun��es  rand()  e srand()
#include <string.h>
#include <time.h> // utilizada para fun��o time()
#include <conio.h>
#include <stdbool.h>
#include <locale.h>

/* UNIVERSIDADE S�O FRANCISCO */
/* DISCIPLINA: PR�TICA PROFISSIONAL FERRAMENTAS E T�CNICAS DE PROGRAMA��O */
/* PROFESSOR AUTOR: FABIO ANDRIJAUSKAS */
/* TUTOR ONLINE: PEDRO FASCINA CASARIN */
/* TUTOR PRESENCIAL: GABRIEL BINOTTI */
/* NOME PROGRAMA: SENSOR - VERS�O: 0003 */
/* DATA: 11/10/2022 */

void registro_coleta();
void acessar_coletas();

void alterar_inspecao(linha, pos_linha);
void mudar_dado(linha, pos_linha);
void remover_inspecao(linha, pos_linha);

void comp(temp, vel_nominal, vel_rampa);
const char* verificar_newline(str);

void main() {
    setlocale(LC_ALL, "Portuguese");
    int coleta;
    while (1) {
        printf("\n\n*************************************************SISTEMA DE MONITORAMENTO********************************************** \n");
        printf("Para efetuar a Inspe��o, digite 1\nPara consultar as Inspe��es, digite 2\nPara sair, digite 3.\n");
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

void registro_coleta() {
    int temp, vel_nominal, vel_rampa;
	char nome[100], data[11], codigo[11];
    while (1) {
        fflush(stdin);
        printf("Nome e sobrenome do respons�vel pela inspe��o: ");
        fgets(nome,100,stdin);

        printf("\nData da inspe��o (dd/mm/aaaa): ");
        fgets(data,11,stdin);
        fflush(stdin);

        printf("\nN�mero de invent�rio do equipamento: ");
        fgets(codigo,11,stdin);

        if (codigo[0] == '\n') {
            printf("%s", codigo);
            printf("Erro: O n�mero de invent�rio n�o pode estar vazio!\n\n");
            continue;
        }

        // Verificar se o fgets() deixou newline nas vari�veis
        verificar_newline(&nome);
        verificar_newline(&data);
        verificar_newline(&codigo);

        printf("\n\n");

        printf("Gerando valores aleat�rios de Coleta:\n\n");

        srand(time(NULL));

        int i;
        for (i=0; i < 1; i++) {
        // gerando valores aleat�rios na faixa de 0 a 100
        temp = rand() % 100;
        printf("Temp: %d\n", temp);
        vel_nominal = rand() % 100;
        printf("Vol: %d\n", vel_nominal);
        vel_rampa = rand() % 100;
        printf("Ramp: %d\n", vel_rampa);
        }

        FILE *fp;
        // Checar exist�ncia do arquivo Coletas.csv
        if((fp = fopen("Coletas.csv","r"))!=NULL) {
            fclose(fp);
            }
        else {
            fp = fopen("Coletas.csv", "a");
            fprintf(fp, "Nome,Data,Codigo,Temp,Vel_nominal,Vel_rampa\n");
            fclose(fp);
            }

        fp = fopen("Coletas.csv", "a");
        fprintf(fp, "%s,%s,%s,%i,%i,%i\n", nome, data, codigo, temp, vel_nominal, vel_rampa);

        fclose(fp);

        comp(temp, vel_nominal, vel_rampa);
        break;
    }
}

void acessar_coletas() {

		// PROCURAR COLETAS
        int procurar;
        printf("1 - Ver todas as Inspe��es\n2 - Ver (e alterar) uma Inspe��o pelo n. do invent�rio\n3 - Voltar\n");
        fflush(stdin);
        scanf(" %i", &procurar);
        if (procurar == 3) {
            return;
        }

        char buffer[255] = {0};

        FILE *fp2;
    	fp2 = fopen("Coletas.csv", "r");

        // OP��ES DO MENU
    	if (procurar == 1) {
            // Ler e printar conte�do do arquivo
            while ((fgets(buffer, 255, fp2)) != NULL) {
                puts(buffer);
            }
            fclose(fp2);
            printf("\n");

    	} else if (procurar == 2) {
            char proc_codigo[21];
            printf("\n");
            printf("Digite o c�digo da coleta:\n");
            fflush(stdin);
            fgets(proc_codigo,21,stdin);
            verificar_newline(&proc_codigo);

            // VERIFICAR SE H� ALGUMA INSPECAO COM ESSE CODIGO


            // Token
            char *token;
            int count = 0;

            //// Procurar a inspe��o que est� o c�digo e printar a inspe��o
            // Vari�veis para copiarmos a linha da coleta que estamos procurando
            char dados[255];
            char dados2[255];
            // Contar posi��o da linha no arquivo
            int pos_linha = 0;
            int pos_linha2;
            // Vari�vel para checar exist�ncia do c�digo da inspe��o
            int tem_inspecao = 0;

            while ((fgets(buffer, 255, fp2)) != NULL) {
                // Token ser� cada dado separado e fgets a linha completa
                strcpy(dados, buffer);
                pos_linha++;
                token = strtok(buffer, ",");
                count = 0;
                // Enquanto n�o passar por todos os dados da inspe��o
                while( token != NULL ) {
                    count++;
                    // Procurando o c�digo
                    if (count == 3 && strcmp(token, proc_codigo) == 0) {
                        // Copiar
                        strcpy(dados2, dados);
                        pos_linha2 = pos_linha;
                        tem_inspecao = 1;
                    }
                    if (count > 3) {
                        count = 0;
                    }

                    token = strtok(NULL, ",");
                }
            }
            if (tem_inspecao == 0) {
                printf("Erro: N�mero de invent�rio n�o existente!");
                return;
            }
            printf("Resultado da pesquisa: %s", dados2);
            fclose(fp2);

            alterar_inspecao(dados2, pos_linha2);
        }
}

void alterar_inspecao(linha, pos_linha) {
    int menu;

    printf("\n1 - Alterar dado\n2 - Excluir inspe��o\n3 - Voltar\n");
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
    printf("Mudar qual dado da Inspe��o?\n1 - Nome\n2 - Data\n3 - N. do invent�rio\n\n4 - Voltar\n");
    fflush(stdin);
    scanf(" %i", &escolha);

    if (escolha == 4) {
        return;
    }

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

    printf("Inspe��o modificada: %s", linha_mod);

    // Ler conte�do do arquivo e passar para a r�plica (substituindo a string antiga pela nova)
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

    //Renomear o arquivo r�plica para o nome original
    remove("Coletas.csv");
    rename("replica.c", "Coletas.csv");
    printf("\n");
}

void remover_inspecao(linha, pos_linha)
{
    FILE *fp1, *fp2;
    char ch;
    int pos = 1;

    //Abrir arquivo de coletas e criar uma r�plica
    fp1 = fopen("Coletas.csv", "r");
    fp2 = fopen("replica.c", "w");

    // Copiar todas as linhas para o arquivo replica.c, exceto a inspe��o a ser deletada
    while ((ch = getc(fp1)) != EOF)
    {
        if (ch == '\n')
            pos++;
            if (pos != pos_linha)
            {
                putc(ch, fp2);
            }
    }
    fclose(fp1);
    fclose(fp2);

    //Renomear o arquivo r�plica para o nome original
    remove("Coletas.csv");
    rename("replica.c", "Coletas.csv");

    printf("\n Inspe��o exclu�da!");

}

const char* verificar_newline(char *str) {
    // Verificar se o fgets() deixou newline nas vari�veis
    char *tmp = str;
    if (tmp[strlen(tmp)-1] == '\n') {
        tmp[strlen(tmp)-1]='\0';
    }
    strcpy(str, tmp);
    return str;
}

void comp(temp, vel_nominal, vel_rampa) {
	int erro = 0;
	FILE *fp3;
	fp3 = fopen("Resultado.txt", "a");
	if (temp < 22 || temp > 38) {
		printf("A temperatura est� fora dos padr�es (padr�o: entre 22 e 38)! Mande para a manuten��o!\n");
		fprintf(fp3, "A temperatura est� fora dos padr�es (padr�o: entre 22 e 38)! Mande para a manuten��o!\n");
		erro++;
	}
	if (vel_nominal < 55 || vel_nominal > 120) {
		printf("A vel. nominal est� fora dos padr�es (padr�o: entre 55 e 120)!  Mande para a manuten��o!\n");
		fprintf(fp3, "A vel. nominal est� fora dos padr�es (padr�o: entre 55 e 120)!  Mande para a manuten��o!\n");
		erro++;
	}
	if (vel_rampa < 60 || vel_rampa > 135) {
		printf("A vel. da rampa est� fora dos padr�es (padr�o: entre 60 e 135)!  Mande para a manuten��o!\n");
		fprintf(fp3, "A vel. da rampa est� fora dos padr�es (padr�o: entre 60 e 135)!  Mande para a manuten��o!\n");
		erro++;
	}
	if (erro == 0) {
	   printf("Est� dentro dos padr�es!\n");
	   fprintf(fp3, "Est� dentro dos padr�es!\n");
	}
	fclose(fp3);
}
