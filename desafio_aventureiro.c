#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ITENS 10

// ==========================
// Estruturas
// ==========================
typedef struct {
    char nome[30];
    char tipo[20];
    int quantidade;
} Item;

typedef struct No {
    Item dados;
    struct No* proximo;
} No;

// ==========================
// Variáveis globais de contagem
// ==========================
int comparacoesSequencial = 0;
int comparacoesBinaria = 0;

// ==========================
// Funções - VETOR
// ==========================
void inserirItemVetor(Item mochila[], int *tamanho);
void removerItemVetor(Item mochila[], int *tamanho);
void listarItensVetor(Item mochila[], int tamanho);
void ordenarVetor(Item mochila[], int tamanho);
int buscarSequencialVetor(Item mochila[], int tamanho, char nome[]);
int buscarBinariaVetor(Item mochila[], int tamanho, char nome[]);

// ==========================
// Funções - LISTA ENCADEADA
// ==========================
void inserirItemLista(No **inicio);
void removerItemLista(No **inicio);
void listarItensLista(No *inicio);
int buscarSequencialLista(No *inicio, char nome[]);

// ==========================
// Função principal
// ==========================
int main() {
    Item mochilaVetor[MAX_ITENS];
    int tamanhoVetor = 0;

    No *inicioLista = NULL;

    int opcaoPrincipal, opcaoSub;

    do {
        printf("\n=== MOCHILA DE LOOT ===\n");
        printf("1. Usar mochila com VETOR\n");
        printf("2. Usar mochila com LISTA ENCADEADA\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcaoPrincipal);
        getchar();

        switch (opcaoPrincipal) {
            case 1:
                do {
                    printf("\n--- Mochila com Vetor ---\n");
                    printf("1. Inserir item\n");
                    printf("2. Remover item\n");
                    printf("3. Listar itens\n");
                    printf("4. Ordenar itens\n");
                    printf("5. Buscar (sequencial)\n");
                    printf("6. Buscar (binária)\n");
                    printf("0. Voltar\n");
                    printf("Escolha: ");
                    scanf("%d", &opcaoSub);
                    getchar();

                    char nomeBusca[30];
                    int pos;

                    switch (opcaoSub) {
                        case 1:
                            inserirItemVetor(mochilaVetor, &tamanhoVetor);
                            break;
                        case 2:
                            removerItemVetor(mochilaVetor, &tamanhoVetor);
                            break;
                        case 3:
                            listarItensVetor(mochilaVetor, tamanhoVetor);
                            break;
                        case 4:
                            ordenarVetor(mochilaVetor, tamanhoVetor);
                            break;
                        case 5:
                            printf("Digite o nome do item: ");
                            fgets(nomeBusca, sizeof(nomeBusca), stdin);
                            nomeBusca[strcspn(nomeBusca, "\n")] = '\0';
                            comparacoesSequencial = 0;
                            pos = buscarSequencialVetor(mochilaVetor, tamanhoVetor, nomeBusca);
                            if (pos != -1)
                                printf("Item encontrado: %s (%s, %d)\n", mochilaVetor[pos].nome, mochilaVetor[pos].tipo, mochilaVetor[pos].quantidade);
                            else
                                printf("Item não encontrado!\n");
                            printf("Comparações: %d\n", comparacoesSequencial);
                            break;
                        case 6:
                            printf("Digite o nome do item: ");
                            fgets(nomeBusca, sizeof(nomeBusca), stdin);
                            nomeBusca[strcspn(nomeBusca, "\n")] = '\0';
                            ordenarVetor(mochilaVetor, tamanhoVetor);
                            comparacoesBinaria = 0;
                            pos = buscarBinariaVetor(mochilaVetor, tamanhoVetor, nomeBusca);
                            if (pos != -1)
                                printf("Item encontrado: %s (%s, %d)\n", mochilaVetor[pos].nome, mochilaVetor[pos].tipo, mochilaVetor[pos].quantidade);
                            else
                                printf("Item não encontrado!\n");
                            printf("Comparações: %d\n", comparacoesBinaria);
                            break;
                    }
                } while (opcaoSub != 0);
                break;

            case 2:
                do {
                    printf("\n--- Mochila com Lista Encadeada ---\n");
                    printf("1. Inserir item\n");
                    printf("2. Remover item\n");
                    printf("3. Listar itens\n");
                    printf("4. Buscar (sequencial)\n");
                    printf("0. Voltar\n");
                    printf("Escolha: ");
                    scanf("%d", &opcaoSub);
                    getchar();

                    char nomeBusca[30];
                    int achou;

                    switch (opcaoSub) {
                        case 1:
                            inserirItemLista(&inicioLista);
                            break;
                        case 2:
                            removerItemLista(&inicioLista);
                            break;
                        case 3:
                            listarItensLista(inicioLista);
                            break;
                        case 4:
                            printf("Digite o nome do item: ");
                            fgets(nomeBusca, sizeof(nomeBusca), stdin);
                            nomeBusca[strcspn(nomeBusca, "\n")] = '\0';
                            comparacoesSequencial = 0;
                            achou = buscarSequencialLista(inicioLista, nomeBusca);
                            if (!achou)
                                printf("Item não encontrado!\n");
                            printf("Comparações: %d\n", comparacoesSequencial);
                            break;
                    }
                } while (opcaoSub != 0);
                break;
        }

    } while (opcaoPrincipal != 0);

    printf("\nEncerrando o sistema...\n");
    return 0;
}

// ==========================
// Implementações - Vetor
// ==========================
void inserirItemVetor(Item mochila[], int *tamanho) {
    if (*tamanho >= MAX_ITENS) {
        printf("Mochila cheia!\n");
        return;
    }

    Item novo;
    printf("Nome: "); fgets(novo.nome, 30, stdin); novo.nome[strcspn(novo.nome, "\n")] = '\0';
    printf("Tipo: "); fgets(novo.tipo, 20, stdin); novo.tipo[strcspn(novo.tipo, "\n")] = '\0';
    printf("Quantidade: "); scanf("%d", &novo.quantidade); getchar();

    mochila[*tamanho] = novo;
    (*tamanho)++;
    printf("Item adicionado!\n");
}

void removerItemVetor(Item mochila[], int *tamanho) {
    char nome[30];
    printf("Nome do item a remover: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = '\0';

    for (int i = 0; i < *tamanho; i++) {
        if (strcmp(mochila[i].nome, nome) == 0) {
            for (int j = i; j < *tamanho - 1; j++) {
                mochila[j] = mochila[j + 1];
            }
            (*tamanho)--;
            printf("Item removido!\n");
            return;
        }
    }
    printf("Item não encontrado!\n");
}

void listarItensVetor(Item mochila[], int tamanho) {
    printf("\nItens na mochila:\n");
    if (tamanho == 0) {
        printf("Nenhum item.\n");
        return;
    }
    for (int i = 0; i < tamanho; i++) {
        printf("%d. %s - %s (%d)\n", i + 1, mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
    }
}

void ordenarVetor(Item mochila[], int tamanho) {
    Item temp;
    for (int i = 0; i < tamanho - 1; i++) {
        for (int j = 0; j < tamanho - i - 1; j++) {
            if (strcmp(mochila[j].nome, mochila[j + 1].nome) > 0) {
                temp = mochila[j];
                mochila[j] = mochila[j + 1];
                mochila[j + 1] = temp;
            }
        }
    }
    printf("Itens ordenados!\n");
}

int buscarSequencialVetor(Item mochila[], int tamanho, char nome[]) {
    for (int i = 0; i < tamanho; i++) {
        comparacoesSequencial++;
        if (strcmp(mochila[i].nome, nome) == 0) return i;
    }
    return -1;
}

int buscarBinariaVetor(Item mochila[], int tamanho, char nome[]) {
    int inicio = 0, fim = tamanho - 1;
    while (inicio <= fim) {
        comparacoesBinaria++;
        int meio = (inicio + fim) / 2;
        int cmp = strcmp(nome, mochila[meio].nome);
        if (cmp == 0) return meio;
        else if (cmp < 0) fim = meio - 1;
        else inicio = meio + 1;
    }
    return -1;
}

// ==========================
// Implementações - Lista Encadeada
// ==========================
void inserirItemLista(No **inicio) {
    No *novo = (No*) malloc(sizeof(No));
    if (!novo) return;

    printf("Nome: "); fgets(novo->dados.nome, 30, stdin); novo->dados.nome[strcspn(novo->dados.nome, "\n")] = '\0';
    printf("Tipo: "); fgets(novo->dados.tipo, 20, stdin); novo->dados.tipo[strcspn(novo->dados.tipo, "\n")] = '\0';
    printf("Quantidade: "); scanf("%d", &novo->dados.quantidade); getchar();

    novo->proximo = *inicio;
    *inicio = novo;

    printf("Item adicionado na lista!\n");
}

void removerItemLista(No **inicio) {
    if (*inicio == NULL) {
        printf("Lista vazia!\n");
        return;
    }

    char nome[30];
    printf("Nome do item a remover: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = '\0';

    No *atual = *inicio, *anterior = NULL;
    while (atual != NULL) {
        if (strcmp(atual->dados.nome, nome) == 0) {
            if (anterior == NULL)
                *inicio = atual->proximo;
            else
                anterior->proximo = atual->proximo;
            free(atual);
            printf("Item removido!\n");
            return;
        }
        anterior = atual;
        atual = atual->proximo;
    }
    printf("Item não encontrado!\n");
}

void listarItensLista(No *inicio) {
    if (inicio == NULL) {
        printf("Lista vazia!\n");
        return;
    }

    printf("\nItens na lista:\n");
    No *aux = inicio;
    int i = 1;
    while (aux != NULL) {
        printf("%d. %s - %s (%d)\n", i++, aux->dados.nome, aux->dados.tipo, aux->dados.quantidade);
        aux = aux->proximo;
    }
}

int buscarSequencialLista(No *inicio, char nome[]) {
    No *aux = inicio;
    while (aux != NULL) {
        comparacoesSequencial++;
        if (strcmp(aux->dados.nome, nome) == 0) {
            printf("Item encontrado: %s (%s, %d)\n", aux->dados.nome, aux->dados.tipo, aux->dados.quantidade);
            return 1;
        }
        aux = aux->proximo;
    }
    return 0;
}
