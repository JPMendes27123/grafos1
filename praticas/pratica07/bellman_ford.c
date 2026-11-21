#include "bellman_ford.h"

#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

GrafoListaPonderado *criar_grafo(int vertices) {
    if (vertices <= 0) return NULL;
    GrafoListaPonderado *g = malloc(sizeof(*g));
    if (!g) return NULL;
    g->adj = calloc((size_t)vertices, sizeof(*g->adj));
    if (!g->adj) { free(g); return NULL; }
    g->vertices = vertices;
    g->vertice_ciclo = -1;
    return g;
}

int adicionar_aresta(GrafoListaPonderado *g, int origem, int destino, int peso) {
    if (!g || origem < 0 || origem >= g->vertices || destino < 0 || destino >= g->vertices) return 0;
    Aresta *a = malloc(sizeof(*a));
    if (!a) return 0;
    a->destino = destino;
    a->peso = peso;
    a->proxima = g->adj[origem];
    g->adj[origem] = a;
    return 1;
}

void liberar_grafo(GrafoListaPonderado *g) {
    if (!g) return;
    for (int i = 0; i < g->vertices; i++) {
        Aresta *a = g->adj[i];
        while (a) {
            Aresta *proxima = a->proxima;
            free(a);
            a = proxima;
        }
    }
    free(g->adj);
    free(g);
}

int bellman_ford(GrafoListaPonderado *g, int origem, int *dist, int *pred) {
    if (!g || !dist || !pred || origem < 0 || origem >= g->vertices) return -1;
    /* Distancias intermediarias maiores evitam overflow ao somar pesos int. */
    int64_t *d = malloc((size_t)g->vertices * sizeof(*d));
    if (!d) return -1;
    g->vertice_ciclo = -1;
    for (int i = 0; i < g->vertices; i++) {
        d[i] = INT64_MAX;
        pred[i] = -1;
        dist[i] = INT_MAX;
    }
    d[origem] = 0;
    /* A ultima passagem identifica vertices afetados por ciclos negativos. */
    for (int passagem = 0; passagem < g->vertices; passagem++) {
        int alterou = 0;
        for (int u = 0; u < g->vertices; u++) {
            if (d[u] == INT64_MAX) continue;
            for (Aresta *a = g->adj[u]; a; a = a->proxima) {
                if ((a->peso > 0 && d[u] > INT64_MAX - a->peso) ||
                    (a->peso < 0 && d[u] < INT64_MIN - a->peso)) {
                    free(d);
                    return -1;
                }
                int64_t nova = d[u] + a->peso;
                if (nova < d[a->destino]) {
                    d[a->destino] = nova;
                    pred[a->destino] = u;
                    alterou = 1;
                    if (passagem == g->vertices - 1) g->vertice_ciclo = a->destino;
                }
            }
        }
        if (!alterou) break;
    }
    if (g->vertice_ciclo != -1) { free(d); return 0; }
    for (int i = 0; i < g->vertices; i++) {
        if (d[i] == INT64_MAX) continue;
        /* INT_MAX fica reservado para vertices inalcancaveis. */
        if (d[i] < INT_MIN || d[i] >= INT_MAX) { free(d); return -1; }
        dist[i] = (int)d[i];
    }
    free(d);
    return 1;
}

void reconstruir_ciclo_negativo(const GrafoListaPonderado *g, const int *pred) {
    if (!g || !pred || g->vertice_ciclo < 0) return;
    int v = g->vertice_ciclo;
    for (int i = 0; i < g->vertices; i++) {
        if (v < 0 || v >= g->vertices) return;
        v = pred[v];
    }
    if (v < 0 || v >= g->vertices) return;
    int *ciclo = malloc(((size_t)g->vertices + 1) * sizeof(*ciclo));
    if (!ciclo) { fprintf(stderr, "Erro de memoria ao reconstruir ciclo.\n"); return; }
    int inicio = v, quantidade = 0;
    do {
        if (v < 0 || v >= g->vertices || quantidade >= g->vertices) { free(ciclo); return; }
        ciclo[quantidade++] = v;
        v = pred[v];
    } while (v != inicio);
    printf("Ciclo negativo: %d", inicio);
    /* pred percorre as arestas ao contrario; imprima na ordem direta. */
    for (int i = quantidade - 1; i >= 0; i--) printf(" -> %d", ciclo[i]);
    printf("\n");
    free(ciclo);
}
