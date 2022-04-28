#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./libs/utils.h"

#include "./libs/utenti.h"
#include "./libs/locali.h"
#include "./libs/eventi.h"
#include "./libs/prenotazioni.h"

int visualizzaMenu();
void eseguiDiagnostica(utenti* utenti, locali* locali, eventi* eventi, prenotazioni* prenotazioni, struct index* index);

/**
 * La funzione main è la funzione principale, carica i dati dei file in memoria e salva i dati in memoria sui file. 
 * Visualizza il menù di navigazione principale.
 * 
 * @return -1 : se i file utilizzati dal programma non sono instalalti correttamente.
 * @return 0 : se il programma si chiude con successo.
 * 
 */
int main() {

	struct index indexs;
	caricaIndexs(&indexs);

    utenti utenti;
	int loadUtenti = caricaUtenti(&utenti);

    locali locali;
	int loadLocali = caricaLocali(&locali, &indexs);

    eventi eventi;
	int loadEventi = caricaEventi(&eventi, &indexs, &locali);

    prenotazioni prenotazioni;
	int loadPrenotazioni = caricaPrenotazioni(&prenotazioni, &indexs);

	if ((loadUtenti == -1) || (loadLocali == -1) || (loadEventi == -1) || (loadPrenotazioni == -1)) {
		printf("[!] Almeno un file non era correttamente installato!\n");
		printf("[!] Installazione completata con successo! Riavvia il programma.\n");
		system("PAUSE");
		exit(-1);
	}

	int scelta;

	do {
		eseguiDiagnostica(&utenti, &locali, &eventi, &prenotazioni, &indexs);
		setColor(DEFAULT);
		scelta = visualizzaMenu();
		int result;
		switch(scelta) { 	
			// Aggiungi Utente
			case 1:
				result = aggiungiUtente(&utenti);
				if (result == 1) {
					salvaUtenti(&utenti);
					setColor(VERDE);
					printf("[!] Utente salvato con Successo!\n");
					setColor(DEFAULT);
				} else if (result == -1) {
					setColor(GIALLO);
					printf("[!] L'utente e' gia' presente in memoria!\n");
					setColor(DEFAULT);
				} else {
					free(utenti.array);
					caricaUtenti(&utenti);
				}
				break;
			
			// Ricerca Utente
			case 2:
				visualizzaUtente(&utenti, NULL);
				break;

			// Modifica Utente
			case 3:
				result = modificaUtente(&utenti);
				if (result == 1) {				
					salvaUtenti(&utenti);
					setColor(VERDE);
					printf("[!] Utente modificato con Successo!\n");
					setColor(DEFAULT);
				} else if (result == 0) {
					setColor(ROSSO);
					printf("[!] L'utente non esiste!\n");
					setColor(DEFAULT);
				} else {
					setColor(ROSSO);
					printf("[!] Vettore non inizializzato!\n");
					setColor(DEFAULT);
				}
				break;  	
			case 4:
				visualizzaUtenti(&utenti);				
				break;
			case 5:
				result = aggiungiEvento(&eventi, &locali);
				if (result == 1) {
					salvaEventi(&eventi, &indexs, &locali);
					salvaIndexs(&indexs);
					setColor(VERDE);
					printf("[!] Evento salvato con Successo!\n");
					setColor(DEFAULT);
				} else {
					setColor(ROSSO);
					printf("[!] Vettore non inizializzato!\n");
					setColor(DEFAULT);
				}
				break;
			case 6:
				visualizzaEvento(&eventi, NULL, &locali);
				break;
			case 7:
				result = modificaEvento(&eventi, &locali);
				if (result == 1) {				
					salvaEventi(&eventi, &indexs, &locali);
					setColor(VERDE);
					printf("[!] Evento modificato con Successo!\n");
					setColor(DEFAULT);
				} else if (result == 0) {
					setColor(ROSSO);
					printf("[!] L'evento non esiste!\n");
					setColor(DEFAULT);
				} else {
					setColor(ROSSO);
					printf("[!] Vettore non inizializzato!\n");
					setColor(DEFAULT);
				}
				break;

			// Elimina Evento
			case 8:
				result = eliminaEvento(&eventi, NULL, &locali);
				if (result == 1) {
					salvaEventi(&eventi, &indexs, &locali);
					setColor(VERDE);
					printf("\n[!] Evento eliminato con Successo!\n\n");
					setColor(DEFAULT);
				} else if (result == 0) {
					setColor(ROSSO);
					printf("\n[!] L'evento ricercato non Esiste!\n\n");
					setColor(DEFAULT);
				} else if (result == -1) {
					setColor(ROSSO);
					printf("\n[!] Vettore non inizializzato!\n\n");
					setColor(DEFAULT);
				}
				break;
			case 9:
				visualizzaSpettatoriEvento(&prenotazioni, &eventi, &utenti, &locali);
				break;
			case 10:
				visualizzaIncassoEvento(&eventi, &prenotazioni, &locali);
				break;
			case 11:
				visualizzaDisponibilitaSala(&prenotazioni, &eventi, NULL, &locali);
				break;
			case 12:
				result = vendiBiglietto(&eventi, &locali, &utenti, &prenotazioni);
				if (result == 1) {
					salvaPrenotazioni(&prenotazioni, &indexs);
					salvaIndexs(&indexs);
					setColor(VERDE);
					printf("[!] Biglietto venduto con successo!\n");
					setColor(DEFAULT);
				} else if (result == 0) {
					caricaIndexs(&indexs);
					setColor(GIALLO);
					printf("[!] Vendita biglietto annullata!\n"); 
					setColor(DEFAULT);
				} else if (result == 2){
					caricaIndexs(&indexs);
                	setColor(ROSSO);
                	printf("[!] Hai selezionato un evento SOLD OUT! Ritornerai nel menu' principale.\n");
                	setColor(DEFAULT);
				}else{
					free(prenotazioni.array);
					caricaPrenotazioni(&prenotazioni, &indexs);
				}
				break;
			case 13:
				visualizzaEventiUtente(&prenotazioni, &eventi, &utenti, &locali);
				break;

			// Aggiungi Locali
			case 14:
				result = aggiungiLocale(&locali);
				if (result = 1) {					
					salvaLocali(&locali, &indexs);
					salvaIndexs(&indexs);
					setColor(VERDE);
					printf("Locale aggiunto con successo!\n");
					setColor(DEFAULT);
				} else
					caricaIndexs(&indexs);				
				break;

			// Visualizza Locale
			case 15:
				visualizzaLocale(&locali, NULL, 0);
				break;

			// Modifica Locale
			case 16:
				result = modificaLocale(&locali);
				if (result == 1) {
					salvaLocali(&locali, &indexs);
					setColor(VERDE);
					printf("\nLocale modificato con successo!\n\n");
					setColor(DEFAULT);
				} else if (result == 0) {
					setColor(ROSSO);
					printf("\n[!] Il locale ricercato non esiste!\n\n");
					setColor(DEFAULT);
				} else {
					setColor(ROSSO);
					printf("\n[!] Vettore non inizializzato!\n\n");
					setColor(DEFAULT);
				}
				break;
			
			// Elimina Locale
			case 17:
				result = eliminaLocale(&locali);
				if (result == 0) {
					setColor(ROSSO);
					printf("\n[!] Il locale ricercato non esiste!\n\n");
					setColor(DEFAULT);
				} else if (result == -1) {
					setColor(ROSSO);
					printf("\n[!] Vettore non inizializzato!\n\n");
					setColor(DEFAULT);
				} else if (result != -2) {					
					mergeSort(&locali, _locali, 0, locali.size - 1);
					salvaLocali(&locali, &indexs);
					caricaEventi(&eventi, &indexs, &locali); // Aggiorna i puntatori ai locali
					setColor(VERDE);
					printf("\nLocale eliminato con successo!\n\n");
					setColor(DEFAULT);
				}
			break;

			// Chiudi Software
			case 18:
				free(prenotazioni.array);
				free(eventi.array);
				free(locali.array);
				free(utenti.array);
				exit(0);
				break;

			default:
				setColor(ROSSO);
				printf("[!] L'opzione indicata non e' valida!\n");
				setColor(DEFAULT);
				break;   	
		}

		system("PAUSE");

	} while(scelta != 18);  
	
    return 0;
}


/**
 * 
 * La funzione visualizza il menù di navigazione principale.
 * 
 * @return scelta_menu: variabile utilizzata per scegliere un'opzione del menù. Il suo valore sarà l'opzione scelta dall'utente.
 * 
 *
*/
int visualizzaMenu() {

	fflush(stdin);
    unsigned int scelta_menu = 0;
	unsigned int check = 0;

	system("CLS");
	setColor(GIALLO);
    printf("\
			\t\n************************************************************\
			\t\n*                           MENU'                          *\
			\t\n************************************************************");

	setColor(VERDE);
	printf("\n\n GESTIONE UTENTI");
	setColor(DEFAULT);
	printf("	\n\t 1. Registra nuovo utente\
				\n\t 2. Ricerca utente\
				\n\t 3. Modifica utente\
				\n\t 4. Visualizza tutti gli utenti");
	setColor(VIOLA);
	printf("\n\n GESTIONE EVENTI");
	setColor(DEFAULT);
	printf("	\n\t 5. Aggiungi nuovo evento\
				\n\t 6. Ricerca un evento\
				\n\t 7. Modifica evento\
				\n\t 8. Elimina evento\
				\n\t 9. Visualizza spettatori di un evento\
				\n\t 10. Visualizza incasso evento");
	setColor(CELESTE);
	printf("\n\n GESTIONE PRENOTAZIONI");
	setColor(DEFAULT);
	printf("	\n\t11. Visualizza disponibilita' sala\
				\n\t12. Vendi biglietto\
				\n\t13. Visualizza eventi a cui ha partecipato un utente");
	setColor(ROSSO);
	printf("\n\n GESTIONE LOCALI");
	setColor(DEFAULT);
	printf("	\n\t14. Aggiungi locale\
				\n\t15. Ricerca locale\
				\n\t16. Modifica locale\
				\n\t17. Elimina locale\
                \n\n\t18. Esci dal programma\n");

	do {
		printf("\nScegli un'opzione: ");
		check = scanf("%u", &scelta_menu);
		if(check!=1 || scelta_menu < 1 || scelta_menu > 18) {
			fflush(stdin);
			setColor(ROSSO);
			printf("[!] L'opzione indicata non e' valida.");
			setColor(DEFAULT);
		}
	} while (check!=1 || scelta_menu < 1 || scelta_menu > 18);

	fflush(stdin);

    return scelta_menu;

}

/**
 * La funzione esegue un controllo di integrità delle inforazioni.
 * 
 * @param [in] utenti : struct contentente l'indirizzo all'array di struct (utilizzato per effettuare operazioni concernenti gli utenti) e la gradezza della stessa.
 * @param [in] locali: struct contentente, tra gli altri campi, l'indirizzo all'array di struct utilizzato per effettuare operazioni concernenti ai locali.
 * @param [in] eventi : struct contentente (tra i vai campi) l'indirizzo all'array di struct utilizzato per effettuare operazioni concernenti gli eventi. 
 * @param [in] prenotazioni : struct contenente, tra gli altri campi, l'indirizzo all'array di struct utilizzato per effettuare operazioni concernenti alle prenotazioni.
 * @param [in] indexs : 
 * 
 */
void eseguiDiagnostica(utenti* utenti, locali* locali, eventi* eventi, prenotazioni* prenotazioni, struct index* indexs) {
	
	system("cls");
	printf("[!] Avvio del Sistema di Controllo integrita' informazioni...\n\n");

	int error, error2;
	int alarm = 0;

	utente* utente = NULL;
	evento* evento = NULL;
	locale* locale = NULL;

	// DIAGNOSTICA EVENTI
	
	for (int e=0; e<eventi->size; e++) {
		locale = ricercaLocaleByCodice(locali, eventi->array[e].locale);

		if (locale == NULL) {
			alarm = 1;
			printf("L'evento con Codice '%d' e con Nome '%s' non possiede un Locale!\n", eventi->array[e].codice, eventi->array[e].nome);
			
			int scelta = -1;

			do {
				printf("Come vuoi Proseguire? (0 : Elimina | 1 : Aggiungi) : ");	
				error = scanf("%d", &scelta);
				if (error != 1 || scelta < 0 || scelta > 1) {
					fflush(stdin);
					printf("[!] Puoi inserire solo un numero tra quelli indicati.\n");
					error=0;
				}
			} while (scelta < 0 || scelta > 1 || error != 1);
			fflush(stdin);

			if (scelta) {
				visualizzaLocali(locali, 1);		
				do {

					printf("Inserisci il Codice di un Locale: ");	
					error = scanf("%d", &scelta);	

					locale = ricercaLocaleByCodice(locali, scelta);

					if (locale == NULL || error != 1){
						fflush(stdin);
						printf("[!] Puoi inserire solo il codice di locale indicato.\n");
					}
					
				} while (locale == NULL || error !=1);
				fflush(stdin);

				eventi->array[e].locale = locale->codice;
			} else {
				eliminaEvento(eventi, &eventi->array[e], locali);
			}
		}
	}

	printf("\n\n");

	locale = NULL;

	// DIAGNOSTICA PRENOTAZIONI
	for (int i=0; i<prenotazioni->size; i++) {
		for (int p=0; p<prenotazioni->size; p++) {
			utente = ricercaUtente(utenti, prenotazioni->array[p].nickname);
			evento = ricercaEventoByCodice(eventi, prenotazioni->array[p].evento);

			if ((utente == NULL) || (evento == NULL)) {
				printf("\n\n[!] Il biglietto numero '%d' risulta non valido in quanto l'utente non esiste piu' oppure l'evento e' stato cancellato!\n", prenotazioni->array[p].numero);
				printf("Il biglietto e' stato rimborsato e annullato! Importo rimborsato: %.2f\n\n", prenotazioni->array[p].importo);
				rimborsaBiglietto(prenotazioni, ricercaPrenotazioneByNumero(prenotazioni, prenotazioni->array[p].numero));
				alarm = 1;
				system("PAUSE");
				continue;
			}

			locale = ricercaLocaleByCodice(locali, evento->locale);
			if ((prenotazioni->array[p].fila > locale->file) || (prenotazioni->array[p].n_poltrona > locale->posti)) {
				printf("\n\n[!] Il biglietto numero '%d' si riferisce ad un posto non piu' presente all'interno dell'evento (a causa di un cambio di locale)!\n", prenotazioni->array[p].numero);
				printf("Il biglietto e' stato rimborsato e annullato! Importo rimborsato: %.2f\n\n", prenotazioni->array[p].importo);
				rimborsaBiglietto(prenotazioni, ricercaPrenotazioneByNumero(prenotazioni, prenotazioni->array[p].numero));
				alarm = 1;
				system("PAUSE");
			}
		}
	}

	if (alarm) {
		printf("[!] Sto applicando i cambiamenti...");
		salvaUtenti(utenti);
		salvaLocali(locali, indexs);
		salvaEventi(eventi, indexs, locali);		
		salvaPrenotazioni(prenotazioni, indexs);	
	}

	alarm = 0;
}