#ifndef CATEGORIA_H
#define CATEGORIA_H

#define MAX_CATEGORIAS 20

typedef struct Categoria {
    int codigo;
    char nome[30];
} Categoria;

int buscar_categoria_por_codigo(const Categoria lista[], int total, int codigo);
int cadastrar_categoria(Categoria lista[], int total);
void listar_categorias(const Categoria lista[], int total);

#endif
