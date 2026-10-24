#include "categoria.h"
#include "util.h"

#include <stdio.h>

int buscar_categoria_por_codigo(const Categoria lista[], int total, int codigo) {
    int i;
    for (i = 0; i < total; i++) {
        if (lista[i].codigo == codigo) return i;
    }
    return -1;
}

int cadastrar_categoria(Categoria lista[], int total) {
    Categoria nova;
    if (total >= MAX_CATEGORIAS) {
        puts("Limite de 20 categorias atingido.");
        return total;
    }
    puts("\n--- Cadastrar Categoria ---");
    for (;;) {
        nova.codigo = ler_inteiro_positivo("Codigo da categoria: ");
        if (buscar_categoria_por_codigo(lista, total, nova.codigo) == -1) break;
        puts("Esse codigo de categoria ja esta cadastrado.");
    }
    ler_texto_obrigatorio("Nome da categoria: ", nova.nome, sizeof nova.nome);
    lista[total] = nova;
    puts("Categoria cadastrada com sucesso:");
    printf("Codigo: %d | Nome: %s\n", nova.codigo, nova.nome);
    return total + 1;
}

void listar_categorias(const Categoria lista[], int total) {
    int i;
    puts("\n--- Listar Categorias ---");
    if (total == 0) {
        puts("Nenhuma categoria cadastrada.");
        return;
    }
    for (i = 0; i < total; i++) {
        printf("Codigo: %d | Nome: %s\n", lista[i].codigo, lista[i].nome);
    }
}
