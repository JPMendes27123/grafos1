#include "hamiltoniano.h"
#include "tps.h"
#include <stdio.h>
#include <time.h>

static void imprimir_caminho(const int *caminho, int tamanho) {
    for (int i = 0; i < tamanho; i++) printf("%s%d", i ? " -> " : "", caminho[i]);
    printf("\n");
}

static int experimento(int n) {
    int adj[12 * 12] = {0}, pesos[12 * 12], percurso[13];
    long long custo;
    /* Grafo nao direcionado deterministico com ciclo 0..n-1..0. */
    for (int u = 0; u < n; u++) for (int v = u + 1; v < n; v++) {
        int existe = v == u + 1 || (u == 0 && v == n - 1) || (u * 17 + v * 31 + u * v * 7) % 10 < 3;
        adj[u*n + v] = adj[v*n + u] = existe;
    }
    if (grafo_para_tsp(adj, n, pesos) < 0) return 0;
    int caminho = encontrar_caminho_hamiltoniano_bt(adj, n, percurso);
    int ciclo = encontrar_ciclo_hamiltoniano_bt(adj, n, percurso);
    if (caminho != 1 || ciclo != 1) return 0;
    printf("\nn=%d: caminho e ciclo hamiltonianos encontrados.\n", n);
    printf("Ciclo: "); imprimir_caminho(percurso, n + 1);
    typedef int (*Metodo)(const int *, int, int, int *, long long *);
    const Metodo metodos[] = {tsp_forca_bruta, tsp_vizinho_mais_proximo, tsp_insercao};
    const char *nomes[] = {"Forca bruta", "Vizinho mais proximo", "Insercao"};
    for (int i = 0; i < 3; i++) {
        clock_t inicio = clock();
        int resultado = metodos[i](pesos, n, 0, percurso, &custo);
        clock_t fim = clock();
        if (resultado != 1 || inicio == (clock_t)-1 || fim == (clock_t)-1) return 0;
        printf("%-21s custo=%lld tempo=%.6f s\n", nomes[i], custo, (double)(fim - inicio) / CLOCKS_PER_SEC);
    }
    return 1;
}

int main(void) {
    const int tamanhos[] = {5, 8, 10, 12};
    printf("Experimento TSP: pesos 1 para aresta existente e 2 para inexistente.\n");
    for (size_t i = 0; i < sizeof(tamanhos) / sizeof(tamanhos[0]); i++) {
        if (!experimento(tamanhos[i])) { fprintf(stderr, "Erro no experimento.\n"); return 1; }
    }
    return 0;
}
