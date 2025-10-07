#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura para representar uma sala (nó da árvore)
typedef struct Sala {
    char nome[50];
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;

// Função para criar uma nova sala (nó)
Sala* criarSala(const char* nome) {
    Sala* novaSala = (Sala*)malloc(sizeof(Sala));
    if (novaSala == NULL) {
        printf("Erro ao alocar memória.\n");
        exit(1);
    }
    strcpy(novaSala->nome, nome);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

// Monta a árvore da mansão com salas fixas
Sala* montarMansao() {
    // Criando salas
    Sala* hall = criarSala("Hall de Entrada");
    Sala* salaEstar = criarSala("Sala de Estar");
    Sala* salaJantar = criarSala("Sala de Jantar");
    Sala* cozinha = criarSala("Cozinha");
    Sala* escritorio = criarSala("Escritório");
    Sala* biblioteca = criarSala("Biblioteca");

    // Conectando as salas (formando a árvore)
    hall->esquerda = salaEstar;
    hall->direita = salaJantar;
    salaEstar->esquerda = cozinha;
    salaEstar->direita = escritorio;
    salaJantar->direita = biblioteca;

    return hall; // raiz da árvore
}

// Função para explorar a mansão
void explorarSalas(Sala* salaAtual) {
    char escolha;

    while (salaAtual != NULL) {
        printf("\nVocê está na sala: %s\n", salaAtual->nome);

        // Verifica se é um nó-folha (sem saídas)
        if (salaAtual->esquerda == NULL && salaAtual->direita == NULL) {
            printf("Você chegou ao fim deste caminho.\n");
            break;
        }

        printf("Escolha o caminho:\n");
        if (salaAtual->esquerda != NULL)
            printf("  (e) Ir para a esquerda (%s)\n", salaAtual->esquerda->nome);
        if (salaAtual->direita != NULL)
            printf("  (d) Ir para a direita (%s)\n", salaAtual->direita->nome);
        printf("  (s) Sair da exploração\n");
        printf("Sua escolha: ");
        scanf(" %c", &escolha);

        if (escolha == 'e' && salaAtual->esquerda != NULL) {
            salaAtual = salaAtual->esquerda;
        } else if (escolha == 'd' && salaAtual->direita != NULL) {
            salaAtual = salaAtual->direita;
        } else if (escolha == 's') {
            printf("Saindo da exploração...\n");
            break;
        } else {
            printf("Opção inválida ou caminho inexistente. Tente novamente.\n");
        }
    }
}

// Função principal
int main() {
    Sala* mansao = montarMansao();
    printf("Bem-vindo à Mansão Misteriosa!\n");
    explorarSalas(mansao);

    // Liberar memória (opcional no nível novato, mas bom hábito)
    // ...

    return 0;
}
}

