#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "omp.h"
#define _GNU_SOURCE

int ocorrencia(char a[], char b[]);
void serial(int argc, char *argv[]);
void paralelo(int argc, char *argv[]);
int countRows(char *argv[]);

void main(int argc, char *argv[])
{
    serial(argc, argv);
    paralelo(argc, argv);
}

void paralelo(int argc, char *argv[])
{

    FILE *arquivo;
    size_t len = 1000;
    char *linha = malloc(len);

    int result = 0;
    int NUM_THREADS;
    double temp_ini, temp_fim;

    int count_rows = 0;

    arquivo = fopen(argv[2], "r");

    NUM_THREADS = atoi(argv[3]);
    temp_ini = omp_get_wtime();

    omp_set_num_threads(NUM_THREADS);

    #pragma omp parallel
    {
        int sum = 0;
        
        while (getline(&linha, &len, arquivo) > 0)
        {
            sum = sum + ocorrencia(argv[1], linha);
        }

        #pragma omp critical
        result = result + sum;
    }

    temp_fim = omp_get_wtime();

    fclose(arquivo);

    printf(" PARALELO: threads %i | Qtd de ocorrencias -> %d | no arquivo %s Duracao: %0.2f \n", NUM_THREADS, result, argv[2], temp_fim - temp_ini);
}

void serial(int argc, char *argv[])
{
    int result = 0;
    FILE *arquivo;
    char texto[500];
    double temp_ini, temp_fim;

    arquivo = fopen(argv[2], "r");

    temp_ini = omp_get_wtime();

    for (int i = 0; fgets(texto, 500, arquivo) != NULL; i++)
    {
        result += ocorrencia(argv[1], texto);
    }

    temp_fim = omp_get_wtime();

    fclose(arquivo);

    printf(" SERIAL: Quantidade de ocorrencias -> %d no arquivo %s Duracao: %0.2f \n", result, argv[2], temp_fim - temp_ini);
}

int ocorrencia(char a[], char b[])
{

    int tam_a = strlen(a); // palavra que eu quero
    int tam_b = strlen(b); // frase procurada
    char palavra[tam_a];
    int ocorrencias = 0;
    char *ifPalavra;

    // percorrendo toda a frase
    for (int i = 0; i <= tam_b; i++)
    {
        for (int j = 0; j < tam_a; j++)
        {
            palavra[j] = b[i + j];
        }

        //verifica se as duas sao iguais
        ifPalavra = strstr(palavra, a);

        if (ifPalavra != NULL)
        {
            ocorrencias = ocorrencias + 1;
        }
    }

    return ocorrencias;
}