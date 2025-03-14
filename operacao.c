#include "sistema.h"
#include <stdlib.h>
#include <stdio.h>

void iniciar(instrucao_str *programa, cpu *processor, RAM *RAM, caches *cache, int total_instrucoes) {
    processor->programa = programa;
    processor->PC = 0;
    processor->opcode = 0;

    // Processar apenas o número de instruções especificado
    for (int i = 0; i < total_instrucoes; i++) {
        instrucao_str *inst = &processor->programa[processor->PC];
        processor->opcode = inst->opcode;

        // Verificar se a instrução é HALT
        if (inst->opcode == HALT) {
            printf("Programa terminou!!\n");
            imprimir(RAM);
            break; // Parar a execução
        }

        // Processar a instrução
        processor->reg1 = memoryGet(RAM, inst->addr1, cache);
        processor->reg2 = memoryGet(RAM, inst->addr2, cache);
        processor->reg3 = memoryGet(RAM, inst->addr3, cache);

        switch (processor->opcode) {
            case SOMA: // SOMA
                processor->reg3->palavras[inst->addr3.endpalavra] = processor->reg1->palavras[inst->addr1.endpalavra] + processor->reg2->palavras[inst->addr2.endpalavra];
                processor->reg3->modificado = 1;
                // printf("Inst sum -> RAM posicao %d com conteudo na cache 1 %d\n", inst->addr3.endpalavra, processor->reg3->palavras[inst->addr3.endpalavra]);
                // printf("Custo até o momento: %d\n", cache->custo);
                // printf("Até o momento HITS C1: %d MISSES C1: %d\n", cache->cacheHitl1, cache->cacheMissl1);
                // printf("Até o momento HITS C2: %d MISSES C2: %d\n", cache->cacheHitl2, cache->cacheMissl2);
                // printf("Até o momento HITS C3: %d MISSES C3: %d\n\n", cache->cacheHitl3, cache->cacheMissl3);
                break;
            case SUBTRAI: // SUBTRAI
                processor->reg3->palavras[inst->addr3.endpalavra] = processor->reg1->palavras[inst->addr1.endpalavra] - processor->reg2->palavras[inst->addr2.endpalavra];
                processor->reg3->modificado = 1;
                // printf("Inst sub -> RAM posicao %d com conteudo na cache 1 %d\n", inst->addr3.endpalavra, processor->reg3->palavras[inst->addr3.endpalavra]);
                // printf("Custo até o momento: %d\n", cache->custo);
                // printf("Até o momento HITS C1: %d MISSES C1: %d\n", cache->cacheHitl1, cache->cacheMissl1);
                // printf("Até o momento HITS C2: %d MISSES C2: %d\n", cache->cacheHitl2, cache->cacheMissl2);
                // printf("Até o momento HITS C3: %d MISSES C3: %d\n\n", cache->cacheHitl3, cache->cacheMissl3);
                break;
            default:
                printf("Instrução desconhecida: %d\n", processor->opcode);
                break;
        }
        processor->PC += 1; // Avançar para a próxima instrução
    }
    
    printf("\n");
    printf("*----------------------------------------------------------------------* \n");
    printf("|                   HITS C1: %-10d MISSES C1: %-20d|\n", cache->cacheHitl1, cache->cacheMissl1);
    printf("|                   HITS C2: %-10d MISSES C2: %-20d| \n", cache->cacheHitl2, cache->cacheMissl2);
    printf("|                   HITS C3: %-10d MISSES C3: %-20d|\n", cache->cacheHitl3, cache->cacheMissl3);   
    printf("*----------------------------------------------------------------------* \n");

    printf("\n");
    printf("*-----------------------------------*\n");
    printf("|    Custo total: %-18d|\n", cache->custo);
    printf("*-----------------------------------*\n");
}