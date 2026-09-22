#include <stdio.h>

// Variaveis globais: todas as funcoes enxergam o estado do jogo
int pinos[3][5];   // pinos[p][i] = disco na posicao i do pino p (posicao 0 = base)
int qtd[3];          // quantos discos existem em cada pino
int n;               // numero de discos da partida
int movimentos = 0;  // quantos movimentos ja foram feitos


// Mostra os 3 pinos (discos listados da base para o topo)
void mostrar() {
    for (int p = 0; p < 3; p++) {
        printf("%c: ", 'A' + p);          // p=0 vira 'A', p=1 vira 'B', p=2 vira 'C'
        for (int i = 0; i < qtd[p]; i++) {
            printf("%d ", pinos[p][i]);
        }
        printf("\n");
    }
    printf("\n");
}


// Move o disco do topo do pino 'origem' para o pino 'destino'.
// Retorna 1 se conseguiu mover e 0 se a jogada for proibida.
int mover(int origem, int destino) {
    if (origem == destino) {
        printf("Origem e destino sao iguais!\n");
        return 0;
    }
    if (qtd[origem] == 0) {
        printf("O pino %c esta vazio!\n", 'A' + origem);
        return 0;
    }

    int disco = pinos[origem][qtd[origem] - 1];   // disco do topo da origem

    // Regra do jogo: nao pode colocar disco maior sobre um menor
    if (qtd[destino] > 0 && pinos[destino][qtd[destino] - 1] < disco) {
        printf("Nao pode colocar um disco maior sobre um menor!\n");
        return 0;
    }

    qtd[origem]--;                            // tira da origem
    pinos[destino][qtd[destino]] = disco;     // coloca no destino
    qtd[destino]++;
    movimentos++;
    return 1;
}


// FUNCAO RECURSIVA: o computador resolve o jogo.
// Move os 'k' menores discos de 'origem' para 'destino', usando 'auxiliar' de apoio.
void resolver(int k, int origem, int destino, int auxiliar) {
    // Condicao de parada: sem discos, nao ha nada para mover
    if (k == 0) {
        return;
    }

    // 1) chama a função novamente para sempre mover o menor disco primeiro para o pino auxiliar
    resolver(k - 1, origem, auxiliar, destino);

    // 2) move ele para o destino
    mover(origem, destino);
    printf("Movimento %d: disco %d de %c para %c\n",
           movimentos, k, 'A' + origem, 'A' + destino);
    mostrar();

    // 3) chama a função novamente para mover os discos que ficaram no pino auxiliar
    resolver(k - 1, auxiliar, destino, origem);
}


int main() {
    char opcao, letra1, letra2;
    int minimo = 0;

    printf("=== TORRE DE HANOI ===\n\n");
    printf("1 - Jogar\n");
    printf("2 - Ver o computador resolver\n");
    printf("Escolha: ");
    scanf(" %c", &opcao);

    // Pede o numero de discos (le como caractere e converte: '3' - '0' = 3)
    do {
        printf("Quantos discos (1 a 5)? ");
        scanf(" %c", &letra1);
        n = letra1 - '0';
    } while (n < 1 || n > 5);

    // Coloca todos os discos no pino A (indice 0), do maior (base) ao menor (topo)
    for (int i = 0; i < n; i++) {
        pinos[0][i] = n - i;
    }
    qtd[0] = n;

    // Menor numero de movimentos possivel: cada disco novo dobra e soma 1
    for (int i = 0; i < n; i++) {
        minimo = minimo * 2 + 1;
    }

    printf("\nInicio:\n");
    mostrar();

    if (opcao == '1') {
        // MODO JOGADOR: repete ate todos os discos estarem no pino C (indice 2)
        while (qtd[2] != n) {
            printf("Jogada (ex: A C) ou Q para sair: ");
            scanf(" %c", &letra1);
            if (letra1 == 'Q' || letra1 == 'q') {
                printf("Ate a proxima!\n");
                return 0;
            }
            scanf(" %c", &letra2);

            // Converte letra minuscula em maiuscula
            if (letra1 >= 'a') letra1 = letra1 - 'a' + 'A';
            if (letra2 >= 'a') letra2 = letra2 - 'a' + 'A';

            int origem = letra1 - 'A';    // 'A' vira 0, 'B' vira 1, 'C' vira 2
            int destino = letra2 - 'A';

            if (origem < 0 || origem > 2 || destino < 0 || destino > 2) {
                printf("Use apenas as letras A, B ou C!\n");
            } else if (mover(origem, destino)) {
                mostrar();
            }
        }
        printf("PARABENS! Voce resolveu em %d movimentos (minimo possivel: %d).\n",
               movimentos, minimo);
    } else {
        // MODO COMPUTADOR: chama a funcao recursiva (de A para C, usando B de apoio)
        resolver(n, 0, 2, 1);
        printf("Resolvido em %d movimentos (minimo possivel: %d).\n", movimentos, minimo);
    }

    return 0;
}
