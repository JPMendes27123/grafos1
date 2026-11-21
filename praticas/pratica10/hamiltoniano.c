#include "hamiltoniano.h"
#include <stdlib.h>

static int buscar(const int *adj, int n, int *caminho, int *usado, int pos, int ciclo) {
    if (pos == n) return !ciclo || adj[caminho[n - 1] * n + caminho[0]];
    for (int v = 0; v < n; v++) {
        if (!usado[v] && adj[caminho[pos - 1] * n + v]) {
            usado[v] = 1;
            caminho[pos] = v;
            if (buscar(adj, n, caminho, usado, pos + 1, ciclo)) return 1;
            usado[v] = 0;
        }
    }
    return 0;
}

static int encontrar(const int *adj, int n, int *caminho, int ciclo) {
    if (!adj || !caminho || n <= 0) return -1;
    int *usado = calloc((size_t)n, sizeof(*usado));
    if (!usado) return -1;
    int encontrado = 0;
    /* Um ciclo pode ser rotacionado para comecar em 0; caminhos nao. */
    int tentativas = ciclo ? 1 : n;
    for (int inicio = 0; inicio < tentativas && !encontrado; inicio++) {
        caminho[0] = inicio;
        usado[inicio] = 1;
        encontrado = buscar(adj, n, caminho, usado, 1, ciclo);
        usado[inicio] = 0;
    }
    free(usado);
    if (encontrado && ciclo) caminho[n] = caminho[0];
    return encontrado;
}

int encontrar_caminho_hamiltoniano_bt(const int *adj, int n, int *caminho) {
    return encontrar(adj, n, caminho, 0);
}

int encontrar_ciclo_hamiltoniano_bt(const int *adj, int n, int *ciclo) {
    return encontrar(adj, n, ciclo, 1);
}
