#ifndef eventi_flag
#define eventi_flag

#include "utils.h"
#include "locali.h"

typedef struct evento {
    int codice;
    char nome[50];
    data data;
    orario oraInizio;
    orario oraFine;
    char ospite[50];
    int locale;
    float costo_prima_fila;
    float costo_ultima_fila;
} evento;

typedef struct eventi {
    evento* array;
    int size;
    int* index; 
} eventi;

int caricaEventi(eventi* eventi, struct index* indexs, locali* locali);
int salvaEventi(eventi* eventi, struct index* indexs, locali* locali);

int aggiungiEvento(eventi* eventi, locali* locali);
int modificaEvento(eventi* eventi, locali* locali);
int eliminaEvento(eventi* eventi, evento* entity, locali* locali);

void visualizzaEvento(eventi* eventi, evento* entity, locali* locali);
void visualizzaEventi(eventi* eventi);

evento* ricercaEventoByCodice(eventi* eventi, int codice);
evento* ricercaEventoByData(eventi* eventi , data data, locali* locali);
evento* ricercaEventoByString(eventi* eventi , char* string, int type, locali* locali);
evento* ricercaEvento(eventi* eventi, locali* locali);

#endif