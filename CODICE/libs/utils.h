#ifndef utils_flag
#define utils_flag

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct index {
    unsigned int evento;
    unsigned int locale;
    unsigned int prenotazione;
};

typedef struct data {
    int giorno;
    int mese;
    int anno;
} data;

typedef struct orario {
    int ora;
    int minuto;
} orario;

void caricaIndexs(struct index* indexs);
void salvaIndexs(struct index* indexs);

int contaLinee(FILE* filePointer);
int stringaValida(const char string[], const char* alphabet);
char* myParser(char** newString, char* delimiter);
char* myLower(const char string[]);

enum colori {DEFAULT, ROSSO, GIALLO, VERDE, CELESTE, VIOLA};
void setColor(enum colori colore);
void inizializzaStringa(char stringa[], int size);
int contaCifre(int numero);
float differenzaOrari(orario inizio, orario fine);
int inserisciData(data* memory);

enum TypeMerge {_utenti, _eventi, _locali, _prenotazioni};
void mergeSort(void* memory, enum TypeMerge type, int inizio, int fine);

#endif