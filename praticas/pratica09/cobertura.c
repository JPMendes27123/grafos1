#include "cobertura.h"

#include <stdlib.h>
#include <string.h>

Grafo *criar_grafo(int vertices, int capacidade) {
    if (vertices <= 0 || capacidade < 0) return NULL;
    Grafo *g = malloc(sizeof(*g));
    if (!g) return NULL;
    g->arestas = capacidade ? malloc((size_t)capacidade * sizeof(*g->arestas)) : NULL;
    if (capacidade && !g->arestas) { free(g); return NULL; }
    g->vertices = vertices;
    g->quantidade = 0;
    g->capacidade = capacidade;
    return g;
}

int adicionar_aresta(Grafo *g, int u, int v) {
    if (!g || u < 0 || v < 0 || u >= g->vertices || v >= g->vertices ||
        g->quantidade >= g->capacidade) return 0;
    g->arestas[g->quantidade++] = (Aresta){u, v};
    return 1;
}

void liberar_grafo(Grafo *g) {
    if (!g) return;
    free(g->arestas);
    free(g);
}

int eh_cobertura(const Grafo *g, const int *cobertura) {
    if (!g || !cobertura) return 0;
    for (int i = 0; i < g->quantidade; i++) {
        Aresta a = g->arestas[i];
        if (!cobertura[a.u] && !cobertura[a.v]) return 0;
    }
    return 1;
}

/* Uma aresta descoberta obriga a escolher pelo menos uma de suas pontas. */
static void buscar(const Grafo *g, int *atual, int tamanho, int *melhor, int *limite) {
    if (tamanho >= *limite) return;
    int i;
    for (i = 0; i < g->quantidade; i++) {
        Aresta a = g->arestas[i];
        if (!atual[a.u] && !atual[a.v]) break;
    }
    if (i == g->quantidade) {
        *limite = tamanho;
        memcpy(melhor, atual, (size_t)g->vertices * sizeof(*melhor));
        return;
    }
    Aresta a = g->arestas[i];
    atual[a.u] = 1;
    buscar(g, atual, tamanho + 1, melhor, limite);
    atual[a.u] = 0;
    if (a.u != a.v) {
        atual[a.v] = 1;
        buscar(g, atual, tamanho + 1, melhor, limite);
        atual[a.v] = 0;
    }
}

int cobertura_minima_bt(const Grafo *g, int *cobertura) {
    if (!g || !cobertura) return -1;
    int *atual = calloc((size_t)g->vertices, sizeof(*atual));
    if (!atual) return -1;
    /* Todos os vertices constituem um limite inicial valido. */
    for (int i = 0; i < g->vertices; i++) cobertura[i] = 1;
    int melhor = g->vertices;
    buscar(g, atual, 0, cobertura, &melhor);
    free(atual);
    return melhor;
}

int cobertura_gulosa(const Grafo *g, int *cobertura) {
    if (!g || !cobertura) return -1;
    int *graus = calloc((size_t)g->vertices, sizeof(*graus));
    if (!graus) return -1;
    memset(cobertura, 0, (size_t)g->vertices * sizeof(*cobertura));
    int tamanho = 0;
    for (;;) {
        memset(graus, 0, (size_t)g->vertices * sizeof(*graus));
        for (int i = 0; i < g->quantidade; i++) {
            Aresta a = g->arestas[i];
            if (!cobertura[a.u] && !cobertura[a.v]) {
                graus[a.u]++;
                if (a.u != a.v) graus[a.v]++;
            }
        }
        int maior = 0;
        for (int v = 1; v < g->vertices; v++)
            if (graus[v] > graus[maior]) maior = v;
        if (graus[maior] == 0) break;
        cobertura[maior] = 1;
        tamanho++;
    }
    free(graus);
    return tamanho;
}
