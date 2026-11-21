#include "dijkstra.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    /* Grafo direcionado de exemplo; o vértice 5 não é alcançável. */
    const int arestas[][3] = {
        {0, 1, 4}, {0, 2, 1}, {2, 1, 2}, {1, 3, 1},
        {2, 3, 5}, {3, 4, 3}, {1, 4, 7}
    };
    const int vertices = 6, origem = 0;
    GrafoListaPonderado *g = criar_grafo(vertices);
    int dist[6], pred[6];
    if (!g) { fprintf(stderr, "Erro ao criar o grafo.\n"); return 1; }
    for (size_t i = 0; i < sizeof(arestas) / sizeof(arestas[0]); i++) {
        if (!adicionar_aresta(g, arestas[i][0], arestas[i][1], arestas[i][2])) {
            fprintf(stderr, "Erro ao adicionar aresta.\n");
            liberar_grafo(g);
            return 1;
        }
    }
    if (!dijkstra(g, origem, dist, pred)) {
        fprintf(stderr, "Erro ao executar Dijkstra.\n");
        liberar_grafo(g);
        return 1;
    }
    for (int v = 0; v < vertices; v++) {
        printf("%d -> %d: ", origem, v);
        if (dist[v] == INT_MAX) printf("inacessivel\n");
        else {
            printf("distancia %d, caminho ", dist[v]);
            reconstruir_caminho(origem, v, pred);
            printf("\n");
        }
    }
    liberar_grafo(g);
    return 0;
}
