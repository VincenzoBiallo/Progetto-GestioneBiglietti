#ifndef utenti_flag
#define utenti_flag

#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct utente {
    char nickname[16];
    char email[64];
    char cognome[20];
    char nome[20];
    int genere;
    data dataNascita;
    char telefono[20];
    int genereMusicale;
} utente;

typedef struct utenti {
    utente* array;
    int size;
} utenti;

int caricaUtenti(utenti* utenti);
int salvaUtenti(utenti* utenti);

int aggiungiUtente(utenti* utenti);
void visualizzaUtente(utenti* utenti , utente* entity);
void visualizzaUtenti(utenti* utenti);
int modificaUtente(utenti* utenti);

utente* ricercaUtente(utenti* utenti, char* nickname);
int esisteUtente(utenti* utenti, char* nickname);

void mergeUtenti(utenti* utenti, int inizio, int meta, int fine);

#endif