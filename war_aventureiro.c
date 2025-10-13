#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Suporte a UTF-8 no Windows (para nomes de territorios com acentos, se o usuario digitar)
#ifdef _WIN32
    #include <windows.h>
#endif

// Estrutura Territorio
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

// Configura console para UTF-8 no Windows
void configurarUTF8() {
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif
}

// Cadastra os territorios
void cadastrarTerritorios(Territorio* mapa, int n) {
    for (int i = 0; i < n; i++) {
        printf("\nTerritorio %d:\n", i + 1);
        printf("Nome: ");
        scanf(" %[^\n]", mapa[i].nome);
        printf("Cor do exercito (ex: vermelho, azul): ");
        scanf(" %[^\n]", mapa[i].cor);
        printf("Numero de tropas: ");
        scanf("%d", &mapa[i].tropas);
    }
}

// Exibe todos os territorios
void exibirTerritorios(Territorio* mapa, int n) {
    printf("\n--- Territorios Cadastrados ---\n");
    for (int i = 0; i < n; i++) {
        printf("%d. Nome: %s | Cor: %s | Tropas: %d\n", 
               i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
    printf("\n");
}

// Simula um ataque entre dois territorios
void atacar(Territorio* atacante, Territorio* defensor) {
    // Nao pode atacar territorio da mesma cor
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("Erro: voce nao pode atacar um territorio da mesma cor!\n");
        return;
    }

    // Atacante precisa de pelo menos 2 tropas
    if (atacante->tropas < 2) {
        printf("Erro: o atacante precisa de pelo menos 2 tropas para atacar!\n");
        return;
    }

    // Simula dados (1 a 6)
    int dadoAtacante = rand() % 6 + 1;
    int dadoDefensor = rand() % 6 + 1;

    printf("\nDado do atacante (%s): %d\n", atacante->nome, dadoAtacante);
    printf("Dado do defensor (%s): %d\n", defensor->nome, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        printf("Ataque bem-sucedido! %s conquista %s!\n", atacante->nome, defensor->nome);

        // Move metade das tropas (arredondado para cima)
        int tropasMovidas = (atacante->tropas + 1) / 2;
        defensor->tropas = tropasMovidas;
        strcpy(defensor->cor, atacante->cor); // muda a cor do territorio

        atacante->tropas -= tropasMovidas;
    } else {
        printf("Ataque falhou! O defensor resiste.\n");
        atacante->tropas -= 1;
    }

    // Garante que sempre haja pelo menos 1 tropa
    if (atacante->tropas < 1) atacante->tropas = 1;
    if (defensor->tropas < 1) defensor->tropas = 1;
}

// Libera memoria alocada
void liberarMemoria(Territorio* mapa) {
    free(mapa);
    printf("Memoria liberada com sucesso.\n");
}

// Funcao principal
int main() {
    configurarUTF8(); // Configura UTF-8 no Windows (opcional, mas seguro)
    srand(time(NULL));

    int n;
    printf("Quantos territorios deseja cadastrar? ");
    scanf("%d", &n);

    Territorio* mapa = (Territorio*) calloc(n, sizeof(Territorio));
    if (mapa == NULL) {
        printf("Erro ao alocar memoria!\n");
        return 1;
    }

    cadastrarTerritorios(mapa, n);
    exibirTerritorios(mapa, n);

    char continuar;
    do {
        int idxAtacante, idxDefensor;

        printf("Escolha o numero do territorio ATACANTE: ");
        scanf("%d", &idxAtacante);
        printf("Escolha o numero do territorio DEFENSOR: ");
        scanf("%d", &idxDefensor);

        if (idxAtacante < 1 || idxAtacante > n || idxDefensor < 1 || idxDefensor > n) {
            printf("Indice invalido! Tente novamente.\n");
            continue;
        }

        atacar(&mapa[idxAtacante - 1], &mapa[idxDefensor - 1]);
        exibirTerritorios(mapa, n);

        printf("Deseja realizar outro ataque? (s/n): ");
        scanf(" %c", &continuar);
    } while (continuar == 's' || continuar == 'S');

    liberarMemoria(mapa);
    return 0;
}