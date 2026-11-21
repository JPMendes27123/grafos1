#include "agm.h"

#include <limits.h>
#include <stdlib.h>
#include <string.h>

DSU *criar_dsu(int n) {
    if (n <= 0) return NULL;
    DSU *d = malloc(sizeof(*d));
    if (!d) return NULL;
    d->pai = malloc((size_t)n * sizeof(*d->pai));
    d->rank = calloc((size_t)n, sizeof(*d->rank));
    if (!d->pai || !d->rank) { free(d->pai); free(d->rank); free(d); return NULL; }
    d->n = n;
    for (int i = 0; i < n; i++) d->pai[i] = i;
    return d;
}

int find(DSU *d, int x) {
    if (d->pai[x] != x) d->pai[x] = find(d, d->pai[x]);
    return d->pai[x];
}

void unir(DSU *d, int x, int y) {
    x = find(d, x);
    y = find(d, y);
    if (x == y) return;
    if (d->rank[x] < d->rank[y]) d->pai[x] = y;
    else {
        d->pai[y] = x;
        if (d->rank[x] == d->rank[y]) d->rank[x]++;
    }
}

void liberar_dsu(DSU *d) {
    if (!d) return;
    free(d->pai); free(d->rank); free(d);
}

Grafo *criar_grafo(int n, int capacidade) {
    if (n <= 0 || capacidade < 0) return NULL;
    Grafo *g = malloc(sizeof(*g));
    if (!g) return NULL;
    g->arestas = capacidade ? malloc((size_t)capacidade * sizeof(*g->arestas)) : NULL;
    if (capacidade && !g->arestas) { free(g); return NULL; }
    g->n = n; g->quantidade = 0; g->capacidade = capacidade;
    return g;
}

int adicionar_aresta(Grafo *g, int u, int v, int peso) {
    if (!g || u < 0 || v < 0 || u >= g->n || v >= g->n || g->quantidade >= g->capacidade) return 0;
    g->arestas[g->quantidade++] = (Aresta){u, v, peso};
    return 1;
}

void liberar_grafo(Grafo *g) {
    if (!g) return;
    free(g->arestas); free(g);
}

static int comparar_arestas(const void *a, const void *b) {
    int x = ((const Aresta *)a)->peso, y = ((const Aresta *)b)->peso;
    return (x > y) - (x < y);
}

int kruskal(const Grafo *g, Aresta *saida, int *quantidade, long long *custo) {
    if (!g || (g->n > 1 && !saida) || !quantidade || !custo) return -1;
    *quantidade = 0; *custo = 0;
    DSU *d = criar_dsu(g->n);
    if (!d) return -1;
    Aresta *ordenadas = NULL;
    if (g->quantidade) {
        ordenadas = malloc((size_t)g->quantidade * sizeof(*ordenadas));
        if (!ordenadas) { liberar_dsu(d); return -1; }
        memcpy(ordenadas, g->arestas, (size_t)g->quantidade * sizeof(*ordenadas));
        qsort(ordenadas, (size_t)g->quantidade, sizeof(*ordenadas), comparar_arestas);
    }
    for (int i = 0; i < g->quantidade && *quantidade < g->n - 1; i++) {
        Aresta a = ordenadas[i];
        if (find(d, a.u) != find(d, a.v)) {
            saida[(*quantidade)++] = a;
            *custo += a.peso;
            unir(d, a.u, a.v);
        }
    }
    free(ordenadas); liberar_dsu(d);
    return *quantidade == g->n - 1;
}

typedef struct { int destino, peso, proxima; } Adjacencia;
typedef struct { int vertice; long long chave; } NoHeap;
typedef struct { NoHeap *nos; int *posicao, tamanho; } MinHeap;

static void trocar(MinHeap *h, int a, int b) {
    NoHeap tmp = h->nos[a]; h->nos[a] = h->nos[b]; h->nos[b] = tmp;
    h->posicao[h->nos[a].vertice] = a; h->posicao[h->nos[b].vertice] = b;
}

static void diminuir(MinHeap *h, int v, long long chave) {
    int i = h->posicao[v];
    h->nos[i].chave = chave;
    while (i > 0 && h->nos[i].chave < h->nos[(i - 1) / 2].chave) {
        int pai = (i - 1) / 2;
        trocar(h, i, pai); i = pai;
    }
}

static NoHeap extrair(MinHeap *h) {
    NoHeap minimo = h->nos[0];
    h->posicao[minimo.vertice] = -1;
    if (--h->tamanho) {
        h->nos[0] = h->nos[h->tamanho];
        h->posicao[h->nos[0].vertice] = 0;
        int i = 0;
        for (;;) {
            int menor = i, esq = 2 * i + 1, dir = esq + 1;
            if (esq < h->tamanho && h->nos[esq].chave < h->nos[menor].chave) menor = esq;
            if (dir < h->tamanho && h->nos[dir].chave < h->nos[menor].chave) menor = dir;
            if (menor == i) break;
            trocar(h, i, menor); i = menor;
        }
    }
    return minimo;
}

int prim(const Grafo *g, Aresta *saida, int *quantidade, long long *custo) {
    if (!g || (g->n > 1 && !saida) || !quantidade || !custo || g->quantidade > INT_MAX / 2) return -1;
    *quantidade = 0; *custo = 0;
    int *cabeca = malloc((size_t)g->n * sizeof(*cabeca));
    int *pai = malloc((size_t)g->n * sizeof(*pai));
    long long *chave = malloc((size_t)g->n * sizeof(*chave));
    Adjacencia *adj = g->quantidade ? malloc((size_t)g->quantidade * 2 * sizeof(*adj)) : NULL;
    MinHeap h = {malloc((size_t)g->n * sizeof(*h.nos)), malloc((size_t)g->n * sizeof(*h.posicao)), g->n};
    if (!cabeca || !pai || !chave || (g->quantidade && !adj) || !h.nos || !h.posicao) {
        free(cabeca); free(pai); free(chave); free(adj); free(h.nos); free(h.posicao); return -1;
    }
    for (int i = 0; i < g->n; i++) {
        cabeca[i] = -1; pai[i] = -1; chave[i] = LLONG_MAX;
        h.nos[i] = (NoHeap){i, LLONG_MAX}; h.posicao[i] = i;
    }
    for (int i = 0; i < g->quantidade; i++) {
        Aresta a = g->arestas[i];
        adj[2*i] = (Adjacencia){a.v, a.peso, cabeca[a.u]}; cabeca[a.u] = 2*i;
        adj[2*i+1] = (Adjacencia){a.u, a.peso, cabeca[a.v]}; cabeca[a.v] = 2*i+1;
    }
    chave[0] = 0; diminuir(&h, 0, 0);
    while (h.tamanho) {
        NoHeap atual = extrair(&h);
        if (atual.chave == LLONG_MAX) break;
        int u = atual.vertice;
        if (pai[u] != -1) {
            saida[(*quantidade)++] = (Aresta){pai[u], u, (int)chave[u]};
            *custo += chave[u];
        }
        for (int i = cabeca[u]; i != -1; i = adj[i].proxima) {
            int v = adj[i].destino;
            if (h.posicao[v] != -1 && adj[i].peso < chave[v]) {
                chave[v] = adj[i].peso; pai[v] = u;
                diminuir(&h, v, chave[v]);
            }
        }
    }
    free(cabeca); free(pai); free(chave); free(adj); free(h.nos); free(h.posicao);
    return *quantidade == g->n - 1;
}
