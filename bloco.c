#include <stdio.h>
#include "sistema.h"
#include <stdlib.h>

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
    if (begins == RAM_MEMORY) {
        // Passo 1: Carrega o bloco da RAM para a L3
        int bloco_l3 = blocoVazio(cache, L3);
        if (bloco_l3 != -1) {
            cache->cachel3[bloco_l3] = ram->blocks[blocoEndereco];
            change_lru(cache, L3, bloco_l3);
        } else {
            // Substitui o bloco menos recentemente usado na L3
            int lru_index_l3 = cache->cacheUsadol3[0];
            memoriaBloco tmp_l3 = cache->cachel3[lru_index_l3];
            cache->cachel3[lru_index_l3] = ram->blocks[blocoEndereco];
            if (tmp_l3.modificado == 1) {
                ram->blocks[blocoEndereco] = tmp_l3; // Write-back
            }
            change_lru(cache, L3, lru_index_l3);
            bloco_l3 = lru_index_l3;
        }

        // Passo 2: Move o bloco da L3 para a L2
        int bloco_l2 = blocoVazio(cache, L2);
        if (bloco_l2 != -1) {
            cache->cachel2[bloco_l2] = cache->cachel3[bloco_l3];
            
            change_lru(cache, L2, bloco_l2);
        } else {
            // Substitui o bloco menos recentemente usado na L2
            int lru_index_l2 = cache->cacheUsadol2[0];
            cache->cachel2[lru_index_l2] = cache->cachel3[bloco_l3];
            change_lru(cache, L2, lru_index_l2);
            bloco_l2 = lru_index_l2;
        }

        // Passo 3: Move o bloco da L2 para a L1
        int bloco_l1 = blocoVazio(cache, L1);
        if (bloco_l1 != -1) {
            cache->cachel1[bloco_l1] = cache->cachel2[bloco_l2];
            
            change_lru(cache, L1, bloco_l1);
            return &cache->cachel1[bloco_l1];
        } else {
            // Substitui o bloco menos recentemente usado na L1
            int lru_index_l1 = cache->cacheUsadol1[0];
            cache->cachel1[lru_index_l1] = cache->cachel2[bloco_l2];
            
            change_lru(cache, L1, lru_index_l1);
            return &cache->cachel1[lru_index_l1];
        }
    } else if (begins == L3) {
        // Move o bloco da L3 para a L2
        int bloco_l2 = blocoVazio(cache, L2);
        if (bloco_l2 != -1) {
            cache->cachel2[bloco_l2] = cache->cachel3[blocoEndereco];
            cache->cachel3[blocoEndereco].blocoEndereco = -1;
            change_lru(cache, L2, bloco_l2);
            return &cache->cachel2[bloco_l2];
        } else {
            // Substitui o bloco menos recentemente usado na L2
            int lru_index_l2 = cache->cacheUsadol2[0];
            memoriaBloco tmp_l2 = cache->cachel2[lru_index_l2];
            cache->cachel2[lru_index_l2] = cache->cachel3[blocoEndereco];
            cache->cachel3[blocoEndereco] = tmp_l2;
            change_lru(cache, L2, lru_index_l2);
            change_lru(cache, L3, blocoEndereco);
            return &cache->cachel2[lru_index_l2];
        }
    } else if (begins == L2) {
        // Move o bloco da L2 para a L1
        int bloco_l1 = blocoVazio(cache, L1);
        if (bloco_l1 != -1) {
            cache->cachel1[bloco_l1] = cache->cachel2[blocoEndereco];
            cache->cachel2[blocoEndereco].blocoEndereco = -1;
            change_lru(cache, L1, bloco_l1);
            return &cache->cachel1[bloco_l1];
        } else {
            // Substitui o bloco menos recentemente usado na L1
            int lru_index_l1 = cache->cacheUsadol1[0];
            memoriaBloco tmp_l1 = cache->cachel1[lru_index_l1];
            cache->cachel1[lru_index_l1] = cache->cachel2[blocoEndereco];
            cache->cachel2[blocoEndereco] = tmp_l1;
            change_lru(cache, L1, lru_index_l1);
            change_lru(cache, L2, blocoEndereco);
            return &cache->cachel1[lru_index_l1];
        }
    } else if (begins == L1) {
        // O bloco já está na L1, apenas atualiza o LRU
        change_lru(cache, L1, blocoEndereco);
        return &cache->cachel1[blocoEndereco];
    }

    return NULL; // Retorno padrão para evitar warnings
}
int encontraMemoria(RAM *ram, caches *cache, endereco e, selecionaMemoria choice) {
    switch(choice){
    case L1:
        for (int i = 0; i < L1_MAX; i++) {
            if (cache->cachel1[i].blocoEndereco == e.endbloco){
                cache->cacheHitl1++;
                cache->custo += 10;
                return i;
            }
        }
        break;
    case L2:
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
    int probabilidade = rand() % 100;

    if (probabilidade < 50) {
        // Procura primeiro na L1
        int end_memoria = encontraMemoria(ram, cache, e, L1);
        if (end_memoria != -1) {
            return memoryMove(ram, cache, end_memoria, L1);
        }
    } else if (probabilidade < 80) {
        // Procura primeiro na L2
        int end_memoria = encontraMemoria(ram, cache, e, L2);
        if (end_memoria != -1) {
            return memoryMove(ram, cache, end_memoria, L2);
        }
    } else {
        // Procura primeiro na L3
        int end_memoria = encontraMemoria(ram, cache, e, L3);
        if (end_memoria != -1) {
            return memoryMove(ram, cache, end_memoria, L3);
        }
    }

    // Se não encontrou em nenhuma cache, busca na RAM
    int end_memoria = encontraMemoria(ram, cache, e, RAM_MEMORY);
    if (end_memoria != -1) {
        return memoryMove(ram, cache, end_memoria, RAM_MEMORY);
    }

    return NULL; // Retorno padrão para evitar warnings
}