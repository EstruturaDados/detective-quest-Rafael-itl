#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_TABELA 10
#define TAM_SUSPEITO 50
#define TAM_LISTA 100

// ===============================
// Estruturas
// ===============================

typedef struct Sala {
    char nome[50];
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;

typedef struct Pista {
    char nome[50];
    struct Pista* esquerda;
    struct Pista* direita;
} Pista;

typedef struct NoHash {
    char pista[50];
    char suspeito[50];
    struct NoHash* prox;
} NoHash;

// ===============================
// Funções de Sala
// ===============================

Sala* criarSala(const char* nome) {
    Sala* nova = malloc(sizeof(Sala));
    strcpy(nova->nome, nome);
    nova->esquerda = nova->direita = NULL;
    return nova;
}

// ===============================
// Funções de BST de Pistas
// ===============================

Pista* inserirPista(Pista* raiz, const char* nome) {
    if (!raiz) {
        Pista* nova = malloc(sizeof(Pista));
        strcpy(nova->nome, nome);
        nova->esquerda = nova->direita = NULL;
        return nova;
    }

    if (strcmp(nome, raiz->nome) < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, nome);
    else if (strcmp(nome, raiz->nome) > 0)
        raiz->direita = inserirPista(raiz->direita, nome);

    return raiz;
}

void emOrdem(Pista* raiz) {
    if (raiz) {
        emOrdem(raiz->esquerda);
        printf("🔍 %s\n", raiz->nome);
        emOrdem(raiz->direita);
    }
}

void liberarPistas(Pista* raiz) {
    if (raiz) {
        liberarPistas(raiz->esquerda);
        liberarPistas(raiz->direita);
        free(raiz);
    }
}

// ===============================
// Funções de Tabela Hash
// ===============================

int hash(const char* str) {
    int soma = 0;
    for (int i = 0; str[i] != '\0'; i++)
        soma += str[i];
    return soma % TAM_TABELA;
}

void inserirNaHash(NoHash* tabela[], const char* pista, const char* suspeito) {
    int h = hash(pista);
    NoHash* novo = malloc(sizeof(NoHash));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->prox = tabela[h];
    tabela[h] = novo;
}

void mostrarAssociacoes(NoHash* tabela[]) {
    printf("\n=== Associações Pista → Suspeito ===\n");
    for (int i = 0; i < TAM_TABELA; i++) {
        NoHash* atual = tabela[i];
        while (atual) {
            printf("🔎 %s → %s\n", atual->pista, atual->suspeito);
            atual = atual->prox;
        }
    }
}

void contarSuspeitos(NoHash* tabela[]) {
    char listaSuspeitos[TAM_LISTA][TAM_SUSPEITO];
    int contagens[TAM_LISTA] = {0};
    int total = 0;

    for (int i = 0; i < TAM_TABELA; i++) {
        NoHash* atual = tabela[i];
        while (atual) {
            int encontrado = 0;
            for (int j = 0; j < total; j++) {
                if (strcmp(listaSuspeitos[j], atual->suspeito) == 0) {
                    contagens[j]++;
                    encontrado = 1;
                    break;
                }
            }
            if (!encontrado && total < TAM_LISTA) {
                strcpy(listaSuspeitos[total], atual->suspeito);
                contagens[total] = 1;
                total++;
            }
            atual = atual->prox;
        }
    }

    // Encontrar o mais citado
    int max = 0, idx = -1;
    for (int i = 0; i < total; i++) {
        if (contagens[i] > max) {
            max = contagens[i];
            idx = i;
        }
    }

    if (idx != -1) {
        printf("\n⚠️ Suspeito mais citado: %s (%d pistas)\n", listaSuspeitos[idx], contagens[idx]);
    } else {
        printf("\nNenhum suspeito registrado.\n");
    }
}

void liberarHash(NoHash* tabela[]) {
    for (int i = 0; i < TAM_TABELA; i++) {
        NoHash* atual = tabela[i];
        while (atual) {
            NoHash* temp = atual;
            atual = atual->prox;
            free(temp);
        }
    }
}

// ===============================
// Sistema de Jogo
// ===============================

void explorarSalas(Sala* salaAtual, Pista** arvorePistas, NoHash* hashTable[]) {
    char escolha;

    while (salaAtual != NULL) {
        printf("\n🧭 Você está na sala: %s\n", salaAtual->nome);

        // Pistas automáticas
        if (strcmp(salaAtual->nome, "Biblioteca") == 0) {
            printf("📄 Pista encontrada: 'Papel rasgado'\n");
            *arvorePistas = inserirPista(*arvorePistas, "Papel rasgado");
            inserirNaHash(hashTable, "Papel rasgado", "Sr. Black");
        } else if (strcmp(salaAtual->nome, "Escritório") == 0) {
            printf("📄 Pista encontrada: 'Chave dourada'\n");
            *arvorePistas = inserirPista(*arvorePistas, "Chave dourada");
            inserirNaHash(hashTable, "Chave dourada", "Sra. White");
        } else if (strcmp(salaAtual->nome, "Despensa") == 0) {
            printf("📄 Pista encontrada: 'Luvas sujas'\n");
            *arvorePistas = inserirPista(*arvorePistas, "Luvas sujas");
            inserirNaHash(hashTable, "Luvas sujas", "Chef Mustard");
        }

        if (salaAtual->esquerda == NULL && salaAtual->direita == NULL) {
            printf("🚪 Fim do caminho.\n");
            break;
        }

        printf("\nPara onde deseja ir?\n");
        if (salaAtual->esquerda)
            printf(" (e) Esquerda: %s\n", salaAtual->esquerda->nome);
        if (salaAtual->direita)
            printf(" (d) Direita: %s\n", salaAtual->direita->nome);
        printf(" (p) Ver pistas | (a) Associações | (m) Mais suspeito | (s) Sair\n");
        printf("Escolha: ");
        scanf(" %c", &escolha);

        if (escolha == 'e' && salaAtual->esquerda)
            salaAtual = salaAtual->esquerda;
        else if (escolha == 'd' && salaAtual->direita)
            salaAtual = salaAtual->direita;
        else if (escolha == 'p') {
            printf("\n📜 Pistas coletadas:\n");
            emOrdem(*arvorePistas);
        } else if (escolha == 'a') {
            mostrarAssociacoes(hashTable);
        } else if (escolha == 'm') {
            contarSuspeitos(hashTable);
        } else if (escolha == 's') {
            printf("👋 Você saiu da exploração.\n");
            break;
        } else {
            printf("❌ Opção inválida.\n");
        }
    }
}

// ===============================
// Main
// ===============================

int main() {
    // Criar salas (árvore binária)
    Sala* hall = criarSala("Hall de Entrada");
    Sala* estar = criarSala("Sala de Estar");
    Sala* cozinha = criarSala("Cozinha");
    Sala* biblioteca = criarSala("Biblioteca");
    Sala* escritorio = criarSala("Escritório");
    Sala* despensa = criarSala("Despensa");

    hall->esquerda = estar;
    hall->direita = cozinha;
    estar->esquerda = biblioteca;
    estar->direita = escritorio;
    cozinha->direita = despensa;

    // Árvores e hash
    Pista* arvorePistas = NULL;
    NoHash* tabelaHash[TAM_TABELA] = {NULL};

    // Início
    printf("🔎 Bem-vindo à Mansão Misteriosa!\n");
    explorarSalas(hall, &arvorePistas, tabelaHash);

    // Finalização
    liberarPistas(arvorePistas);
    liberarHash(tabelaHash);
    free(biblioteca); free(escritorio); free(estar);
    free(despensa); free(cozinha); free(hall);

    return 0;
}