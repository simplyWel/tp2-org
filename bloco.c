#include <stdio.h>
#include "sistema.h"

int blocoVazio (caches *cache, selecionaMemoria cacheVigia) { //responsável por procurar blocos vazios

    int i;
    switch (cacheVigia) {
        case L1:    
            for (i = 0; cache->cachel1[i].blocoEndereco != -1 && i < L1_MAX; i++);
            return (i < L1_MAX) ? i : -1;
            break;
        case L2:
            for (i = 0; cache->cachel2[i].blocoEndereco != -1 && i < L2_MAX;i++);
            return (i < L2_MAX) ? i : -1;
            break;
        case L3:
            for (i = 0; cache->cachel3[i].blocoEndereco != -1 && i < L3_MAX; i++);
            return (i < L3_MAX) ? i : -1;
            break;
        default:
        break;

    }
	return -1;

}
memoriaBloco* memoryMove (RAM *ram, caches *cache, int blocoEndereco, selecionaMemoria begins) {
    if (begins == L1) {
        change_lru(cache, L1, blocoEndereco);
        printf("Movendo para o registrador\n");
        return &cache->cachel1[blocoEndereco]; 
    }

    if (begins == L2) {
        int bloco = blocoVazio(cache, L1);
        if (bloco != -1) {
            cache->cachel1[bloco] = cache->cachel2[blocoEndereco];
            cache->cachel2[blocoEndereco].blocoEndereco = -1;
            printf("Movendo para L1\n");
            memoryMove(ram, cache, bloco, L1);
        }else {
            memoriaBloco tmp = cache->cachel1[cache->cacheUsadol1[0]];
			int tmp_indice = cache->cacheUsadol1[0];
            cache->cachel1[cache->cacheUsadol1[0]] = cache->cachel2[blocoEndereco];
            cache->cachel2[blocoEndereco] = tmp;
            change_lru(cache, L1, cache->cacheUsadol1[0]);
            change_lru(cache, L2, blocoEndereco);
			memoryMove(ram, cache, tmp_indice, L1);
            printf("Movendo bloco Endereco %d de L2 para o bloco de endereco %d de L1:", blocoEndereco, cache->cacheUsadol1[0]);
            printf("Resultado: %d\n", cache->cachel1[cache->cacheUsadol1[0]].palavras[0]);
        }
    }
    if (begins == L3) {
        int bloco = blocoVazio(cache, L2);
        if (bloco != -1) {
            cache->cachel2[bloco] = cache->cachel3[blocoEndereco];
            cache->cachel3[blocoEndereco].blocoEndereco = -1;
            printf("Movendo para L2\n");
            memoryMove(ram, cache, bloco, L2);
        }else {
            memoriaBloco tmp = cache->cachel2[cache->cacheUsadol2[0]];
			int tmp_indice = cache->cacheUsadol2[0];
            cache->cachel2[cache->cacheUsadol2[0]] = cache->cachel3[blocoEndereco];
            cache->cachel3[blocoEndereco] = tmp;
            change_lru(cache, L2, cache->cacheUsadol2[0]);
            change_lru(cache, L3, blocoEndereco);
			memoryMove(ram, cache, tmp_indice, L2);
        }
    }
    if (begins == RAM_MEMORY) {
        int bloco = blocoVazio(cache,L3);
        if (bloco != -1) {
            cache->cachel3[bloco] = ram->blocks[blocoEndereco];
            printf("Movendo para L3\n");
            memoryMove(ram, cache, bloco, L3);
        }
        else {
            memoriaBloco tmp = cache->cachel3[cache->cacheUsadol3[0]];
            cache->cachel3[cache->cacheUsadol3[0]] = ram->blocks[blocoEndereco];
            if (tmp.modificado == 1)
                ram->blocks[blocoEndereco] = tmp;
			memoryMove(ram, cache, bloco, L3);
            change_lru(cache, L3, cache->cacheUsadol3[0]);
        }
    }   
}

int encontraMemoria(RAM *ram, caches *cache, endereco e, selecionaMemoria choice) {
    switch(choice){
        case  L1:
            for (int i = 0; i < L1_MAX; i++) {
                if (cache->cachel1[i].blocoEndereco == e.endbloco){
                    cache->cacheHitl1 ++;
                    cache->custo += 10;
                    return i;
                }
            }
            break;
        case  L2:
            for (int i = 0; i < L2_MAX; i++) {
                if (cache->cachel2[i].blocoEndereco == e.endbloco){
                    cache->cacheHitl2++;
                    cache->cacheMissl1++;
                    cache->custo += 110;
                    return i;
                }
            }
            break;
        case L3:
            for (int i = 0; i < L3_MAX; i++) {
                if (cache->cachel3[i].blocoEndereco == e.endbloco){
                    cache->cacheMissl1++;
                    cache->cacheMissl2++;
                    cache->cacheHitl3++;
                    cache->custo += 1110;
                    return i;
                }
            }
            break; 
        case RAM_MEMORY:
            for (int i = 0; i < ram->size; i++) {
                if (ram->blocks[i].blocoEndereco == e.endbloco) {
                    cache->cacheMissl1++;
                    cache->cacheMissl2++;
                    cache->cacheMissl3++;
                    cache->custo += 10110;
                    return i;
                }
            }
            break;
        default:
            break;
    }
    return -1;
}
memoriaBloco* memoryGet (RAM *ram, endereco e, caches *cache) {
	selecionaMemoria onde_procurar[] = {L1, L2, L3, RAM_MEMORY, MISS};
    selecionaMemoria memoria_encontrada;

	int end_memoria;
	int i = 0;

	do {
		end_memoria = encontraMemoria(ram, cache, e, onde_procurar[i++]);

	} while(end_memoria == -1 && onde_procurar[i] != MISS);

	memoria_encontrada = onde_procurar[i-1];


    switch (memoria_encontrada) {
        case L1:
            printf("Estou em l1!\n");
            return memoryMove(ram, cache, end_memoria, L1);
            break;
        case L2:
            printf("Estou em l2!\n");
                return memoryMove(ram, cache, end_memoria, L2);
                break;
        case L3:
            printf("Estou rm l3!\n");
                return memoryMove(ram, cache, end_memoria, L3);
                break;
        case RAM_MEMORY:   
            printf("Estou rm ram!\n");
            return memoryMove(ram, cache, end_memoria, RAM_MEMORY );
            break;
        default:
            break;
    }
}