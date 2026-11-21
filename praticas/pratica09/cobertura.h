#ifndef COBERTURA_H
#define COBERTURA_H

typedef struct {
    int u, v;
} Aresta;

typedef struct {
    int vertices, quantidade, capacidade;
    Aresta *arestas;
} Grafo;

Grafo *criar_grafo(int vertices, int capacidade);
int adicionar_aresta(Grafo *g, int u, int v);
void liberar_grafo(Grafo *g);
/* A cobertura usa um vetor de vertices posicoes: 1 selecionado, 0 nao. */
int eh_cobertura(const Grafo *g, const int *cobertura);
/* Retornam o tamanho da cobertura, ou -1 em caso de erro. */
int cobertura_minima_bt(const Grafo *g, int *cobertura);
int cobertura_gulosa(const Grafo *g, int *cobertura);

#endif
