#ifndef BELLMAN_FORD_H
#define BELLMAN_FORD_H

typedef struct Aresta {
    int destino;
    int peso;
    struct Aresta *proxima;
} Aresta;

typedef struct {
    int vertices;
    Aresta **adj;
    int vertice_ciclo;
} GrafoListaPonderado;

GrafoListaPonderado *criar_grafo(int vertices);
int adicionar_aresta(GrafoListaPonderado *g, int origem, int destino, int peso);
void liberar_grafo(GrafoListaPonderado *g);
/* Retorna 1 para sucesso, 0 para ciclo negativo alcancavel e -1 em erro. */
int bellman_ford(GrafoListaPonderado *g, int origem, int *dist, int *pred);
void reconstruir_ciclo_negativo(const GrafoListaPonderado *g, const int *pred);

#endif
