#include "sistema.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>


RAM * ramAloca (int size){
	RAM *ram; ram = malloc (sizeof(RAM));
	ram->blocks = malloc(sizeof(memoriaBloco) * size);
	for (int i = 0; i < size; ++i){
		ram->blocks[i].modificado  = 0;
	}
	ram->size = size;
	return ram;
}


RAM* ramLibera(RAM *ram) {
	free(ram->blocks);
	free(ram);

	return NULL;

}
void ramEhvaziaa (RAM *ram) {

	for (int i = 0; i < ram->size; i++){
		ram->blocks[i].blocoEndereco = i;
		for (int j = 0; j < MAX_PALAVRAS; ++j){
			ram->blocks[i].palavras[j] = 0;
		}
	}
}
void sorteiaRam (RAM *ram){

	srand(time(NULL));

	for (int i = 0; i < ram->size; ++i){
		ram->blocks[i].blocoEndereco = i;
		for (int j = 0; j < MAX_PALAVRAS; ++j){
				ram->blocks[i].palavras[j] = rand() % 101;
		}
	}
}
void imprimir (RAM *ram){
	for (int i = 0; i < ram->size; ++i) {
		for (int j = 0; j < MAX_PALAVRAS; ++j) {
			printf("%d  ", ram->blocks[i].palavras[j]);
		}
		printf("\n");
	}
}