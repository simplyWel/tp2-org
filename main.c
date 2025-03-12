#include "sistema.h"
#include <stdlib.h>
#include <stdio.h>

int main(void) {
    instGenerator();

    // Inicializar a CPU, RAM e cache
    RAM *ram = ramAloca(DEFAULT);
    cpu *processor = malloc(sizeof(cpu));
    caches *cache = malloc(sizeof(caches));

    iniciar_cache(cache);
    sorteiaRam(ram);

    // Ler instruções do arquivo
    int total_instrucoes;
    instrucao_str* instrucoes = lerInstrucoes("instructions.txt", &total_instrucoes);
    if (!instrucoes) {
        printf("Erro ao ler instruções.\n");
        return 1;
    }

    // Executar as instruções
    iniciar(instrucoes, processor, ram, cache, total_instrucoes);

    // Liberar memória
    free(instrucoes);
    ram = ramLibera(ram);
    free(processor);
    free(cache);

    return 0;
}

instrucao_str* lerInstrucoes(const char* arquivo, int* total_instrucoes) {
    FILE *f = fopen(arquivo, "r");
    if (!f) {
        perror("Erro ao abrir o arquivo");
        return NULL;
    }

    // Contar o número de linhas (instruções) no arquivo
    int count = 0;
    char ch;
    while ((ch = fgetc(f)) != EOF) {
        if (ch == '\n') 
            count++;
    }
    rewind(f); // Voltar ao início do arquivo

    // Alocar memória para as instruções
    instrucao_str* instrucoes = malloc(count * sizeof(instrucao_str));
    if (!instrucoes) {
        perror("Erro ao alocar memória");
        fclose(f);
        return NULL;
    }

    // Ler cada linha e converter em uma instrução
    for (int i = 0; i < count; i++) {
        fscanf(f, "%d:%d:%d:%d:%d:%d:%d",
               &instrucoes[i].opcode,
               &instrucoes[i].addr1.endbloco,
               &instrucoes[i].addr1.endpalavra,
               &instrucoes[i].addr2.endbloco,
               &instrucoes[i].addr2.endpalavra,
               &instrucoes[i].addr3.endbloco,
               &instrucoes[i].addr3.endpalavra);
    }

    fclose(f);
    *total_instrucoes = count;
    return instrucoes;
}
