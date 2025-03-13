#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sistema.h"

int rep[5][7]; // Instruções que podem se repetir

void instGenerator() {
    FILE *arquivo = fopen("instructions.txt", "w");
    int random;
    int instruc[8] = {6, 6, 6, 6, 6, 6, 6, 6}; // Número de campos por opcode
    srand(time(NULL));

    // Gerar instruções que podem se repetir
    for (int i = 0; i < N_FOR; i++) {
        random = rand() % N_OPCODE; // Opcode será 0 (SOMA) ou 1 (SUBTRAI)
        rep[i][0] = random;
        for (int j = 1; j <= (instruc[random]); j += 2) {
            rep[i][j] = rand() % N_MEM;   // Endereço de bloco aleatório
            rep[i][j + 1] = rand() % N_WORD; // Endereço de palavra aleatório
        }
    }

    // Gerar as instruções principais
    for (int i = 0; i < N_INST - 1;) { // Reserve a última instrução para HALT
        random = (rand() % 100) + 1;
        if (random <= N_PROB && i + N_FOR < N_INST - 1) {
            // Repetir as instruções
            for (int j = 0; j < N_FOR; j++) {
                for (int k = 0; k < 7; k++) {
                    fprintf(arquivo, "%d:", rep[j][k]);
                }
                fprintf(arquivo, "\n");
            }
            i += N_FOR;
        } else {
            // Gerar uma nova instrução
            random = rand() % N_OPCODE; // Opcode será 0 (SOMA) ou 1 (SUBTRAI)
            fprintf(arquivo, "%d:", random);
            for (int j = 1; j <= instruc[random]; j += 2) {
                fprintf(arquivo, "%d:%d:", rand() % N_MEM, rand() % N_WORD);
            }
            fprintf(arquivo, "\n");
            i++;
        }
    }

    // Adicionar a instrução HALT no final
    fprintf(arquivo, "-1:0:0:0:0:0:0\n");

    fclose(arquivo);
}