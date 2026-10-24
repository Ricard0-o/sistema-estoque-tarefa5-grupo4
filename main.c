#include "arquivo.h"
#include "categoria.h"
#include "produto.h"
#include "util.h"

#include <stdio.h>

#define ARQUIVO_CATEGORIAS "categorias.txt"
#define ARQUIVO_PRODUTOS "produtos.txt"

static int menu_categorias(Categoria categorias[], int total) {
    int opcao;
    for (;;) {
        puts("\n=== CATEGORIAS ===");
        puts("1 - Cadastrar Categoria");
        puts("2 - Listar Categorias");
        puts("3 - Voltar");
        opcao = ler_inteiro("Opcao: ");
        switch (opcao) {
            case 1: total = cadastrar_categoria(categorias, total); break;
            case 2: listar_categorias(categorias, total); break;
            case 3: return total;
            default: puts("Opcao invalida.");
        }
    }
}

int main(void) {
    Categoria categorias[MAX_CATEGORIAS];
    Produto produtos[MAX_PRODUTOS];
    int total_categorias = carregar_categorias(ARQUIVO_CATEGORIAS, categorias);
    int total_produtos = carregar_produtos(ARQUIVO_PRODUTOS, produtos, categorias, total_categorias);
    int opcao;

    printf("Dados carregados: %d categoria(s), %d produto(s).\n", total_categorias, total_produtos);
    for (;;) {
        puts("\n=== SISTEMA DE ESTOQUE ===");
        puts("1 - Cadastrar Produto");
        puts("2 - Listar Produtos");
        puts("3 - Pesquisar Produto");
        puts("4 - Alterar Estoque de um Produto");
        puts("5 - Remover Produto");
        puts("6 - Categorias");
        puts("7 - Sair");
        opcao = ler_inteiro("Opcao: ");
        switch (opcao) {
            case 1:
                total_produtos = cadastrar_produto(produtos, total_produtos, categorias, total_categorias);
                break;
            case 2:
                listar_produtos(produtos, total_produtos, categorias, total_categorias);
                break;
            case 3:
                pesquisar_produto(produtos, total_produtos, categorias, total_categorias);
                break;
            case 4:
                alterar_estoque(produtos, total_produtos, categorias, total_categorias);
                break;
            case 5:
                remover_produto(produtos, total_produtos);
                break;
            case 6:
                total_categorias = menu_categorias(categorias, total_categorias);
                break;
            case 7: {
                int categorias_salvas = salvar_categorias(ARQUIVO_CATEGORIAS, categorias, total_categorias);
                int produtos_salvos = salvar_produtos(ARQUIVO_PRODUTOS, produtos, total_produtos);
                if (!categorias_salvas || !produtos_salvos) {
                    puts("Erro ao salvar os dados. Verifique permissao de escrita na pasta.");
                    return 1;
                }
                puts("Categorias e produtos salvos. Ate logo!");
                return 0;
            }
            default:
                puts("Opcao invalida.");
        }
    }
}
