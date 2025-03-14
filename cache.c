#include "sistema.h"
#include <stdio.h>

void iniciar_cache(caches *cache) {
    cache->custo = 0;
    cache->cacheHitl1 = 0; cache->cacheMissl1 = 0;
    cache->cacheHitl2 = 0; cache->cacheMissl2 = 0;
    cache->cacheHitl3 = 0; cache->cacheMissl3 = 0;
    for (int i = 0; i < L1_MAX; ++i) {
        cache->cachel1[i].modificado = 0;
        cache->cachel1[i].blocoEndereco = -1;
        cache->cacheUsadol1[i]  = (L1_MAX-i-1);
    }

    for (int i = 0; i < L2_MAX; ++i) {
        cache->cachel2[i].modificado = 0;
        cache->cachel2[i].blocoEndereco = -1;
        cache->cacheUsadol2[i]  = (L2_MAX-i-1);
    }
    for (int i = 0; i < L3_MAX; ++i) {
        cache->cachel3[i].modificado = 0;
        cache->cachel3[i].blocoEndereco = -1;
        cache->cacheUsadol3[i]  = (L3_MAX-i-1);
    }
}

void change_lru(caches *cache, selecionaMemoria memory, int indice) {
    switch (memory) {
        case L1:
            for (int i = 0; i < L1_MAX; i++) {
                if (cache->cacheUsadol1[i] == indice) {
                    // Move o índice para o final da lista (mais recentemente usado)
                    for (int j = i; j < L1_MAX - 1; j++) {
                        cache->cacheUsadol1[j] = cache->cacheUsadol1[j + 1];
                    }
                    cache->cacheUsadol1[L1_MAX - 1] = indice;
                    break;
                }
            }
            break;
        case L2:
            for (int i = 0; i < L2_MAX; i++) {
                if (cache->cacheUsadol2[i] == indice) {
                    // Move o índice para o final da lista (mais recentemente usado)
                    for (int j = i; j < L2_MAX - 1; j++) {
                        cache->cacheUsadol2[j] = cache->cacheUsadol2[j + 1];
                    }
                    cache->cacheUsadol2[L2_MAX - 1] = indice;
                    break;
                }
            }
            break;
        case L3:
            for (int i = 0; i < L3_MAX; i++) {
                if (cache->cacheUsadol3[i] == indice) {
                    // Move o índice para o final da lista (mais recentemente usado)
                    for (int j = i; j < L3_MAX - 1; j++) {
                        cache->cacheUsadol3[j] = cache->cacheUsadol3[j + 1];
                    }
                    cache->cacheUsadol3[L3_MAX - 1] = indice;
                    break;
                }
            }
            break;
        default:
            break;
    }
}