#ifndef UTIL_H
#define UTIL_H

#include <stddef.h>

int ler_inteiro(const char *mensagem);
int ler_inteiro_positivo(const char *mensagem);
int ler_inteiro_nao_negativo(const char *mensagem);
float ler_preco_positivo(const char *mensagem);
void ler_texto_obrigatorio(const char *mensagem, char *destino, size_t tamanho);
char ler_prateleira(const char *mensagem);

#endif
