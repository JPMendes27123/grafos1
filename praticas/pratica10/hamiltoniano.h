#ifndef HAMILTONIANO_H
#define HAMILTONIANO_H

/* Matriz de adjacencia n x n em ordem de linhas. */
int encontrar_caminho_hamiltoniano_bt(const int *adj, int n, int *caminho);
/* Em caso de sucesso, ciclo contem n vertices e repete a origem na posicao n. */
int encontrar_ciclo_hamiltoniano_bt(const int *adj, int n, int *ciclo);

#endif
