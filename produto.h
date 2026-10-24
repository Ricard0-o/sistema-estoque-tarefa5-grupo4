#ifndef PRODUTO_H
#define PRODUTO_H

#include "categoria.h"

#define MAX_PRODUTOS 50

typedef struct Produto {
    int codigo;
    char nome[50];
    char prateleira;
    float preco_unitario;
    int quantidade;
    int disponivel;
    int codigo_categoria;
    int ativo;
} Produto;

int buscar_produto_por_codigo(const Produto lista[], int total, int codigo);
int buscar_produto_por_nome(const Produto lista[], int total, const char *nome);
int cadastrar_produto(Produto lista[], int total, const Categoria categorias[], int total_categorias);
void exibir_produto(const Produto *produto, const Categoria categorias[], int total_categorias);
void listar_produtos(const Produto lista[], int total, const Categoria categorias[], int total_categorias);
void pesquisar_produto(const Produto lista[], int total, const Categoria categorias[], int total_categorias);
void alterar_estoque(Produto lista[], int total, const Categoria categorias[], int total_categorias);
void remover_produto(Produto lista[], int total);

#endif
