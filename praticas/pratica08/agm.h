#ifndef AGM_H
#define AGM_H

typedef struct {
    int *pai, *rank;
    int n;
} DSU;

typedef struct {
    int u, v, peso;
} Aresta;

/* Cada aresta representa uma ligacao nao direcionada. */
typedef struct {
    int n, quantidade, capacidade;
    Aresta *arestas;
} Grafo;

DSU *criar_dsu(int n);
int find(DSU *d, int x);
void unir(DSU *d, int x, int y);
void liberar_dsu(DSU *d);
Grafo *criar_grafo(int n, int capacidade);
int adicionar_aresta(Grafo *g, int u, int v, int peso);
void liberar_grafo(Grafo *g);
/* Retornam 1 se existe AGM, 0 se desconexo e -1 em erro.
   A saida deve ter espaco para n-1 arestas. */
int kruskal(const Grafo *g, Aresta *saida, int *quantidade, long long *custo);
int prim(const Grafo *g, Aresta *saida, int *quantidade, long long *custo);

#endif
