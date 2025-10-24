/*
  Sistema de Priorização e Montagem da Torre de Resgate
  - Implementa Bubble Sort (por nome), Insertion Sort (por tipo) e Selection Sort (por prioridade)
  - Mede comparações e tempo de execução
  - Permite busca binária após ordenação por nome
  - Usabilidade: menu interativo e uso de fgets para entrada de strings
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_COMPONENTES 20
#define NOME_SIZE 30
#define TIPO_SIZE 20

typedef struct {
    char nome[NOME_SIZE];
    char tipo[TIPO_SIZE];
    int prioridade; // 1..10
} Componente;

/* --- Protótipos --- */
void cadastroComponentes(Componente vet[], int *n);
void mostrarComponentes(Componente vet[], int n);

/* Ordenações (cada uma recebe contador de comparações por ponteiro) */
void bubbleSortNome(Componente vet[], int n, long *comparacoes);
void insertionSortTipo(Componente vet[], int n, long *comparacoes);
void selectionSortPrioridade(Componente vet[], int n, long *comparacoes);

/* Busca binária por nome (assume vet ordenado por nome) */
int buscaBinariaPorNome(Componente vet[], int n, const char chave[], long *comparacoes);

/* Utilitário: medir tempo de execução de uma função de ordenação */
double medirTempo(void (*alg)(Componente[], int, long*), Componente vet[], int n, long *comparacoes);

/* Função auxiliar para limpar newline de fgets */
void strip_newline(char *s);

/* --- Implementação --- */

int main() {
    Componente componentes[MAX_COMPONENTES];
    int n = 0;
    int opcao;
    int ordenadoPorNome = 0; // flag para indicar se vetor está ordenado por nome

    printf("=== Módulo Final: Montagem da Torre de Resgate ===\n");

    do {
        printf("\nMenu principal:\n");
        printf("1 - Cadastrar componentes (até %d)\n", MAX_COMPONENTES);
        printf("2 - Mostrar componentes\n");
        printf("3 - Ordenar por NOME (Bubble Sort)\n");
        printf("4 - Ordenar por TIPO (Insertion Sort)\n");
        printf("5 - Ordenar por PRIORIDADE (Selection Sort)\n");
        printf("6 - Buscar componente-chave por NOME (Busca Binária) [requer ordenação por nome]\n");
        printf("0 - Sair\n");
        printf("Escolha uma opção: ");
        if (scanf("%d", &opcao) != 1) { // leitura robusta
            while (getchar() != '\n'); // limpar
            printf("Entrada inválida.\n");
            continue;
        }
        getchar(); // limpar '\n'

        if (opcao == 1) {
            cadastroComponentes(componentes, &n);
            ordenadoPorNome = 0;
        } else if (opcao == 2) {
            mostrarComponentes(componentes, n);
        } else if (opcao == 3) {
            if (n == 0) { printf("Nenhum componente cadastrado.\n"); continue; }
            long comps = 0;
            // medir tempo e comparações
            double t = medirTempo(bubbleSortNome, componentes, n, &comps);
            printf("\n== Resultado: Bubble Sort por NOME ==\n");
            printf("Comparações: %ld\n", comps);
            printf("Tempo (clock): %.6f s\n", t);
            mostrarComponentes(componentes, n);
            ordenadoPorNome = 1;
        } else if (opcao == 4) {
            if (n == 0) { printf("Nenhum componente cadastrado.\n"); continue; }
            long comps = 0;
            double t = medirTempo(insertionSortTipo, componentes, n, &comps);
            printf("\n== Resultado: Insertion Sort por TIPO ==\n");
            printf("Comparações: %ld\n", comps);
            printf("Tempo (clock): %.6f s\n", t);
            mostrarComponentes(componentes, n);
            ordenadoPorNome = 0;
        } else if (opcao == 5) {
            if (n == 0) { printf("Nenhum componente cadastrado.\n"); continue; }
            long comps = 0;
            double t = medirTempo(selectionSortPrioridade, componentes, n, &comps);
            printf("\n== Resultado: Selection Sort por PRIORIDADE ==\n");
            printf("Comparações: %ld\n", comps);
            printf("Tempo (clock): %.6f s\n", t);
            mostrarComponentes(componentes, n);
            ordenadoPorNome = 0;
        } else if (opcao == 6) {
            if (n == 0) { printf("Nenhum componente cadastrado.\n"); continue; }
            if (!ordenadoPorNome) {
                printf("A busca binária exige que os componentes estejam ordenados por NOME.\n");
                printf("Deseja ordenar por NOME agora com Bubble Sort? (s/n): ");
                char resp = getchar(); getchar(); // ler e limpar
                if (resp == 's' || resp == 'S') {
                    long comps = 0;
                    double t = medirTempo(bubbleSortNome, componentes, n, &comps);
                    printf("Ordenado por nome. Comparações: %ld Tempo: %.6f s\n", comps, t);
                    ordenadoPorNome = 1;
                } else {
                    printf("Operação de busca cancelada. Ordene por nome primeiro.\n");
                    continue;
                }
            }
            char chave[NOME_SIZE];
            printf("Digite o nome do componente-chave a buscar: ");
            fgets(chave, sizeof(chave), stdin);
            strip_newline(chave);

            long compsBusca = 0;
            int pos = buscaBinariaPorNome(componentes, n, chave, &compsBusca);
            if (pos != -1) {
                printf("\nComponente encontrado!\n");
                printf("Nome: %s | Tipo: %s | Prioridade: %d\n",
                       componentes[pos].nome, componentes[pos].tipo, componentes[pos].prioridade);
            } else {
                printf("\nComponente-chave NÃO encontrado.\n");
            }
            printf("Comparações na busca binária: %ld\n", compsBusca);
        } else if (opcao == 0) {
            printf("Encerrando...\n");
        } else {
            printf("Opção inválida.\n");
        }

    } while (opcao != 0);

    return 0;
}

/* --- Cadastro: permite inserir até MAX_COMPONENTES itens -- */
void cadastroComponentes(Componente vet[], int *n) {
    int restantes = MAX_COMPONENTES - *n;
    if (restantes == 0) { printf("Limite de componentes atingido (%d).\n", MAX_COMPONENTES); return; }

    printf("Quantos componentes deseja cadastrar? (máx %d): ", restantes);
    int k;
    if (scanf("%d", &k) != 1) { while (getchar() != '\n'); printf("Entrada inválida.\n"); return; }
    getchar();
    if (k < 1) { printf("Nenhum componente adicionado.\n"); return; }
    if (k > restantes) k = restantes;

    for (int i = 0; i < k; i++) {
        printf("\nCadastro %d de %d\n", i + 1, k);
        printf("Nome: ");
        fgets(vet[*n].nome, NOME_SIZE, stdin);
        strip_newline(vet[*n].nome);

        printf("Tipo: ");
        fgets(vet[*n].tipo, TIPO_SIZE, stdin);
        strip_newline(vet[*n].tipo);

        printf("Prioridade (1-10): ");
        int p;
        if (scanf("%d", &p) != 1) { while (getchar() != '\n'); printf("Entrada inválida. Prioridade setada para 1.\n"); p = 1; }
        if (p < 1) p = 1; if (p > 10) p = 10;
        vet[*n].prioridade = p;
        getchar(); // limpar '\n'

        (*n)++;
        printf("Componente cadastrado: %s | %s | %d\n", vet[*n - 1].nome, vet[*n - 1].tipo, vet[*n - 1].prioridade);
    }
}

/* --- Exibe o vetor de componentes formatado --- */
void mostrarComponentes(Componente vet[], int n) {
    printf("\n--- Componentes (%d) ---\n", n);
    if (n == 0) { printf("Nenhum componente cadastrado.\n"); return; }
    for (int i = 0; i < n; i++) {
        printf("%2d) Nome: %-28s | Tipo: %-12s | Prioridade: %2d\n",
               i + 1, vet[i].nome, vet[i].tipo, vet[i].prioridade);
    }
}

/* --- Bubble Sort por nome (string) --- */
void bubbleSortNome(Componente vet[], int n, long *comparacoes) {
    *comparacoes = 0;
    for (int i = 0; i < n - 1; i++) {
        int trocou = 0;
        for (int j = 0; j < n - i - 1; j++) {
            (*comparacoes)++;
            if (strcmp(vet[j].nome, vet[j + 1].nome) > 0) {
                Componente tmp = vet[j];
                vet[j] = vet[j + 1];
                vet[j + 1] = tmp;
                trocou = 1;
            }
        }
        if (!trocou) break;
    }
}

/* --- Insertion Sort por tipo (string) --- */
void insertionSortTipo(Componente vet[], int n, long *comparacoes) {
    *comparacoes = 0;
    for (int i = 1; i < n; i++) {
        Componente chave = vet[i];
        int j = i - 1;
        // comparar tipos
        while (j >= 0) {
            (*comparacoes)++;
            if (strcmp(vet[j].tipo, chave.tipo) > 0) {
                vet[j + 1] = vet[j];
                j--;
            } else break;
        }
        vet[j + 1] = chave;
    }
}

/* --- Selection Sort por prioridade (int) --- */
void selectionSortPrioridade(Componente vet[], int n, long *comparacoes) {
    *comparacoes = 0;
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < n; j++) {
            (*comparacoes)++;
            if (vet[j].prioridade < vet[minIdx].prioridade) {
                minIdx = j;
            }
        }
        if (minIdx != i) {
            Componente tmp = vet[i];
            vet[i] = vet[minIdx];
            vet[minIdx] = tmp;
        }
    }
}

/* --- Busca binária por nome --- */
int buscaBinariaPorNome(Componente vet[], int n, const char chave[], long *comparacoes) {
    int inicio = 0, fim = n - 1;
    *comparacoes = 0;
    while (inicio <= fim) {
        int meio = (inicio + fim) / 2;
        (*comparacoes)++;
        int cmp = strcmp(chave, vet[meio].nome);
        if (cmp == 0) return meio;
        else if (cmp < 0) fim = meio - 1;
        else inicio = meio + 1;
    }
    return -1;
}

/* --- medirTempo: executa a função de ordenação e retorna segundos --- */
double medirTempo(void (*alg)(Componente[], int, long*), Componente vet[], int n, long *comparacoes) {
    // Faz uma cópia do vetor para não alterar o original? Aqui alteramos in-place (aceitável)
    clock_t inicio = clock();
    alg(vet, n, comparacoes);
    clock_t fim = clock();
    return (double)(fim - inicio) / CLOCKS_PER_SEC;
}

/* --- remover '\n' final de strings lidas por fgets --- */
void strip_newline(char *s) {
    size_t len = strlen(s);
    if (len == 0) return;
    if (s[len - 1] == '\n') s[len - 1] = '\0';
}
