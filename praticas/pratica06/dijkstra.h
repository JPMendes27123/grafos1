#ifndef DIJKSTRA_H
#define DIJKSTRA_H

typedef struct Aresta {
    int destino, peso;
    struct Aresta *proxima;
} Aresta;

typedef struct {
    int vertices;
    Aresta **adj;
} GrafoListaPonderado;

typedef struct {
    int vertice;
    int distancia;
} NoHeap;

typedef struct {
    NoHeap *dados;
    int capacidade, tamanho;
    int *posicao;
} MinHeap;

GrafoListaPonderado *criar_grafo(int vertices);
int adicionar_aresta(GrafoListaPonderado *g, int origem, int destino, int peso);
void liberar_grafo(GrafoListaPonderado *g);
MinHeap *criar_min_heap(int capacidade);
int inserir_heap(MinHeap *heap, int vertice, int distancia);
NoHeap extrair_minimo(MinHeap *heap);
void diminuir_chave(MinHeap *heap, int vertice, int distancia);
void liberar_min_heap(MinHeap *heap);
int dijkstra(GrafoListaPonderado *g, int origem, int *dist, int *pred);
void reconstruir_caminho(int origem, int destino, int *pred);

#endif
