#ifndef ARQUIVO_H
#define ARQUIVO_H

#include "produto.h"

int carregar_categorias(const char *caminho, Categoria lista[]);
int carregar_produtos(const char *caminho, Produto lista[], const Categoria categorias[], int total_categorias);
int salvar_categorias(const char *caminho, const Categoria lista[], int total);
int salvar_produtos(const char *caminho, const Produto lista[], int total);

#endif
