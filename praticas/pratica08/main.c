#include "agm.h"

#include <stdio.h>

static void mostrar(const char *nome, const Aresta *arestas, int quantidade, long long custo, int resultado) {
    printf("%s:\n", nome);
    if (resultado < 0) { printf("Erro ao calcular a AGM.\n"); return; }
    if (!resultado) { printf("Grafo desconexo: nao existe AGM.\n"); return; }
    for (int i = 0; i < quantidade; i++)
        printf("  %d -- %d (peso %d)\n", arestas[i].u, arestas[i].v, arestas[i].peso);
    printf("  Custo total: %lld\n", custo);
}

int main(void) {
    const Aresta entrada[] = {{0,1,4}, {0,2,2}, {1,2,1}, {1,3,5}, {2,3,8}, {2,4,10}, {3,4,2}};
    const int total = (int)(sizeof(entrada) / sizeof(entrada[0]));
    Grafo *g = criar_grafo(5, total);
    Aresta resultado[4];
    int quantidade, status;
    long long custo;
    if (!g) { fprintf(stderr, "Erro ao criar grafo.\n"); return 1; }
    for (int i = 0; i < total; i++) {
        if (!adicionar_aresta(g, entrada[i].u, entrada[i].v, entrada[i].peso)) {
            fprintf(stderr, "Erro ao adicionar aresta.\n"); liberar_grafo(g); return 1;
        }
    }
    status = kruskal(g, resultado, &quantidade, &custo);
    mostrar("Kruskal", resultado, quantidade, custo, status);
    if (status != 1) { liberar_grafo(g); return 1; }
    status = prim(g, resultado, &quantidade, &custo);
    mostrar("Prim", resultado, quantidade, custo, status);
    liberar_grafo(g);
    return status == 1 ? 0 : 1;
}
