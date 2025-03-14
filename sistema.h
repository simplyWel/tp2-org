#ifndef SISTEMA_H
#define SISTEMA_H

#define MAX_PALAVRAS 4
#define HALT -1
#define SOMA 0
#define SUBTRAI 1
#define DEFAULT 30
#define L1_MAX 10
#define L2_MAX 20
#define L3_MAX 30
#define N_INST 100 // Número de instruções
#define N_FOR 5      // Número de instruções que vão se repetir
#define N_MEM 1000   // Número da memória para endereço do bloco
#define N_PROB 50    // Probabilidade de repetição
#define N_OPCODE 2   // Número máximo de opcodes (0: SOMA, 1: SUBTRAI)
#define N_WORD 4     // Número de palavras no bloco

typedef struct {
    int palavras[MAX_PALAVRAS];
    int blocoEndereco;
    int modificado;
} memoriaBloco;

typedef struct {
    memoriaBloco *blocks;
    int size;

} RAM;

typedef enum {
    L1,
    L2,
    L3,
    RAM_MEMORY,
    MISS

} selecionaMemoria;

typedef struct {
    int endbloco;
    int endpalavra;
} endereco; 

typedef struct {
    endereco addr1;
    endereco addr2;
    endereco addr3;
    int opcode;

} instrucao_str;

typedef struct {
    memoriaBloco *reg1, *reg2, *reg3;
    int PC;
    
    instrucao_str *programa;
    int opcode;


} cpu;

typedef struct {
    memoriaBloco cachel1[L1_MAX]; int cacheUsadol1[L1_MAX]; int cacheHitl1; int cacheMissl1;
    memoriaBloco cachel2[L2_MAX]; int cacheUsadol2[L2_MAX]; int cacheHitl2; int cacheMissl2;
    memoriaBloco cachel3[L3_MAX]; int cacheUsadol3[L3_MAX]; int cacheHitl3; int cacheMissl3;
    int custo;
} caches; 

void iniciar_cache(caches *cache);
void change_lru(caches *cache, selecionaMemoria memory, int indice);

void iniciar(instrucao_str *programa, cpu *processor, RAM *RAM, caches *cache, int total_instrucoes);

int blocoVazio (caches *cache, selecionaMemoria cache_looking);
memoriaBloco *memoryMove (RAM *ram, caches *cache, int blocoEndereco, selecionaMemoria begins);
int encontraMemoria(RAM *ram, caches *cache, endereco e, selecionaMemoria choice);
memoriaBloco *memoryGet (RAM *ram, endereco e, caches *cache);

RAM* ramAloca (int tam);
RAM* ramLibera(RAM *ram);
void ramEhvazia (RAM *ram);
void sorteiaRam (RAM *ram);
void imprimir (RAM *ram);
instrucao_str* lerInstrucoes(const char* arquivo, int* total_instrucoes);
void instGenerator();
#endif