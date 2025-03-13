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
    //printf("Estou em CHANGE LRU!!!");
    switch (memory) {
		int i;
        case L1:
            for (i = 0; cache->cacheUsadol1[i] != indice; ++i);
            //printf("Achou i de LRU em %d\n\n", i);
            for (i = 0 ; i < (L1_MAX - 1); i++)
                cache->cacheUsadol1[i] = cache->cacheUsadol1[i+1];
            cache->cacheUsadol1[L1_MAX-1] = indice;
        break;

        case L2:
            for (i = 0; cache->cacheUsadol2[i] != indice; ++i);
            for (i = 0; i < (L2_MAX - 1); i++)
                cache->cacheUsadol2[i] = cache->cacheUsadol2[i+1];
            cache->cacheUsadol2[L2_MAX-1] = indice;
        break;

        case L3:
            for (i = 0; cache->cacheUsadol3[i] != indice; ++i);
            for (i = 0; i < (L3_MAX - 1); i++)
                cache->cacheUsadol3[i] = cache->cacheUsadol3[i+1];
            cache->cacheUsadol3[L3_MAX-1] = indice;
        break;
		default:
		break;

    }
}

// void imprimir_cache(caches *cache, selecionaMemoria memory){
//     switch (memory) {
//         case L1:
// 			printf("CACHE HITS:   %d\n", cache->cacheHitl1);
// 			printf("CACHE MISSES: %d\n\n", cache->cacheMissl1);
//             for (int i = 0; i < L1_MAX; ++i) {
//                 printf("Bloco %d\n", i);
//                 printf("Endereco: %d\n", cache->cachel1[i].blocoEndereco);
//                 printf("modificado: %d\n", cache->cachel1[i].modificado);
//                 printf("Palavras: %d - %d - %d - %d\n", cache->cachel1[i].palavras[0], cache->cachel1[i].palavras[1], cache->cachel1[i].palavras[2], cache->cachel1[i].palavras[3]);

//             }
//             break;
//         case L2:
//             for (int i = 0; i < L2_MAX; ++i) {
//                 printf("Bloco %d\n", i);
//                 printf("Endereco: %d\n", cache->cachel2[i].blocoEndereco);
//                 printf("modificado: %d\n", cache->cachel2[i].modificado);
//                 printf("CACHE HITS: %d\n", cache->cacheHitl2);
//                 printf("Palavras: %d - %d - %d - %d\n", cache->cachel2[i].palavras[0], cache->cachel2[i].palavras[1], cache->cachel2[i].palavras[2], cache->cachel2[i].palavras[3]);
//             }
//             break;
//         case L3:
//             for (int i = 0; i < L3_MAX; ++i) {
//                 printf("Bloco %d\n", i);
//                 printf("Endereco: %d\n", cache->cachel3[i].blocoEndereco);
//                 printf("modificado: %d\n", cache->cachel3[i].modificado);
//                 printf("CACHE HITS: %d\n", cache->cacheHitl3);
//                 printf("Palavras: %d - %d - %d - %d\n", cache->cachel3[i].palavras[0], cache->cachel3[i].palavras[1], cache->cachel3[i].palavras[2], cache->cachel3[i].palavras[3]);
//             }
//             break;
// 		default:
// 		    break;

//     }
//     printf("Custo Total: %d\n", cache->custo);

// } 