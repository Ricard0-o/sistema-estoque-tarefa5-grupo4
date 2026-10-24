#include "util.h"

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int ler_linha(char *destino, size_t tamanho) {
    int caractere;
    size_t comprimento;

    if (fgets(destino, (int)tamanho, stdin) == NULL) {
        puts("Entrada encerrada.");
        exit(EXIT_FAILURE);
    }
    comprimento = strlen(destino);
    if (comprimento > 0 && destino[comprimento - 1] == '\n') {
        destino[comprimento - 1] = '\0';
        return 1;
    }
    while ((caractere = getchar()) != '\n' && caractere != EOF) {
    }
    return 0;
}

static char *sem_espacos_iniciais(char *texto) {
    while (isspace((unsigned char)*texto)) texto++;
    return texto;
}

static void remover_espacos_finais(char *texto) {
    size_t tamanho = strlen(texto);
    while (tamanho > 0 && isspace((unsigned char)texto[tamanho - 1])) {
        texto[--tamanho] = '\0';
    }
}

int ler_inteiro(const char *mensagem) {
    char linha[128], *inicio, *fim;
    long valor;
    for (;;) {
        printf("%s", mensagem);
        fflush(stdout);
        if (!ler_linha(linha, sizeof linha)) {
            puts("Entrada muito longa. Tente novamente.");
            continue;
        }
        inicio = sem_espacos_iniciais(linha);
        errno = 0;
        valor = strtol(inicio, &fim, 10);
        fim = sem_espacos_iniciais(fim);
        if (inicio != fim && *fim == '\0' && errno != ERANGE && valor >= INT_MIN && valor <= INT_MAX) {
            return (int)valor;
        }
        puts("Digite um numero inteiro valido.");
    }
}

int ler_inteiro_positivo(const char *mensagem) {
    int valor;
    do {
        valor = ler_inteiro(mensagem);
        if (valor <= 0) puts("O valor deve ser positivo.");
    } while (valor <= 0);
    return valor;
}

int ler_inteiro_nao_negativo(const char *mensagem) {
    int valor;
    do {
        valor = ler_inteiro(mensagem);
        if (valor < 0) puts("O valor nao pode ser negativo.");
    } while (valor < 0);
    return valor;
}

float ler_preco_positivo(const char *mensagem) {
    char linha[128], *inicio, *fim;
    float valor;
    for (;;) {
        printf("%s", mensagem);
        fflush(stdout);
        if (!ler_linha(linha, sizeof linha)) {
            puts("Entrada muito longa. Tente novamente.");
            continue;
        }
        inicio = sem_espacos_iniciais(linha);
        errno = 0;
        valor = strtof(inicio, &fim);
        fim = sem_espacos_iniciais(fim);
        if (inicio != fim && *fim == '\0' && errno != ERANGE && isfinite(valor) && valor > 0.0f) {
            return valor;
        }
        puts("Digite um preco maior que zero (use ponto decimal).");
    }
}

void ler_texto_obrigatorio(const char *mensagem, char *destino, size_t tamanho) {
    char linha[256], *inicio;
    for (;;) {
        printf("%s", mensagem);
        fflush(stdout);
        if (!ler_linha(linha, sizeof linha)) {
            puts("Texto muito longo. Tente novamente.");
            continue;
        }
        inicio = sem_espacos_iniciais(linha);
        remover_espacos_finais(inicio);
        if (*inicio == '\0') {
            puts("O nome nao pode ficar em branco.");
        } else if (strchr(inicio, '|') != NULL) {
            puts("O caractere | nao e permitido no nome.");
        } else if (strlen(inicio) >= tamanho) {
            printf("Use no maximo %zu caracteres.\n", tamanho - 1);
        } else {
            strcpy(destino, inicio);
            return;
        }
    }
}

char ler_prateleira(const char *mensagem) {
    char linha[128], *inicio;
    for (;;) {
        printf("%s", mensagem);
        fflush(stdout);
        if (!ler_linha(linha, sizeof linha)) {
            puts("Entrada muito longa. Tente novamente.");
            continue;
        }
        inicio = sem_espacos_iniciais(linha);
        remover_espacos_finais(inicio);
        if (inicio[0] != '\0' && inicio[1] == '\0') {
            char letra = (char)toupper((unsigned char)inicio[0]);
            if (letra >= 'A' && letra <= 'D') return letra;
        }
        puts("Prateleira invalida. Escolha A, B, C ou D.");
    }
}
