#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ITENS 10

// Estrutura que representa um item da mochila
typedef struct {
    char nome[30];
    char tipo[20];
    int quantidade;
} Item;

// Funções do sistema
void inserirItem(Item mochila[], int *tamanho);
void removerItem(Item mochila[], int *tamanho);
void listarItens(Item mochila[], int tamanho);
void buscarItem(Item mochila[], int tamanho);

int main() {
    Item mochila[MAX_ITENS];
    int tamanho = 0;
    int opcao;

    do {
        printf("\n=== SISTEMA DE INVENTÁRIO ===\n");
        printf("1. Cadastrar item\n");
        printf("2. Remover item\n");
        printf("3. Listar itens\n");
        printf("4. Buscar item\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar(); // limpar buffer do teclado

        switch (opcao) {
            case 1:
                inserirItem(mochila, &tamanho);
                listarItens(mochila, tamanho);
                break;
            case 2:
                removerItem(mochila, &tamanho);
                listarItens(mochila, tamanho);
                break;
            case 3:
                listarItens(mochila, tamanho);
                break;
            case 4:
                buscarItem(mochila, tamanho);
                break;
            case 0:
                printf("\nEncerrando o sistema...\n");
                break;
            default:
                printf("\nOpção inválida! Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}

// Função para cadastrar novo item
void inserirItem(Item mochila[], int *tamanho) {
    if (*tamanho >= MAX_ITENS) {
        printf("\nMochila cheia! Não é possível adicionar mais itens.\n");
        return;
    }

    Item novoItem;

    printf("\n--- Cadastro de Item ---\n");
    printf("Nome do item: ");
    fgets(novoItem.nome, sizeof(novoItem.nome), stdin);
    novoItem.nome[strcspn(novoItem.nome, "\n")] = '\0'; // remover \n

    printf("Tipo do item (arma, munição, cura...): ");
    fgets(novoItem.tipo, sizeof(novoItem.tipo), stdin);
    novoItem.tipo[strcspn(novoItem.tipo, "\n")] = '\0';

    printf("Quantidade: ");
    scanf("%d", &novoItem.quantidade);
    getchar();

    mochila[*tamanho] = novoItem;
    (*tamanho)++;

    printf("\nItem cadastrado com sucesso!\n");
}

// Função para remover item pelo nome
void removerItem(Item mochila[], int *tamanho) {
    if (*tamanho == 0) {
        printf("\nMochila vazia! Nada a remover.\n");
        return;
    }

    char nomeBusca[30];
    printf("\n--- Remover Item ---\n");
    printf("Digite o nome do item a ser removido: ");
    fgets(nomeBusca, sizeof(nomeBusca), stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = '\0';

    int encontrado = -1;
    for (int i = 0; i < *tamanho; i++) {
        if (strcmp(mochila[i].nome, nomeBusca) == 0) {
            encontrado = i;
            break;
        }
    }

    if (encontrado != -1) {
        for (int j = encontrado; j < *tamanho - 1; j++) {
            mochila[j] = mochila[j + 1];
        }
        (*tamanho)--;
        printf("\nItem removido com sucesso!\n");
    } else {
        printf("\nItem não encontrado!\n");
    }
}

// Função para listar todos os itens da mochila
void listarItens(Item mochila[], int tamanho) {
    printf("\n--- Itens na Mochila ---\n");
    if (tamanho == 0) {
        printf("A mochila está vazia.\n");
        return;
    }

    for (int i = 0; i < tamanho; i++) {
        printf("[%d] Nome: %s | Tipo: %s | Quantidade: %d\n",
               i + 1, mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
    }
}

// Função para buscar um item pelo nome (busca sequencial)
void buscarItem(Item mochila[], int tamanho) {
    if (tamanho == 0) {
        printf("\nA mochila está vazia. Nenhum item para buscar.\n");
        return;
    }

    char nomeBusca[30];
    printf("\n--- Buscar Item ---\n");
    printf("Digite o nome do item a buscar: ");
    fgets(nomeBusca, sizeof(nomeBusca), stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = '\0';

    for (int i = 0; i < tamanho; i++) {
        if (strcmp(mochila[i].nome, nomeBusca) == 0) {
            printf("\nItem encontrado!\n");
            printf("Nome: %s | Tipo: %s | Quantidade: %d\n",
                   mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
            return;
        }
    }

    printf("\nItem não encontrado!\n");
}
