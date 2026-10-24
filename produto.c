#include "produto.h"
#include "util.h"

#include <stdio.h>
#include <limits.h>
#include <string.h>

int buscar_produto_por_codigo(const Produto lista[], int total, int codigo) {
    int i;
    for (i = 0; i < total; i++) {
        if (lista[i].codigo == codigo) return i;
    }
    return -1;
}

int buscar_produto_por_nome(const Produto lista[], int total, const char *nome) {
    int i;
    for (i = 0; i < total; i++) {
        if (lista[i].ativo && strcmp(lista[i].nome, nome) == 0) return i;
    }
    return -1;
}

void exibir_produto(const Produto *produto, const Categoria categorias[], int total_categorias) {
    int posicao = buscar_categoria_por_codigo(categorias, total_categorias, produto->codigo_categoria);
    printf("Codigo: %d\n", produto->codigo);
    printf("Nome: %s\n", produto->nome);
    printf("Categoria: %s\n", posicao >= 0 ? categorias[posicao].nome : "desconhecida");
    printf("Prateleira: %c\n", produto->prateleira);
    printf("Preco unitario: R$ %.2f\n", produto->preco_unitario);
    printf("Quantidade em estoque: %d\n", produto->quantidade);
    printf("Disponivel para venda: %d\n", produto->disponivel);
}

int cadastrar_produto(Produto lista[], int total, const Categoria categorias[], int total_categorias) {
    Produto novo;
    if (total >= MAX_PRODUTOS) {
        puts("Limite de 50 produtos atingido.");
        return total;
    }
    if (total_categorias == 0) {
        puts("Cadastre ao menos uma categoria antes de cadastrar produtos.");
        return total;
    }
    puts("\n--- Cadastrar Produto ---");
    for (;;) {
        novo.codigo = ler_inteiro_positivo("Codigo do produto: ");
        if (buscar_produto_por_codigo(lista, total, novo.codigo) == -1) break;
        puts("Esse codigo de produto ja esta cadastrado.");
    }
    ler_texto_obrigatorio("Nome do produto: ", novo.nome, sizeof novo.nome);
    novo.prateleira = ler_prateleira("Prateleira (A, B, C ou D): ");
    novo.preco_unitario = ler_preco_positivo("Preco unitario: ");
    novo.quantidade = ler_inteiro_nao_negativo("Quantidade em estoque: ");
    novo.disponivel = novo.quantidade > 0 ? 1 : 0;
    novo.ativo = 1;
    listar_categorias(categorias, total_categorias);
    for (;;) {
        novo.codigo_categoria = ler_inteiro_positivo("Codigo da categoria: ");
        if (buscar_categoria_por_codigo(categorias, total_categorias, novo.codigo_categoria) != -1) break;
        puts("Categoria inexistente. Escolha um codigo da lista.");
    }
    lista[total] = novo;
    puts("\nProduto cadastrado com sucesso:");
    exibir_produto(&lista[total], categorias, total_categorias);
    return total + 1;
}

void listar_produtos(const Produto lista[], int total, const Categoria categorias[], int total_categorias) {
    int i, exibidos = 0;
    puts("\n--- Listar Produtos ---");
    int ativos = 0;
    for (i = 0; i < total; i++) ativos += lista[i].ativo != 0;
    if (ativos == 0) {
        puts("Nenhum produto cadastrado.");
        return;
    }
    for (i = 0; i < total; i++) {
        if (!lista[i].ativo) continue;
        printf("\nProduto %d de %d\n", ++exibidos, ativos);
        exibir_produto(&lista[i], categorias, total_categorias);
    }
}

void pesquisar_produto(const Produto lista[], int total, const Categoria categorias[], int total_categorias) {
    int opcao, posicao;
    char nome[50];
    puts("\n--- Pesquisar Produto ---");
    puts("1 - Por codigo");
    puts("2 - Por nome");
    opcao = ler_inteiro("Opcao: ");
    if (opcao == 1) {
        posicao = buscar_produto_por_codigo(lista, total, ler_inteiro_positivo("Codigo do produto: "));
    } else if (opcao == 2) {
        ler_texto_obrigatorio("Nome do produto: ", nome, sizeof nome);
        posicao = buscar_produto_por_nome(lista, total, nome);
    } else {
        puts("Opcao invalida.");
        return;
    }
    if (posicao < 0 || !lista[posicao].ativo) {
        puts("Nenhum produto foi encontrado.");
    } else {
        puts("Produto encontrado:");
        exibir_produto(&lista[posicao], categorias, total_categorias);
    }
}

void alterar_estoque(Produto lista[], int total, const Categoria categorias[], int total_categorias) {
    int codigo, posicao, operacao, quantidade;
    puts("\n--- Alterar Estoque de um Produto ---");
    codigo = ler_inteiro_positivo("Codigo do produto: ");
    posicao = buscar_produto_por_codigo(lista, total, codigo);
    if (posicao < 0 || !lista[posicao].ativo) {
        puts("Produto nao encontrado.");
        return;
    }
    puts("1 - Entrada");
    puts("2 - Saida");
    operacao = ler_inteiro("Opcao: ");
    if (operacao != 1 && operacao != 2) {
        puts("Opcao invalida.");
        return;
    }
    quantidade = ler_inteiro_positivo("Quantidade: ");
    if (operacao == 1) {
        if (quantidade > INT_MAX - lista[posicao].quantidade) {
            puts("Quantidade excede o limite permitido.");
            return;
        }
        lista[posicao].quantidade += quantidade;
    } else {
        if (quantidade > lista[posicao].quantidade) {
            puts("Estoque insuficiente. Operacao nao realizada.");
            return;
        }
        lista[posicao].quantidade -= quantidade;
    }
    lista[posicao].disponivel = lista[posicao].quantidade > 0 ? 1 : 0;
    puts("Estoque atualizado com sucesso:");
    exibir_produto(&lista[posicao], categorias, total_categorias);
}

void remover_produto(Produto lista[], int total) {
    int codigo, posicao;
    puts("\n--- Remover Produto ---");
    codigo = ler_inteiro_positivo("Codigo do produto: ");
    posicao = buscar_produto_por_codigo(lista, total, codigo);
    if (posicao < 0 || !lista[posicao].ativo) {
        puts("Produto nao encontrado.");
        return;
    }
    lista[posicao].ativo = 0;
    printf("Produto %d removido com sucesso.\n", codigo);
}
