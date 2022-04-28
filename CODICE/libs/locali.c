#include "locali.h"

/**
 * La funzione apre il file di riferimento e il file degli index, legge riga per riga il file del CSV, le analizza e le inserisce in elementi di un vettore dinamico
 * che viene generato in base al numero di righe che vengono lette dal file. In seguito viene creato un puntatore all'index di AUTO_INCREMENT
 * 
 * @param [in] locali : struct contentente, tra gli altri campi, l'indirizzo all'array di struct utilizzato per effettuare operazioni concernenti ai locali.
 * @param [in] indexs : puntatore a struct index
 * 
 * @return 0 : il vettore è stato caricato con successo
 * @return -1 : impossibile aprire il file
 * 
 */

int caricaLocali(locali* locali, struct index* indexs) {
    
    FILE* fp;
    
    if ((fp = fopen("./files/locali.csv", "r")) != NULL) {

        char buffer[1000];
        int lines = contaLinee(fp);
        if (lines == -1) 
            return -1;    

        locali->size = lines;
        locali->array = calloc(locali->size, sizeof(locale));

        int count = 0;

        // Viene letta ogni riga del file CSV e viene caricata in un buffer temporaneo di 1000 caratteri
		while (fgets(buffer, sizeof(buffer), fp)) {

            char* token;
            char* line = buffer;
            char** input = &line;

            token = myParser(input, ";");
			locali->array[count].codice = atoi(token);   

            token = myParser(input, ";");
			strcpy(locali->array[count].nome, token);

            token = myParser(input, ";");
			locali->array[count].file = atoi(token);   

            token = myParser(input, ";");
			locali->array[count].posti = atoi(token);

            count++;
        }

        locali->index = &indexs->locale;

        fclose(fp); 
    } else {
        setColor(ROSSO);
		printf("[!] Impossibile aprire il file locali.csv\n");
		setColor(DEFAULT);
        FILE* fp = fopen("./files/locali.csv", "w");
        fclose(fp);
        return -1;
    }

    return 0;
}

/**
 * La funzione cancella e ricrea il file di riferimento inserendo una riga per ogni struct nel vettore dinamico. In seguito viene distrutto il vettore e viene richiamata
 * la funzione caricaLocali
 * 
 * @param [in] locali : struct contentente, tra gli altri campi, l'indirizzo all'array di struct utilizzato per effettuare operazioni concernenti ai locali.
 * @param [in] indexs : puntatore a struct index
 * 
 * @return 0 : il vettore è stato salvato su file
 * @return -1 : il vettore dinamico non esiste oppure non è stato possibile salvare il file
 * 
 */

int salvaLocali(locali* locali, struct index* indexs) {

    if (locali->array == NULL) {
        setColor(GIALLO);
		printf("[!] Il vettore dei Locali non esiste in memoria!\n");
		setColor(DEFAULT);
        FILE* fp = fopen("./files/locali.csv", "w");
        fclose(fp);
        return -1;
    }

    FILE* fp;
    
    if ((fp = fopen("./files/locali.csv", "w")) != NULL) {

        for (int i=0; i<locali->size;i++) {
            fprintf(fp, "%d;", locali->array[i].codice);
            fprintf(fp, "%s;", locali->array[i].nome);
            fprintf(fp, "%d;", locali->array[i].file);
            fprintf(fp, "%d\n", locali->array[i].posti);
        }
        fclose(fp);
    } else {
        setColor(ROSSO);
		printf("[!] Impossibile salvare il file locali.csv\n");
		setColor(DEFAULT);
        return -1;
    }

    free(locali->array);
    caricaLocali(locali, indexs);
    return 0;
}

/** La funzione visualizza a schermo il modulo di inserimento dati e permette la registrazione di un nuovo locale sul file CSV apposito.
 * 
 * @param [in] locali : struct contentente, tra gli altri campi, l'indirizzo all'array di struct utilizzato per effettuare operazioni concernenti ai locali.
 *
 * @return 1: valore che indica il successo dell'operazione di registrazione.
 * @return -1: se l'array di struct concernente ai locali è nullo.
 * 
 */

int aggiungiLocale(locali* locali) {

	int error, error2;
    locale temp;

	if (locali->array == NULL)
		return -1;

    system("cls");
    setColor(ROSSO);
    printf("\
			\t\n***********************************************\
			\t\n*               REGISTRA LOCALE               *\
			\t\n***********************************************\n");
    setColor(DEFAULT);
    error = 0;
    error2 = 0;
	do {
        inizializzaStringa(temp.nome, 20);
		if (error) {
			fflush(stdin);
			setColor(ROSSO);
			printf("[!] Puoi inserire solo lettere numeri e &!\n");
			setColor(DEFAULT);
			error = 0;
		}

		if (error2) {
			fflush(stdin);
			setColor(ROSSO);
			printf("[!] Il nome deve avere minimo 4 caratteri e massimo 19.\n");
			setColor(DEFAULT);
			error2 = 0;
		}

		printf("Inserisci nome: ");			
		scanf("%[^\n]", &temp.nome);

		if (stringaValida(temp.nome, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789& ") == 1)
			error = 1;

		if (strlen(temp.nome)>19 || strlen(temp.nome)<4)
			error2 = 1;

	} while(error != 0 || error2 != 0);

    fflush(stdin);

	do {

		printf("Inserisci numero file: ");			
		error=scanf("%d", &temp.file);

        if (error != 1 || temp.file < 1){
            setColor(ROSSO);
            printf("[!] Dato non valido.\n");
            setColor(DEFAULT);
            fflush(stdin);
        }

	} while(temp.file < 1 || error != 1);

    fflush(stdin);

	do {
		printf("Inserisci numero posti: ");			
		error=scanf("%d", &temp.posti);

        if (error != 1 || temp.posti < 1){
            setColor(ROSSO);
            printf("[!] Dato non valido.\n");
            setColor(DEFAULT);
            fflush(stdin);
        }

	} while(temp.posti < 1|| error != 1);

    temp.codice = (*(locali->index))++;

    locali->array = (locale*) realloc(locali->array, sizeof(locale) * ++locali->size);
    memcpy(&locali->array[locali->size - 1], &temp, sizeof(temp));

    return 1;
}

/** La funzione stampa a video i dati di un locale indicato in input.
 * 
 * @param [in] locali : struct contentente, tra gli altri campi, l'indirizzo all'array di struct utilizzato per effettuare operazioni concernenti ai locali.
 * @param [in] entity : puntatore ad un locale oppure nullo, se nullo verrà chiesto di cercare un locale, altrimenti si opera su quel puntatore
 * 
 */

void visualizzaLocale(locali* locali, locale* entity, int diagnostica) {

    fflush(stdin);
    if (locali->array == NULL) {
		setColor(GIALLO);
		printf("[!] Il vettore dei Locali è nullo!\n");
		setColor(DEFAULT);
		return;
	}

    if (!diagnostica) {
        system("cls");
        setColor(ROSSO);
        printf("\
			\t\n***********************************************\
			\t\n*               VISUALIZZA LOCALE             *\
			\t\n***********************************************\n");
        setColor(DEFAULT);
    }

    char nome[20] = {'\0'};
    locale* locale;

    if (entity == NULL) {

        int error = 0;
        int error2 = 0;

        do {
            inizializzaStringa(nome, 20);
            if (error) {
                fflush(stdin);
                setColor(ROSSO);
                printf("[!] Puoi inserire solo lettere numeri e &!\n");
                setColor(DEFAULT);
                error = 0;
            }

            if (error2) {
                fflush(stdin);
                setColor(ROSSO);
                printf("[!] Il nome deve avere minimo 4 caratteri e massimo 19.\n");
                setColor(DEFAULT);
                error2 = 0;
            }

            printf("Inserisci nome: ");			
            scanf("%[^\n]", nome);

            if (stringaValida(nome, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789& ") == 1)
                error = 1;
            if (strlen(nome)>19 || strlen(nome)<4)
                error2 = 1;

	    } while(error != 0 || error2 != 0);

        locale = ricercaLocale(locali, nome);

    } else {
        locale = entity;
    }

    if (locale != NULL) {
        setColor(GIALLO);
        printf("%-20s : %-30d\n", "Codice", locale->codice);
        printf("%-20s : %-30s\n", "Nome", locale->nome);
        printf("%-20s : %-30d\n", "Numero File", locale->file);
        printf("%-20s : %-30d\n\n", "Numero Posti", locale->posti);
        setColor(DEFAULT);
    } else {
		setColor(GIALLO);
		printf("Il locale '%s' non esiste!\n\n", nome);
		setColor(DEFAULT);
	}
}

/**
 * La funzione visualizza tutti i locali.
 * 
 * @param [in] locali : struct contentente, tra gli altri campi, l'indirizzo all'array di struct utilizzato per effettuare operazioni concernenti ai locali.
 * @param [in] diagnostica : sentinella per segnalare una pulizia dello schermo e visualizzare un'intestazione.
 * 
 */
void visualizzaLocali(locali* locali, int diagnostica) {

    if (locali->array == NULL) {
		setColor(GIALLO);
		printf("[!] Il vettore dei Locali è nullo!\n");
		setColor(DEFAULT);
		return;
	}

    for (int i=0; i<locali->size; i++)
        visualizzaLocale(locali, &locali->array[i], diagnostica);

}

/** Il programma visualizza con il medesimo modulo di registrazione, l'interfaccia locali, mostrando inoltre l'attuale valore che possiede ogni campo. 
 * E' possibile modificarne il contenuto (tranne del codice) oppure inviare il carattere di End-Of-File se si vuol lasciare inalterato il campo.
 * 
 * @param [in] locali : struct contentente, tra gli altri campi, l'indirizzo all'array di struct utilizzato per effettuare operazioni concernenti ai locali.
 * 
 * @return 0: il locale cercato non è stato trovato
 * @return 1: valore che indica il successo dell'operazione di modifica
 * @return -1: se l'array di struct concernente ai locali è nullo.
 */
int modificaLocale(locali* locali) {
    
    if (locali->array == NULL)
		return -1;

	fflush(stdin);
	system("cls");
    setColor(ROSSO);
    printf("\
			\t\n***********************************************\
			\t\n*               MODIFICA LOCALE               *\
			\t\n***********************************************\n");
    setColor(DEFAULT);

	char nome[20] = {'\0'};

	int error = 0;
    int error2 = 0;

     do {
        inizializzaStringa(nome, 20);
        if (error) {
            fflush(stdin);
            setColor(ROSSO);
            printf("[!] Puoi inserire solo lettere numeri e &!\n");
            setColor(DEFAULT);
            error = 0;
        }

        if (error2) {
            fflush(stdin);
            setColor(ROSSO);
            printf("[!] Il nome deve avere minimo 4 caratteri e massimo 19.\n");
            setColor(DEFAULT);
            error2 = 0;
        }

        printf("Inserisci nome: ");			
        scanf("%[^\n]", nome);

        if (stringaValida(nome, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789& ") == 1)
            error = 1;
        if (strlen(nome)>19 || strlen(nome)<4)
            error2 = 1;

	} while(error != 0 || error2 != 0);


    locale* found = ricercaLocale(locali, nome);

	if (found != NULL) {
        locale temp;
        fflush(stdin);

        temp.codice = found->codice;

        setColor(ROSSO);
		puts("\n[!] Premere CTRL + Z se si vuol lasciare inalterato il valore!\n");

        setColor(GIALLO);

        int error = 0;
        int error2 = 0;
        do {
            inizializzaStringa(temp.nome, 20);
            if (error) {
                fflush(stdin);
                setColor(ROSSO);
                printf("[!] Puoi inserire solo lettere numeri e &!\n");
                setColor(DEFAULT);
                error = 0;
            }

            if (error2) {
                fflush(stdin);
                setColor(ROSSO);
                printf("[!] Il nome deve avere minimo 4 caratteri e massimo 19.\n");
                setColor(DEFAULT);
                error2 = 0;
            }

            printf("[Attuale: %s] Modifica nome: ", found->nome);			
            scanf("%[^\n]", &temp.nome);

            if (feof(stdin)) {
				strcpy(temp.nome, found->nome);
				fflush(stdin);
			}

            if (stringaValida(temp.nome, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789& ") == 1)
                error = 1;
            if (strlen(temp.nome)>19 || strlen(temp.nome)<4)
                error2 = 1;

        } while(error != 0 || error2 != 0);
        fflush(stdin);

        do {
            printf("[Attuale: %d] Modifica numero file: ", found->file);			
            error=scanf("%d", &temp.file);
            if (feof(stdin)) {
				temp.file = found->file;
				fflush(stdin);
                break;
			}
            if (error != 1 || temp.file < 1){
                setColor(ROSSO);
                printf("[!] Puoi inserire solo numeri validi.\n");
                setColor(DEFAULT);
                fflush(stdin);
            }

        } while(temp.file < 1 || error != 1);
        error = 0;
        fflush(stdin);

        do {
            printf("[Attuale: %d] Modifica numero poltrone: ", found->posti);			
            error=scanf("%d", &temp.posti);
            if (feof(stdin)) {
				temp.posti = found->posti;
				fflush(stdin);
                break;
			}
            if (error != 1 || temp.posti < 1){
                setColor(ROSSO);
                printf("[!] Puoi inserire solo numeri validi.\n");
                setColor(DEFAULT);
                fflush(stdin);
            }

        } while(temp.posti <= 0 || error != 1);
        fflush(stdin);

        memcpy(found, &temp, sizeof(temp));

    } else	
		return 0;

    return 1;
}

/** Il programma esegue una ricerca per nome di locale, e chiede una conferma per l'eliminazione. Successivamente il locale trovato viene spostato in ultima posizione
 * e viene fatto un realloc del vettore dinamico che così esclude il locale da eliminare
 * 
 * @param [in] locali : struct contentente, tra gli altri campi, l'indirizzo all'array di struct utilizzato per effettuare operazioni concernenti ai locali.
 *
 * @return 0: il locale cercato non è stato trovato
 * @return code: codice del locale eliminato
 * @return -2: si è deciso di non eliminare il locale
 * @return -1: se l'array di struct concernente ai locali è nullo.
 */
int eliminaLocale(locali* locali) {

    if (locali->array == NULL)
		return -1;

    fflush(stdin);
	system("cls");
    setColor(ROSSO);
    printf("\
			\t\n***********************************************\
			\t\n*               ELIMINA  LOCALE               *\
			\t\n***********************************************\n");
    setColor(DEFAULT);

	char nome[20] = {'\0'};
    int error = 0, error2 = 0;
	     do {
        inizializzaStringa(nome, 20);
        if (error) {
            fflush(stdin);
            setColor(ROSSO);
            printf("[!] Puoi inserire solo lettere numeri e &!\n");
            setColor(DEFAULT);
            error = 0;
        }

        if (error2) {
            fflush(stdin);
            setColor(ROSSO);
            printf("[!] Il nome deve avere minimo 4 caratteri e massimo 19.\n");
            setColor(DEFAULT);
            error2 = 0;
        }

        printf("Inserisci nome: ");			
        scanf("%[^\n]", nome);

        if (stringaValida(nome, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789& ") == 1)
            error = 1;
        if (strlen(nome)>19 || strlen(nome)<4)
            error2 = 1;

	} while(error != 0 || error2 != 0);

    locale* found = ricercaLocale(locali, nome);
    int code = found->codice;

    if (found != NULL) {

        visualizzaLocale(locali, found, 0);

        int error;
        int scelta;
        do {
            setColor(ROSSO);	
		    printf("\n\nVuoi veramente eliminare questo locale? (0 : no | 1 : si): ");		
		    error = scanf("%d", &scelta);
		    if (error != 1 || scelta < 0 || scelta > 1){
				fflush(stdin);
				printf("[!] Puoi inserire solo un numero tra quelli indicati.\n");
				error=0;
		    }
	    } while(error != 1 || scelta < 0 || scelta > 1);

        if (scelta) {

            locale temp;

            /*
             * Viene effettuato lo swap tra il locale trovato e l'elemento in ultima posizione per poi eliminarlo;
             * successivamente viene effettuato un ordinamento per codice crescente
             */

            memcpy(&temp, found, sizeof(locale));
            memcpy(found, &locali->array[locali->size - 1], sizeof(locale));
            memcpy(&locali->array[locali->size - 1], &temp, sizeof(locale));
            locali->array = (locale*) realloc(locali->array, sizeof(locale) * --locali->size);
        } else
            return -2;

    } else
        return 0;
    

    return code;
}

/**
 * La funzione ricerca nel vettore dinamico la struct di riferimento al nome dato in input, se ne trova una corrispondenza ne ritorna il puntatore. Se ne trova
 * più di uno fornisce un elenco in cui è possibile derivarne uno.
 * 
 * @param [in] locali : struct contentente, tra gli altri campi, l'indirizzo all'array di struct utilizzato per effettuare operazioni concernenti ai locali.
 * @param [in] nickname : array di caratteri contenente il nickname.
 * 
 * @return NULL : Il vettore è nullo oppure non è stata trovata alcuna corrispondenza.
 * @return *locali : Viene trovato il locale e restituito il puntatore nell'array dinamico.
 * 
 */
locale* ricercaLocale(locali* locali, char* nome) {

	if (locali->array == NULL) {
		setColor(GIALLO);
		printf("[!] Il vettore dei Locali è nullo!\n");
		setColor(DEFAULT);
		return NULL;
	}

    int* buffer = calloc(locali->size, sizeof(int));
    unsigned int occorrenze = 0;

    for (int i=0; i<locali->size; i++) {
        if (strcmp(myLower(locali->array[i].nome), myLower(nome)) == 0) {
            buffer[occorrenze++] = i+1;
        }
    }

    if (occorrenze > 1) {

        unsigned int scelta; 
        int error, error2;

        printf("\nCon il nome '%s', sono stati trovati i seguenti locali:\n", nome);

        for (int j=0; j<occorrenze; j++){           
            printf("%d : Nome: '%s' | File: '%d' | Posti: '%d'\n", buffer[j], locali->array[buffer[j] - 1].nome, locali->array[buffer[j] - 1].file, locali->array[buffer[j] - 1].posti);    
        }

        do {
            fflush(stdin);
            do {
                printf("\nIndica il locale a cui ti riferisci:");
                error=scanf("%u",&scelta);
                if(error != 1){
                    fflush(stdin);
                    setColor(ROSSO);
                    printf("[!] Puoi inserire solo numeri.\n");
                    setColor(DEFAULT);
                }        
            } while(error != 1);
     
            for(int j=0; j < occorrenze; j++) {
                if (buffer[j] == scelta) {
                    error2 = 0;
                    break;
                 } else {
                    error2 = 1;    
                 }                    
            } 

            if(error2 != 0){
                setColor(ROSSO);
                printf("[!] Devi indicare una scelta tra quelle proposte.\n");
                setColor(DEFAULT);
            }

        }while(error2 != 0);

        free(buffer);
        return &locali->array[scelta-1];

    } else if (occorrenze == 1) {
        return &locali->array[buffer[0] - 1];
    }

	return NULL;
}


/**
 * La funzione ricerca nel vettore dinamico la struct di riferimento al codice dato in input, se ne trova una corrispondenza ne ritorna il puntatore. Questa funzione
 * ha scopo a livello applicativo
 * 
 * @param [in] locali : struct contentente, tra gli altri campi, l'indirizzo all'array di struct utilizzato per effettuare operazioni concernenti ai locali.
 * @param [in] codice : codice univoco di index
 * 
 * @return NULL : Il vettore è nullo oppure non è stata trovata alcuna corrispondenza.
 * @return *locali : Viene trovato il locale e restituito il puntatore nell'array dinamico.
 * 
 */
locale* ricercaLocaleByCodice(locali* locali, int codice) {

	if (locali->array == NULL) {
		setColor(GIALLO);
		printf("[!] Il vettore dei Locali è nullo!\n");
		setColor(DEFAULT);
		return NULL;
	}

    for (int i=0; i<locali->size; i++) {
        if (locali->array[i].codice == codice) {
            return &locali->array[i];
        }
    }

	return NULL;
}


/**
 * La funzione lavora insieme alla funzione mergeSort, si occupa dell'ordinamento vero e proprio dell'array, confrontando gli elementi delle varie partizioni create.
 * 
 * @param [in] locali: struct contentente, tra gli altri campi, l'indirizzo all'array di struct utilizzato per effettuare operazioni concernenti ai locali.
 * @param [in] inizio : intero che indica l'indice dell'inizio dell'array.
 * @param [in] meta : intero che indica l'indice della metà dell'array.
 * @param [in] fine : intero che indica l'indice della fine dell'array.
 * 
 */
void mergeLocali(locali* locali, int inizio, int meta, int fine) {
	
	int i = inizio;
	int j = meta + 1;
	int k = 0;
	locale* temp = calloc(locali->size, sizeof(locale));

	while (i<=meta && j<=fine){
		if(locali->array[i].codice < locali->array[j].codice) {
			memcpy(&temp[k], &locali->array[i], sizeof(locale));
			i++;
		} else {
			memcpy(&temp[k], &locali->array[j], sizeof(locale));
			j++;
		}
		k++;
	}
	
	while (i<=meta){
		memcpy(&temp[k], &locali->array[i], sizeof(locale));
		i++;
		k++;
	}
	
	while (j<=fine){
		memcpy(&temp[k], &locali->array[j], sizeof(locale));
		j++;
		k++;
	}
	
	for(k=inizio; k<=fine; k++) {
		memcpy(&locali->array[k], &temp[k-inizio], sizeof(locale));
	}

	free(temp);
}