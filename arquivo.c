#include "arquivo.h"

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Cada registro ocupa uma linha. O separador | nao e aceito nos nomes. */
static int separar(char *linha, char *campos[], int esperado) {
    int i;
    char *atual = linha;
    char *separador;
    size_t tamanho = strlen(linha);
    while (tamanho > 0 && (linha[tamanho - 1] == '\n' || linha[tamanho - 1] == '\r')) {
        linha[--tamanho] = '\0';
    }
    for (i = 0; i < esperado - 1; i++) {
        separador = strchr(atual, '|');
        if (separador == NULL) return 0;
        *separador = '\0';
        campos[i] = atual;
        atual = separador + 1;
    }
    if (strchr(atual, '|') != NULL) return 0;
    campos[esperado - 1] = atual;
    return 1;
}

static int inteiro_valido(const char *texto, int *resultado) {
    char *fim;
    long valor;
    errno = 0;
    valor = strtol(texto, &fim, 10);
    if (texto == fim || *fim != '\0' || errno == ERANGE || valor < INT_MIN || valor > INT_MAX) return 0;
    *resultado = (int)valor;
    return 1;
}

static int preco_valido(const char *texto, float *resultado) {
    char *fim;
    float valor;
    errno = 0;
    valor = strtof(texto, &fim);
    if (texto == fim || *fim != '\0' || errno == ERANGE || !isfinite(valor) || valor <= 0.0f) return 0;
    *resultado = valor;
    return 1;
}

int carregar_categorias(const char *caminho, Categoria lista[]) {
    FILE *arquivo = fopen(caminho, "r");
    char linha[256], *campos[2];
    int total = 0;
    Categoria categoria;
    if (arquivo == NULL) return 0; /* Primeira execucao: lista vazia. */
    while (fgets(linha, sizeof linha, arquivo) != NULL && total < MAX_CATEGORIAS) {
        if (!separar(linha, campos, 2) || !inteiro_valido(campos[0], &categoria.codigo)) continue;
        if (categoria.codigo <= 0 || campos[1][0] == '\0' || strlen(campos[1]) >= sizeof categoria.nome) continue;
        if (buscar_categoria_por_codigo(lista, total, categoria.codigo) != -1) continue;
        strcpy(categoria.nome, campos[1]);
        lista[total++] = categoria;
    }
    fclose(arquivo);
    return total;
}

int carregar_produtos(const char *caminho, Produto lista[], const Categoria categorias[], int total_categorias) {
    FILE *arquivo = fopen(caminho, "r");
    char linha[512], *campos[8];
    int total = 0, quantidade_campos, separadores;
    Produto produto;
    if (arquivo == NULL) return 0;
    while (fgets(linha, sizeof linha, arquivo) != NULL && total < MAX_PRODUTOS) {
        char *cursor;
        separadores = 0;
        for (cursor = linha; *cursor != '\0'; cursor++) {
            if (*cursor == '|') separadores++;
        }
        quantidade_campos = separadores == 7 ? 8 : separadores == 6 ? 7 : 0;
        if (quantidade_campos == 0 || !separar(linha, campos, quantidade_campos)) continue;
        if (!inteiro_valido(campos[0], &produto.codigo) || produto.codigo <= 0) continue;
        if (campos[1][0] == '\0' || strlen(campos[1]) >= sizeof produto.nome) continue;
        if (strlen(campos[2]) != 1 || strchr("ABCD", toupper((unsigned char)campos[2][0])) == NULL) continue;
        if (!preco_valido(campos[3], &produto.preco_unitario)) continue;
        if (!inteiro_valido(campos[4], &produto.quantidade) || produto.quantidade < 0) continue;
        if (!inteiro_valido(campos[5], &produto.disponivel)) continue;
        if (!inteiro_valido(campos[6], &produto.codigo_categoria)) continue;
        if (quantidade_campos == 8) {
            if (!inteiro_valido(campos[7], &produto.ativo) || (produto.ativo != 0 && produto.ativo != 1)) continue;
        } else {
            produto.ativo = 1; /* Arquivos antigos da Tarefa 4. */
        }
        if (buscar_produto_por_codigo(lista, total, produto.codigo) != -1) continue;
        if (buscar_categoria_por_codigo(categorias, total_categorias, produto.codigo_categoria) == -1) continue;
        strcpy(produto.nome, campos[1]);
        produto.prateleira = (char)toupper((unsigned char)campos[2][0]);
        produto.disponivel = produto.quantidade > 0 ? 1 : 0;
        lista[total++] = produto;
    }
    fclose(arquivo);
    return total;
}

int salvar_categorias(const char *caminho, const Categoria lista[], int total) {
    FILE *arquivo = fopen(caminho, "w");
    int i, erro = 0;
    if (arquivo == NULL) return 0;
    for (i = 0; i < total; i++) {
        if (fprintf(arquivo, "%d|%s\n", lista[i].codigo, lista[i].nome) < 0) erro = 1;
    }
    if (fclose(arquivo) != 0) erro = 1;
    return !erro;
}

int salvar_produtos(const char *caminho, const Produto lista[], int total) {
    FILE *arquivo = fopen(caminho, "w");
    int i, erro = 0;
    if (arquivo == NULL) return 0;
    for (i = 0; i < total; i++) {
        if (fprintf(arquivo, "%d|%s|%c|%.2f|%d|%d|%d|%d\n",
                    lista[i].codigo, lista[i].nome, lista[i].prateleira,
                    lista[i].preco_unitario, lista[i].quantidade,
                    lista[i].disponivel, lista[i].codigo_categoria, lista[i].ativo) < 0) erro = 1;
    }
    if (fclose(arquivo) != 0) erro = 1;
    return !erro;
}
