#include "bellman_ford.h"

#include <limits.h>
#include <stdio.h>

static int executar_exemplo(int com_ciclo) {
    const int arestas[][3] = {
        {0, 1, 4}, {0, 2, 5}, {1, 2, -2}, {2, 3, 3}, {1, 3, 4}
    };
    GrafoListaPonderado *g = criar_grafo(5);
    int dist[5], pred[5];
    if (!g) return 0;
    for (size_t i = 0; i < sizeof(arestas) / sizeof(arestas[0]); i++) {
        if (!adicionar_aresta(g, arestas[i][0], arestas[i][1], arestas[i][2])) {
            liberar_grafo(g);
            return 0;
        }
    }
    /* 1 -> 2 -> 3 -> 1 tem custo -2 + 3 - 3 = -2. */
    if (com_ciclo && !adicionar_aresta(g, 3, 1, -3)) {
        liberar_grafo(g);
        return 0;
    }
    int resultado = bellman_ford(g, 0, dist, pred);
    if (resultado == 0) reconstruir_ciclo_negativo(g, pred);
    else if (resultado == 1) {
        for (int i = 0; i < g->vertices; i++) {
            if (dist[i] == INT_MAX) printf("0 -> %d: inacessivel\n", i);
            else printf("0 -> %d: distancia %d\n", i, dist[i]);
        }
    } else fprintf(stderr, "Erro ao executar Bellman-Ford.\n");
    liberar_grafo(g);
    return resultado >= 0;
}

int main(void) {
    printf("Grafo sem ciclo negativo:\n");
    if (!executar_exemplo(0)) return 1;
    printf("\nGrafo com ciclo negativo:\n");
    if (!executar_exemplo(1)) return 1;
    return 0;
}
