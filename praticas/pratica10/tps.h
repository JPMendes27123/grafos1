#ifndef TPS_H
#define TPS_H

/* Matrizes n x n; os percursos tem espaco para n+1 vertices. */
int grafo_para_tsp(const int *adj, int n, int *pesos);
/* TSP admite matriz completa de pesos inteiros nao negativos.
   Retorna 1 em sucesso ou -1 em erro; custo e percurso sao saidas. */
int tsp_forca_bruta(const int *pesos, int n, int origem, int *percurso, long long *custo);
int tsp_vizinho_mais_proximo(const int *pesos, int n, int origem, int *percurso, long long *custo);
int tsp_insercao(const int *pesos, int n, int origem, int *percurso, long long *custo);

#endif
