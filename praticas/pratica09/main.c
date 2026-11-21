#include "cobertura.h"

#include <stdio.h>
#include <time.h>

/* Grafos fixos e reproduziveis, sem lacos, de tamanho ate 15 vertices. */
static Grafo *grafo_experimento(int n) {
    Grafo *g = criar_grafo(n, n * (n - 1) / 2);
    if (!g) return NULL;
    for (int u = 0; u < n; u++) {
        for (int v = u + 1; v < n; v++) {
            if (((u * 17 + v * 31 + u * v * 7) % 10) < 4 && !adicionar_aresta(g, u, v)) {
                liberar_grafo(g);
                return NULL;
            }
        }
    }
    return g;
}

int main(void) {
    const int tamanhos[] = {5, 8, 10, 12, 15};
    const int repeticoes = 1000;
    printf("Tempos medios de CPU por execucao (ms); %d repeticoes.\n", repeticoes);
    printf(" n | arestas | exata | gulosa | tempo exata | tempo gulosa\n");
    for (size_t i = 0; i < sizeof(tamanhos) / sizeof(tamanhos[0]); i++) {
        int n = tamanhos[i], exata[15], gulosa[15];
        Grafo *g = grafo_experimento(n);
        if (!g) { fprintf(stderr, "Erro ao criar grafo.\n"); return 1; }
        int qtd_exata = -1, qtd_gulosa = -1;
        clock_t inicio = clock();
        for (int r = 0; r < repeticoes; r++) qtd_exata = cobertura_minima_bt(g, exata);
        clock_t meio = clock();
        for (int r = 0; r < repeticoes; r++) qtd_gulosa = cobertura_gulosa(g, gulosa);
        clock_t fim = clock();
        if (inicio == (clock_t)-1 || meio == (clock_t)-1 || fim == (clock_t)-1 ||
            qtd_exata < 0 || qtd_gulosa < 0 || !eh_cobertura(g, exata) || !eh_cobertura(g, gulosa)) {
            fprintf(stderr, "Erro no experimento ou cobertura invalida.\n");
            liberar_grafo(g);
            return 1;
        }
        double tempo_exata = 1000.0 * (double)(meio - inicio) / CLOCKS_PER_SEC / repeticoes;
        double tempo_gulosa = 1000.0 * (double)(fim - meio) / CLOCKS_PER_SEC / repeticoes;
        printf("%2d | %7d | %5d | %6d | %11.6f | %12.6f\n",
               n, g->quantidade, qtd_exata, qtd_gulosa, tempo_exata, tempo_gulosa);
        liberar_grafo(g);
    }
    return 0;
}
