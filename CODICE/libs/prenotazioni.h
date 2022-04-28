#ifndef prenotazioni_flag
#define prenotazioni_flag

#include "utils.h"
#include "eventi.h"
#include "locali.h"
#include "utenti.h"

typedef struct prenotazione {
    int numero;
    int evento;
    char nickname[16];
    int fila;
    int n_poltrona;
    float importo;
} prenotazione;

typedef struct prenotazioni {
    prenotazione* array;
    int size;
    int* index;
} prenotazioni;

int caricaPrenotazioni(prenotazioni* prenotazioni, struct index* indexs);
int salvaPrenotazioni(prenotazioni* prenotazioni, struct index* indexs);

void visualizzaBiglietto(char* nomeEvento, int nBiglietto, data dataEvento, char* nickname, int nFila, int nPoltrona, char* nomeLocale, float importo);
void visualizzaDisponibilitaSala(prenotazioni* prenotazioni, eventi* eventi, evento* entity, locali* locali);
int vendiBiglietto(eventi* eventi, locali* locali, utenti* utenti, prenotazioni* prenotazioni);
int rimborsaBiglietto(prenotazioni* prenotazioni, prenotazione* entity);

int differenzaDate(data data, struct data* entity);

prenotazione* ricercaPrenotazione(prenotazioni* prenotazioni, int fila, int posto, int codiceEvento);
prenotazione* ricercaPrenotazioneByNumero(prenotazioni* prenotazioni, int numero);

void visualizzaSpettatoriEvento(prenotazioni* prenotazioni, eventi* eventi, utenti* utenti, locali* locali);
void visualizzaEventiUtente(prenotazioni* prenotazioni, eventi* eventi, utenti* utenti, locali* locali);
void visualizzaIncassoEvento(eventi* eventi, prenotazioni* prenotazioni, locali* locali);

void ordinaEventi(eventi* eventi, int* memory, const unsigned int size);



#endif