#include "dijkstra.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

GrafoListaPonderado *criar_grafo(int vertices) {
    if (vertices <= 0) return NULL;
    GrafoListaPonderado *g = malloc(sizeof(*g));
    if (!g) return NULL;
    g->adj = calloc((size_t)vertices, sizeof(*g->adj));
    if (!g->adj) { free(g); return NULL; }
    g->vertices = vertices;
    return g;
}

int adicionar_aresta(GrafoListaPonderado *g, int origem, int destino, int peso) {
    if (!g || origem < 0 || origem >= g->vertices || destino < 0 ||
        destino >= g->vertices || peso < 0) return 0;
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
        while (a) { Aresta *proxima = a->proxima; free(a); a = proxima; }
    }
    free(g->adj);
    free(g);
}

MinHeap *criar_min_heap(int capacidade) {
    if (capacidade <= 0) return NULL;
    MinHeap *h = malloc(sizeof(*h));
    if (!h) return NULL;
    h->dados = malloc((size_t)capacidade * sizeof(*h->dados));
    h->posicao = malloc((size_t)capacidade * sizeof(*h->posicao));
    if (!h->dados || !h->posicao) {
        free(h->dados); free(h->posicao); free(h); return NULL;
    }
    h->capacidade = capacidade;
    h->tamanho = 0;
    for (int i = 0; i < capacidade; i++) h->posicao[i] = -1;
    return h;
}

static void trocar(MinHeap *h, int a, int b) {
    NoHeap temp = h->dados[a];
    h->dados[a] = h->dados[b];
    h->dados[b] = temp;
    h->posicao[h->dados[a].vertice] = a;
    h->posicao[h->dados[b].vertice] = b;
}

void diminuir_chave(MinHeap *h, int vertice, int distancia) {
    if (!h || vertice < 0 || vertice >= h->capacidade) return;
    int i = h->posicao[vertice];
    if (i < 0 || distancia > h->dados[i].distancia) return;
    h->dados[i].distancia = distancia;
    while (i > 0 && h->dados[(i - 1) / 2].distancia > h->dados[i].distancia) {
        int pai = (i - 1) / 2;
        trocar(h, i, pai);
        i = pai;
    }
}

int inserir_heap(MinHeap *h, int vertice, int distancia) {
    if (!h || vertice < 0 || vertice >= h->capacidade ||
        h->tamanho == h->capacidade || h->posicao[vertice] != -1) return 0;
    int i = h->tamanho++;
    h->dados[i].vertice = vertice;
    h->dados[i].distancia = distancia;
    h->posicao[vertice] = i;
    diminuir_chave(h, vertice, distancia);
    return 1;
}

NoHeap extrair_minimo(MinHeap *h) {
    NoHeap vazio = { -1, INT_MAX };
    if (!h || h->tamanho == 0) return vazio;
    NoHeap minimo = h->dados[0];
    h->posicao[minimo.vertice] = -1;
    h->tamanho--;
    if (h->tamanho > 0) {
        h->dados[0] = h->dados[h->tamanho];
        h->posicao[h->dados[0].vertice] = 0;
        int i = 0;
        for (;;) {
            int esq = 2 * i + 1, dir = esq + 1, menor = i;
            if (esq < h->tamanho && h->dados[esq].distancia < h->dados[menor].distancia) menor = esq;
            if (dir < h->tamanho && h->dados[dir].distancia < h->dados[menor].distancia) menor = dir;
            if (menor == i) break;
            trocar(h, i, menor);
            i = menor;
        }
    }
    return minimo;
}

void liberar_min_heap(MinHeap *h) {
    if (!h) return;
    free(h->dados); free(h->posicao); free(h);
}

int dijkstra(GrafoListaPonderado *g, int origem, int *dist, int *pred) {
    if (!g || !dist || !pred || origem < 0 || origem >= g->vertices) return 0;
    MinHeap *h = criar_min_heap(g->vertices);
    if (!h) return 0;
    for (int i = 0; i < g->vertices; i++) {
        dist[i] = i == origem ? 0 : INT_MAX;
        pred[i] = -1;
        inserir_heap(h, i, dist[i]);
    }
    while (h->tamanho > 0) {
        NoHeap atual = extrair_minimo(h);
        if (atual.distancia == INT_MAX) break;
        for (Aresta *a = g->adj[atual.vertice]; a; a = a->proxima) {
            if (h->posicao[a->destino] != -1 && a->peso < INT_MAX - atual.distancia &&
                atual.distancia + a->peso < dist[a->destino]) {
                dist[a->destino] = atual.distancia + a->peso;
                pred[a->destino] = atual.vertice;
                diminuir_chave(h, a->destino, dist[a->destino]);
            }
        }
    }
    liberar_min_heap(h);
    return 1;
}

void reconstruir_caminho(int origem, int destino, int *pred) {
    if (origem == destino) { printf("%d", origem); return; }
    if (pred[destino] == -1) { printf("Sem caminho"); return; }
    reconstruir_caminho(origem, pred[destino], pred);
    printf(" -> %d", destino);
}
