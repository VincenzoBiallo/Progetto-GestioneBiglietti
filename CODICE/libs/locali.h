#ifndef locali_flag
#define locali_flag

#include "utils.h"

typedef struct locale {
    int codice;
    char nome[20];
    int file;
    int posti;
} locale;

typedef struct locali {
    locale* array;
    int size;
    int* index;
} locali;

int caricaLocali(locali* locali, struct index* indexs);
int salvaLocali(locali* locali, struct index* indexs);

void visualizzaLocale(locali* locali, locale* entity, int diagnostica);
void visualizzaLocali(locali* locali, int diagnostica);

int aggiungiLocale(locali* locali);
int modificaLocale(locali* locali);
int eliminaLocale(locali* locali);

locale* ricercaLocale(locali* locali, char* nome);
locale* ricercaLocaleByCodice(locali* locali, int codice);

void mergeLocali(locali* locali, int inizio, int meta, int fine);

#endif