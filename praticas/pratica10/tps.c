#include "tps.h"
#include <limits.h>
#include <stdlib.h>
#include <string.h>

int grafo_para_tsp(const int *adj, int n, int *pesos) {
    if (!adj || !pesos || n <= 0) return -1;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            pesos[i * n + j] = i == j ? 0 : (adj[i * n + j] ? 1 : 2);
    return 1;
}

static int validar(const int *p, int n, int origem, const int *percurso, const long long *custo) {
    if (!p || !percurso || !custo || n <= 0 || origem < 0 || origem >= n) return 0;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (p[i * n + j] < 0) return 0;
    return 1;
}

typedef struct {
    const int *pesos;
    int n, origem, *atual, *melhor, *usado;
    long long melhor_custo;
} Busca;

/* Examina todas as permutacoes dos vertices restantes. */
static void permutar(Busca *b, int pos, long long custo) {
    if (pos == b->n) {
        long long total = custo + b->pesos[b->atual[pos - 1] * b->n + b->origem];
        if (total < b->melhor_custo) {
            b->melhor_custo = total;
            memcpy(b->melhor, b->atual, (size_t)b->n * sizeof(*b->melhor));
            b->melhor[b->n] = b->origem;
        }
        return;
    }
    for (int v = 0; v < b->n; v++) {
        if (!b->usado[v]) {
            b->usado[v] = 1; b->atual[pos] = v;
            permutar(b, pos + 1, custo + b->pesos[b->atual[pos - 1] * b->n + v]);
            b->usado[v] = 0;
        }
    }
}

int tsp_forca_bruta(const int *p, int n, int origem, int *percurso, long long *custo) {
    if (!validar(p, n, origem, percurso, custo)) return -1;
    int *atual = malloc((size_t)n * sizeof(*atual));
    int *usado = calloc((size_t)n, sizeof(*usado));
    if (!atual || !usado) { free(atual); free(usado); return -1; }
    atual[0] = origem; usado[origem] = 1;
    Busca b = {p, n, origem, atual, percurso, usado, LLONG_MAX};
    permutar(&b, 1, 0);
    *custo = b.melhor_custo;
    free(atual); free(usado);
    return 1;
}

int tsp_vizinho_mais_proximo(const int *p, int n, int origem, int *percurso, long long *custo) {
    if (!validar(p, n, origem, percurso, custo)) return -1;
    int *usado = calloc((size_t)n, sizeof(*usado));
    if (!usado) return -1;
    percurso[0] = origem; usado[origem] = 1; *custo = 0;
    for (int i = 1; i < n; i++) {
        int anterior = percurso[i - 1], melhor = -1;
        for (int v = 0; v < n; v++)
            if (!usado[v] && (melhor == -1 || p[anterior*n + v] < p[anterior*n + melhor])) melhor = v;
        percurso[i] = melhor; usado[melhor] = 1;
        *custo += p[anterior*n + melhor];
    }
    percurso[n] = origem;
    *custo += p[percurso[n - 1] * n + origem];
    free(usado);
    return 1;
}

int tsp_insercao(const int *p, int n, int origem, int *percurso, long long *custo) {
    if (!validar(p, n, origem, percurso, custo)) return -1;
    int *usado = calloc((size_t)n, sizeof(*usado));
    if (!usado) return -1;
    percurso[0] = origem; percurso[1] = origem; usado[origem] = 1;
    int tamanho = 1;
    while (tamanho < n) {
        int escolhido = -1, distancia = INT_MAX;
        /* Escolhe o vertice fora do ciclo mais proximo de algum vertice do ciclo. */
        for (int v = 0; v < n; v++) if (!usado[v]) {
            for (int i = 0; i < tamanho; i++) {
                int d = p[percurso[i] * n + v];
                if (escolhido == -1 || d < distancia) { escolhido = v; distancia = d; }
            }
        }
        int pos = 0;
        long long aumento = LLONG_MAX;
        for (int i = 0; i < tamanho; i++) {
            int a = percurso[i], b = percurso[i + 1];
            long long delta = (long long)p[a*n + escolhido] + p[escolhido*n + b] - p[a*n + b];
            if (delta < aumento) { aumento = delta; pos = i + 1; }
        }
        for (int i = tamanho + 1; i > pos; i--) percurso[i] = percurso[i - 1];
        percurso[pos] = escolhido; usado[escolhido] = 1; tamanho++;
    }
    *custo = 0;
    for (int i = 0; i < n; i++) *custo += p[percurso[i]*n + percurso[i+1]];
    free(usado);
    return 1;
}
