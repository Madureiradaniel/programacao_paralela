#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "omp.h"
#define _GNU_SOURCE

int ocorrencia(char a[], char b[]);
void serial(int argc, char *argv[]);
void paralelo(int argc, char *argv[]);
void arquivo();
int ocorrenciaParalela(char a[], char b[], char *argv[]);

void main(int argc, char *argv[])
{
    //serial(argc, argv);
    arquivo(argv);
    //paralelo(argc, argv);
}

void paralelo(int argc, char *argv[])
{

    FILE *arquivo;
    size_t len = 1000000000000;
    char *linha = malloc(len);
    int result = 0;
    int NUM_THREADS;
    double temp_ini, temp_fim;

    arquivo = fopen(argv[2], "r");

    NUM_THREADS = atoi(argv[3]);

    temp_ini = omp_get_wtime();

    omp_set_num_threads(NUM_THREADS);

#pragma omp parallel
    {
        int sum = 0;
        //tive grande dificudlade em paralelizar essa parte por conta que o while não é paralelizavel
        // atribuir o texto a uma matriz, porem só funcionou até 1mb
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

int ocorrenciaParalela(char a[], char b[], char *argv[])
{

    int tam_a = strlen(a); // palavra que eu quero
    int tam_b = strlen(b); // frase procurada
    char palavra[tam_a];
    int ocorrencias = 0;
    char *ifPalavra;
    int NUM_THREADS;

    NUM_THREADS = atoi(argv[3]);

    omp_set_num_threads(NUM_THREADS);

// percorrendo toda a frase
    #pragma omp parallel
    {
        int sum = 0;

        
        for (int i = 0; i <= tam_b; i++)
        {
            #pragma omp for
            for (int j = 0; j < tam_a; j++)
            {
                palavra[j] = b[i + j];
            }

            //verifica se as duas sao iguais  
            #pragma omp atomic
            {
                ifPalavra = strstr(palavra, a);

                if (ifPalavra != NULL)
                {
                    sum = sum + 1;
                }
            }

        }

        #pragma omp critical
        ocorrencias = ocorrencias + sum;
    }

    return ocorrencias;
}

void arquivo(char *argv[])
{
    FILE *pFile;
    long lSize;
    char *buffer;
    size_t result;
    int ocorrencias = 0;

    pFile = fopen(argv[2], "rb");
    if (pFile == NULL)
    {
        fputs("File error", stderr);
        exit(1);
    }

    // obtain file size:
    fseek(pFile, 0, SEEK_END);
    lSize = ftell(pFile);
    rewind(pFile);

    // allocate memory to contain the whole file:
    buffer = (char *)malloc(sizeof(char) * lSize);
    if (buffer == NULL)
    {
        fputs("Memory error", stderr);
        exit(2);
    }

    // copy the file into the buffer:
    result = fread(buffer, 1, lSize, pFile);
    if (result != lSize)
    {
        fputs("Reading error", stderr);
        exit(3);
    }

    //achar ocorrencias
    ocorrencias = ocorrenciaParalela(argv[1], buffer, argv);

    printf("ocorrencias %i  \n", ocorrencias);

    fclose(pFile);
    free(buffer);
}