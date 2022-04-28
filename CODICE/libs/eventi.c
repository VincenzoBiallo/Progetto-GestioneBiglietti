#include "eventi.h"

/**
 * La funzione apre il file di riferimento e il file degli index, legge riga per riga il file del CSV, le analizza e le inserisce in elementi di un vettore dinamico
 * che viene generato in base al numero di righe che vengono lette dal file. In seguito viene creato un puntatore all'index di AUTO_INCREMENT
 * 
 * @param [in] eventi : struct contentente, tra gli altri campi, l'indirizzo all'array di struct utilizzato per effettuare operazioni concernenti agli eventi.
 * @param [in] indexs : puntatore a struct index
 * @param [in] locali : struct contentente, tra gli altri campi, l'indirizzo all'array di struct utilizzato per effettuare operazioni concernenti ai locali.
 * 
 * @return 0 : il vettore è stato caricato con successo
 * @return -1 : impossibile aprire il file
 * 
 */

int caricaEventi(eventi* eventi, struct index* indexs, locali* locali) {

    FILE* fp;
    
    if ((fp = fopen("./files/eventi.csv", "r")) != NULL) {

        char buffer[1000];
        int lines = contaLinee(fp);
        if (lines == -1) 
            return -1;    

        eventi->size = lines;
        eventi->array = calloc(eventi->size, sizeof(evento));

        int count = 0;

        // Viene letta ogni riga del file CSV e viene caricata in un buffer temporaneo di 1000 caratteri
		while (fgets(buffer, sizeof(buffer), fp)) {

            char* token;
            char* line = buffer;
            char** input = &line;

            token = myParser(input, ";");
			eventi->array[count].codice = atoi(token);

            token = myParser(input, ";");
			strcpy(eventi->array[count].nome, token);

            token = myParser(input, ";");
            if (token != NULL) {
                int day = 0;
                int month = 0;
                int year = 0;
                sscanf(token, "%d/%d/%d", &day, &month, &year);
                eventi->array[count].data.giorno = day;
                eventi->array[count].data.mese = month;
                eventi->array[count].data.anno = year;                      
            }

            token = myParser(input, ";");
            if (token != NULL) {
                int hour = 0;
                int minute = 0;
                sscanf(token, "%d:%d", &hour, &minute);
                eventi->array[count].oraInizio.ora = hour;
                eventi->array[count].oraInizio.minuto = minute;                    
            }
			
            token = myParser(input, ";");
            if (token != NULL) {
                int hour = 0;
                int minute = 0;
                sscanf(token, "%d:%d", &hour, &minute);
                eventi->array[count].oraFine.ora = hour;
                eventi->array[count].oraFine.minuto = minute;                    
            }

            token = myParser(input, ";");
			strcpy(eventi->array[count].ospite, token);

            token = myParser(input, ";");
            if (ricercaLocaleByCodice(locali, atoi(token)) != NULL)
                eventi->array[count].locale = atoi(token);
            else
                eventi->array[count].locale = -1;

            token = myParser(input, ";");
			eventi->array[count].costo_prima_fila = atof(token);

            token = myParser(input, ";");
			eventi->array[count].costo_ultima_fila = atof(token);

            count++;
        }

        eventi->index = &indexs->evento;

        fclose(fp); 
    } else {
        setColor(ROSSO);
		printf("[!] Impossibile aprire il file eventi.csv\n");
		setColor(DEFAULT);
        FILE* fp = fopen("./files/eventi.csv", "w");
        fclose(fp);
        return -1;
    }

    return 1;
}

/**
 * La funzione cancella e ricrea il file di riferimento inserendo una riga per ogni struct nel vettore dinamico. In seguito viene distrutto il vettore e viene richiamata
 * la funzione caricaEventi
 * 
 * @param [in] eventi : struct contentente, tra gli altri campi, l'indirizzo all'array di struct utilizzato per effettuare operazioni concernenti agli eventi.
 * @param [in] indexs : puntatore a struct index
 * @param [in] locali : struct contentente, tra gli altri campi, l'indirizzo all'array di struct utilizzato per effettuare operazioni concernenti ai locali.
 * 
 * @return 0 : il vettore è stato salvato su file
 * @return -1 : il vettore dinamico non esiste oppure non è stato possibile salvare il file
 * 
 */

int salvaEventi(eventi* eventi, struct index* indexs, locali* locali) {

    if (eventi->array == NULL) {
        setColor(GIALLO);
		printf("[!] Il vettore degli Eventi non esiste in memoria!\n");
		setColor(DEFAULT);
        FILE* fp = fopen("./files/eventi.csv", "w");
        fclose(fp);
        return -1;
    }

    FILE* fp;
    
    if ((fp = fopen("./files/eventi.csv", "w")) != NULL) {

        for (int i=0; i<eventi->size;i++) {
            fprintf(fp, "%d;", eventi->array[i].codice);
            fprintf(fp, "%s;", eventi->array[i].nome);
            fprintf(fp, "%d/%d/%d;", eventi->array[i].data.giorno, eventi->array[i].data.mese, eventi->array[i].data.anno);
            fprintf(fp, "%d:%d;", eventi->array[i].oraInizio.ora, eventi->array[i].oraInizio.minuto);
            fprintf(fp, "%d:%d;", eventi->array[i].oraFine.ora, eventi->array[i].oraFine.minuto);
            fprintf(fp, "%s;", eventi->array[i].ospite);
            if (ricercaLocaleByCodice(locali, eventi->array[i].locale) != NULL)
                fprintf(fp, "%d;", eventi->array[i].locale);
            else
                fprintf(fp, "%d;", -1);
            fprintf(fp, "%f;", eventi->array[i].costo_prima_fila);
            fprintf(fp, "%f\n", eventi->array[i].costo_ultima_fila);
        }
        fclose(fp);
    } else {
        setColor(ROSSO);
		printf("[!] Impossibile salvare il file eventi.csv\n");
		setColor(DEFAULT);
        return -1;
    }

    free(eventi->array);
    caricaEventi(eventi, indexs, locali);

    return 1;
}

/** La funzione visualizza a schermo il modulo di inserimento dati e permette la registrazione di un nuovo evento sul file CSV apposito.
 * 
 * @param [in] eventi : struct contentente, tra gli altri campi, l'indirizzo all'array di struct utilizzato per effettuare operazioni concernenti agli eventi.
 * @param [in] locali : struct contentente, tra gli altri campi, l'indirizzo all'array di struct utilizzato per effettuare operazioni concernenti ai locali.
 *
 * @return 1: valore che indica il successo dell'operazione di registrazione.
 * 
 */

int aggiungiEvento(eventi* eventi, locali* locali){

    evento temp;
    int ritorno;
    int error = 0, error2 = 0;
    float differenza;

    system("cls");
    setColor(VIOLA);
    printf("\
            \t\n***********************************************\
            \t\n*               REGISTRA EVENTO               *\
            \t\n***********************************************\n");
    setColor(DEFAULT);
    
    /**************
	* Nome evento
	**************/
    do {
        inizializzaStringa(temp.nome, 50);
		if (error) {
			fflush(stdin);
			setColor(ROSSO);
			printf("[!] Puoi inserire solo lettere, numeri, spazi e &!\n");
			setColor(DEFAULT);
			error = 0;
		}

		if (error2) {
			fflush(stdin);
			setColor(ROSSO);
			printf("[!] Il nome deve avere minimo 4 caratteri e massimo 49.\n");
			setColor(DEFAULT);
			error2 = 0;
		}

		printf("Inserisci nome: ");			
		scanf("%[^\n]", &temp.nome);

		if (stringaValida(temp.nome, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 &") == 1)
			error = 1;

		if (strlen(temp.nome)>49 || strlen(temp.nome)<4)
			error2 = 1;

	} while(error != 0 || error2 != 0);

    fflush(stdin);

	/**************
	* Data evento
	**************/
    printf("Inserisci la data dell'evento:\n");
    do {
		fflush(stdin);
		ritorno = inserisciData(&temp.data);
	} while(ritorno == -1);

    setColor(GIALLO);
    printf("\nRegistrazione orari dell'evento\
            \n[!] Un evento deve durare minimo 1 ora e puo' durare massimo 4 ore.\n\n");
    setColor(DEFAULT);

  	/**************
	* Orario inizio
	**************/
    do {

        if(error){
            setColor(ROSSO);
            printf("[!] Hai inserito una durata dell'evento non valida.\n");
            setColor(DEFAULT);
            error=0;
        }      

        printf("Indica l'orario di inizio dell'evento:\n");
        
        do {
            printf("\tOra: ");
            error=scanf("%d", &temp.oraInizio.ora);
            if (error != 1 || temp.oraInizio.ora < 0 || temp.oraInizio.ora > 23) {
                fflush(stdin);
                setColor(ROSSO);
                printf("[!] L'ora indicata non e' valida.\n");
                setColor(DEFAULT);
            }
        } while(error != 1 || temp.oraInizio.ora < 0 || temp.oraInizio.ora > 23);
        error=0;
        fflush(stdin);

        do {
            printf("\tMinuti: ");
            error=scanf("%d", &temp.oraInizio.minuto);
            if (error != 1 || temp.oraInizio.minuto < 0 || temp.oraInizio.minuto > 59) {
                fflush(stdin);
                setColor(ROSSO);
                printf("[!] I minuti indicati non sono validi.\n");
                setColor(DEFAULT);
            }
        } while(error != 1 || temp.oraInizio.minuto < 0 || temp.oraInizio.minuto > 59);
        error=0;
        fflush(stdin);
	
        /**************
        * Orario fine
        **************/ 
	    printf("Indica l'orario di fine dell'evento:\n");

        do {
            printf("\tOra: ");
            error=scanf("%d", &temp.oraFine.ora);
            if (error != 1 || temp.oraFine.ora < 0 || temp.oraFine.ora > 23) {
                fflush(stdin);
                setColor(ROSSO);
                printf("[!] L'ora indicata non e' valida.\n");
                setColor(DEFAULT);
            }
        } while(error != 1 || temp.oraFine.ora < 0 || temp.oraFine.ora > 23);
        error=0;
        fflush(stdin);

        do {
            printf("\tMinuti: ");
            error=scanf("%d", &temp.oraFine.minuto);
            if (error != 1 || temp.oraFine.minuto < 0 || temp.oraFine.minuto > 59) {
                fflush(stdin);
                setColor(ROSSO);
                printf("[!] I minuti indicati non sono validi.\n");
                setColor(DEFAULT);
            }
        } while(error != 1 || temp.oraFine.minuto < 0 || temp.oraFine.minuto > 59);
        error=0;

        differenza = differenzaOrari(temp.oraInizio, temp.oraFine);
        if (differenza < 1 || differenza > 4) {
            error=1;
        }

    } while(error != 0);

    fflush(stdin);
	
	
     /**************
     * Ospite
     **************/ 
	do {
        inizializzaStringa(temp.ospite, 50);

        if (error) {
			fflush(stdin);
			setColor(ROSSO);
			printf("[!] Puoi inserire solo lettere, numeri, spazi e '&'!\n");
			setColor(DEFAULT);
			error = 0;
		}

		if (error2) {
			fflush(stdin);
			setColor(ROSSO);
			printf("[!] Il nome dell'ospite deve avere minimo 2 caratteri e massimo 49.\n");
			setColor(DEFAULT);
			error2 = 0;
		}

        printf("Inserisci il nome dell'ospite: ");
        scanf("%[^\n]", &temp.ospite);

        if (stringaValida(temp.ospite, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz &1234567890") == 1)
			error = 1;
		if (strlen(temp.ospite)>49 || strlen(temp.ospite)<2)
			error2 = 1;

    } while(error != 0 || error2 != 0);
    fflush(stdin);

	/**************
    * Locale
    **************/

    char nome[20] = {'\0'};
    locale* locale;

    do {

        if (error) {
            fflush(stdin);
            setColor(ROSSO);
            printf("[!] Non e' stato trovato alcun locale col nome '%s' !\n", nome);
            setColor(DEFAULT);
            error = 0;
        }

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
            printf("\n\nNel sistema sono presenti i seguenti locali: \n");
            for(int i=0; i < locali->size; i++){
                printf("%-12s %20s | %-15s %2d | %-15s %2d\n", 
                "Nome locale:", locali->array[i].nome, 
                "Numero file:", locali->array[i].file, 
                "Numero posti:", locali->array[i].posti);
            }
            printf("\nInserisci nome locale: ");			
            scanf("%[^\n]", nome);

            if (stringaValida(nome, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789& ") == 1)
                error = 1;
            if (strlen(nome)>19 || strlen(nome)<4)
                error2 = 1;

	    } while(error != 0 || error2 != 0);

        locale = ricercaLocale(locali, nome);

        if (locale == NULL)
            error = 1;

    } while(error != 0);

    temp.locale = locale->codice;
	
    /********************
     * Costo prima fila
     ********************/ 
	do { 
        printf("\n\nInserisci il costo della prima fila: ");
        error=scanf("%f", &temp.costo_prima_fila);
        if (error!= 1 || temp.costo_prima_fila < 1) {
            fflush(stdin);
			setColor(ROSSO);
			printf("[!] Il costo indicato non e' valido.\n");
			setColor(DEFAULT);
        }
    } while(error!= 1 || temp.costo_prima_fila < 1);
    error = 0;
    fflush(stdin);
	
     /********************
     * Costo ultima fila
     ********************/ 	
	do{ 
        printf("\n\nInserisci il costo dell'ultima fila: ");
        error=scanf("%f", &temp.costo_ultima_fila);
        if (error!= 1 || temp.costo_ultima_fila < 1) {
            fflush(stdin);
			setColor(ROSSO);
			printf("[!] Il costo indicato non e' valido.\n");
			setColor(DEFAULT);
        }

        if(temp.costo_ultima_fila>temp.costo_prima_fila){
            fflush(stdin);
            setColor(ROSSO);
            printf("[!] Il costo dell'ultima fila non puo' essere maggiore di quello della prima fila.\n");
            setColor(DEFAULT);
        }
    } while(error!= 1 || temp.costo_ultima_fila < 1 || temp.costo_ultima_fila>temp.costo_prima_fila);
    error = 0;
    fflush(stdin);

    temp.codice = (*(eventi->index))++;

    eventi->array = (evento*) realloc(eventi->array, sizeof(evento) * ++eventi->size);
    memcpy(&eventi->array[eventi->size - 1], &temp, sizeof(temp));

    return 1;
}


/**
 * La funzione ricerca un evento per data, inserita in input.
 * 
 * @param [in] eventi : struct contentente (tra i vari campi) l'indirizzo all'array di struct utilizzato per effettuare operazioni concernenti gli eventi. 
 * @param [in] data : struct tipo data, contiene la data utilizzata come parametro di ricerca
 * @param [in] eventi : struct contentente (tra i vari campi) l'indirizzo all'array di struct utilizzato per effettuare operazioni concernenti ai locali. 
 * 
 * @return NULL : se il vettore degli eventi è nullo oppure non è stata trovata alcuna corrispondenza.
 * @return evento* : se sono state trovate una o più corrispondenze, se ne sono state trovate più di 1, l'utente ne seleziona 1 e ritornerà l'indirizzo della struct selezionata.
 * 
 */
evento* ricercaEventoByData(eventi* eventi , data data, locali* locali) {

    if (eventi->array == NULL) {
        setColor(GIALLO);
		printf("[!] Il vettore degli Eventi è nullo!\n");
		setColor(DEFAULT);
        return NULL;
    }

    int* buffer = calloc(eventi->size, sizeof(int));
    unsigned int occorrenze = 0;

    for (int i=0; i<eventi->size; i++) {
        if( (eventi->array[i].data.giorno == data.giorno) && 
            (eventi->array[i].data.mese == data.mese) &&
            (eventi->array[i].data.anno == data.anno) ) {
                buffer[occorrenze++] = i+1;
            }
    }

    if(occorrenze > 1) {

        unsigned int scelta;
        int error, error2;

        printf("\nNella data '%d/%d/%d', sono stati trovati i seguenti eventi:\n", data.giorno, data.mese, data.anno);

        for (int j=0; j<occorrenze; j++) {
            char* nomeLoc = NULL;
            locale* found = ricercaLocaleByCodice(locali, eventi->array[buffer[j] - 1].locale);
            if (found != NULL)
                nomeLoc = found->nome;
            else
                nomeLoc = "null";
            printf("%d : Ospite: '%s' | Locale: '%s' | Ora inizio: '%02d:%02d' | Ora fine: '%02d:%02d'\n", buffer[j], eventi->array[buffer[j] - 1].ospite, nomeLoc, eventi->array[buffer[j] - 1].oraInizio.ora, eventi->array[buffer[j] - 1].oraInizio.minuto, eventi->array[buffer[j] - 1].oraFine.ora, eventi->array[buffer[j] - 1].oraFine.minuto);     
        }

        do {
            fflush(stdin);
            do {
                printf("\nIndica l'evento a cui ti riferisci:");
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
        return &eventi->array[scelta-1];
        
    } else if (occorrenze == 1) {
        return &eventi->array[buffer[0] - 1];
    }

	return NULL;
}

/**
 * La funzione ricerca un evento per nome dello stesso o per l'ospite.
 * 
 * @param [in] eventi : struct contentente (tra i vai campi) l'indirizzo all'array di struct utilizzato per effettuare operazioni concernenti gli eventi. 
 * @param [in] string : array di caratteri contenente la stringa usata come parametro di ricerca.
 * @param [in] type : intero che indica il tipo di ricerca, se per nome evento o per ospite; il meccanismo si basa su uno switch.
 * @param [in] locali : struct contentente (tra i vai campi) l'indirizzo all'array di struct utilizzato per effettuare operazioni concernenti ai locali. 
 * 
 * @return NULL : se il vettore degli eventi è nullo oppure non è stata trovata alcuna corrispondenza.
 * @return evento* : se sono state trovate una o più corrispondenze, se ne sono state trovate più di 1, l'utente ne seleziona 1 e ritornerà l'indirizzo della struct selezionata.
 */
evento* ricercaEventoByString(eventi* eventi , char* string, int type, locali* locali) {

    if (eventi->array == NULL) {
        setColor(GIALLO);
		printf("[!] Il vettore degli Eventi è nullo!\n");
		setColor(DEFAULT);
        return NULL;
    }

    int* buffer = calloc(eventi->size, sizeof(int));
    unsigned int occorrenze = 0;

    switch(type) {

        case 1:
            for (int i=0; i<eventi->size; i++) {
                if (strcmp(myLower(eventi->array[i].ospite), myLower(string))==0)
                    buffer[occorrenze++] = i+1;
            }
            break;
        
        case 2:
            for (int i=0; i<eventi->size; i++) {
                if (strcmp(myLower(eventi->array[i].nome), myLower(string))==0)
                    buffer[occorrenze++] = i+1;
            }
            break;
        
        default:
            break;
    }

    if (occorrenze > 1) {

        unsigned int scelta;
        int error, error2;

        printf("\nCon: '%s', sono stati trovati i seguenti eventi:\n\n\n", string);

        for (int j=0; j<occorrenze; j++){           
            char* nomeLoc = NULL;
            locale* found = ricercaLocaleByCodice(locali, eventi->array[buffer[j] - 1].locale);
            if (found != NULL)
                nomeLoc = found->nome;
            else
                nomeLoc = "null";
             printf("%d : Data: '%d/%d/%d' | Locale: '%s' | Ora inizio: '%02d:%02d' | Ora fine: '%02d:%02d'\n", buffer[j], eventi->array[buffer[j] - 1].data.giorno, eventi->array[buffer[j] - 1].data.mese, eventi->array[buffer[j] - 1].data.anno, nomeLoc, eventi->array[buffer[j] - 1].oraInizio.ora, eventi->array[buffer[j] - 1].oraInizio.minuto, eventi->array[buffer[j] - 1].oraFine.ora, eventi->array[buffer[j] - 1].oraFine.minuto);     
        }

        do {
            fflush(stdin);
            do {
                printf("\n\nIndica l'evento a cui ti riferisci:");
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
        return &eventi->array[scelta-1];
        
    } else if (occorrenze == 1) {
        return &eventi->array[buffer[0] - 1];
    }

	return NULL;
}

/**
 *  La funzione ricerca un evento generico.
 * 
 * @param [in] eventi : struct contentente (tra i vai campi) l'indirizzo all'array di struct utilizzato per effettuare operazioni concernenti gli eventi. 
 * @param [in] locali : struct contentente (tra i vai campi) l'indirizzo all'array di struct utilizzato per effettuare operazioni concernenti ai locali. 
 * 
 * @return NULL : se il vettore degli eventi è nullo oppure non è stata trovata alcuna corrispondenza.
 * @return evento* : se sono state trovate una o più corrispondenze, se ne sono state trovate più di 1, l'utente ne seleziona 1 e ritornerà l'indirizzo della struct selezionata.
 */
evento* ricercaEvento(eventi* eventi, locali* locali) {

    int error = 0, error2 = 0;
    int scelta, ritorno;
    evento* found = NULL;
    data data;
    char stringa[50];

    printf("Indica un parametro di ricerca:\
          \n\t0. Data evento\
          \n\t1. Ospite\
          \n\t2. Nome evento\n\n");
    
    do{
        printf("Scegli un'opzione: ");
        error=scanf("%d", &scelta);
        if (error!=1 || scelta < 0 || scelta > 2) {
            fflush(stdin);
            setColor(ROSSO);
            printf("[!] L'opzione indicata non e' valida.\n");
            setColor(DEFAULT);
        }
    }while(error!=1 || scelta < 0 || scelta > 2);
    fflush(stdin);
    error=0;

    switch(scelta){
        
        case 0:
            printf("Inserisci la data dell'evento:\n");
            do {
                fflush(stdin);
                ritorno = inserisciData(&data);
	        } while(ritorno == -1);
            found = ricercaEventoByData(eventi, data, locali);
            break;
        
        case 1:
            do {
                inizializzaStringa(stringa, 50);

                if (error) {
                     fflush(stdin);
                    setColor(ROSSO);
                    printf("[!] Puoi inserire solo lettere, numeri, spazi e '&'!\n");
                    setColor(DEFAULT);
                    error = 0;
                }

                 if (error2) {
                     fflush(stdin);
                     setColor(ROSSO);
                     printf("[!] Il nome dell'ospite deve avere minimo 2 caratteri e massimo 49.\n");
                    setColor(DEFAULT);
                    error2 = 0;
                }

                 printf("Inserisci il nome dell'ospite: ");
                scanf("%[^\n]", &stringa);

                 if (stringaValida(stringa, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz &1234567890") == 1)
                    error = 1;
                 if (strlen(stringa)>49 || strlen(stringa)<2)
                    error2 = 1;

            } while(error != 0 || error2 != 0);
            fflush(stdin);
            found = ricercaEventoByString(eventi, stringa, scelta, locali);
            break;

        case 2:
            do {
                inizializzaStringa(stringa, 50);

                if (error) {
                     fflush(stdin);
                    setColor(ROSSO);
                    printf("[!] Puoi inserire solo lettere, numeri, spazi e '&'!\n");
                    setColor(DEFAULT);
                    error = 0;
                }

                 if (error2) {
                     fflush(stdin);
                     setColor(ROSSO);
                     printf("[!] Il nome dell'ospite deve avere minimo 2 caratteri e massimo 49.\n");
                    setColor(DEFAULT);
                    error2 = 0;
                }

                 printf("Inserisci il nome dell'evento: ");
                scanf("%[^\n]", &stringa);

                 if (stringaValida(stringa, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz &1234567890") == 1)
                    error = 1;
                 if (strlen(stringa)>49 || strlen(stringa)<2)
                    error2 = 1;

            } while(error != 0 || error2 != 0);
            fflush(stdin);
            found = ricercaEventoByString(eventi, stringa, scelta, locali);
            break;
    }
    return found;
        
}

/**
 * La funzione ricerca un evento per codice (Utilizzata solo a livello operativo, non utilizzabile direttamente dall'utente).
 * 
 * @param [in] eventi : struct contentente (tra i vai campi) l'indirizzo all'array di struct utilizzato per effettuare operazioni concernenti gli eventi. 
 * @param [in] codice : codice evento da ricercare
 * 
 * @return NULL : se il vettore degli eventi è nullo oppure non è stata trovata alcuna corrispondenza.
 * @return evento* : puntatore ad evento trovato.
 */
evento* ricercaEventoByCodice(eventi* eventi, int codice) {

	if (eventi->array == NULL) {
		setColor(GIALLO);
		printf("[!] Il vettore degli Eventi è nullo!\n");
		setColor(DEFAULT);
		return NULL;
	}

    for (int i=0; i<eventi->size; i++) {
        if (eventi->array[i].codice == codice) {
            return &eventi->array[i];
        }
    }

	return NULL;
}


/**
 * La funzione visualizza a schermo l'evento ricercato, chiamando le apposite funzioni di ricerca.
 * 
 *  @param [in] eventi : struct contentente (tra i vai campi) l'indirizzo all'array di struct utilizzato per effettuare operazioni concernenti gli eventi. 
 *  @param [in] entity : puntatore ad un singolo evento che può essere nullo, in modo da visualizzare direttamente quell'evento senza ricerca.
 *  @param [in] locali : struct contentente (tra i vai campi) l'indirizzo all'array di struct utilizzato per effettuare operazioni concernenti ai locali.
 *
 */
void visualizzaEvento(eventi* eventi, evento* entity, locali* locali) {

    evento* found;
    
    if (entity == NULL) {
        system("cls");
        setColor(VIOLA);
        printf("\
			\t\n***********************************************\
			\t\n*               VISUALIZZA EVENTO             *\
			\t\n***********************************************\n\n");
        setColor(DEFAULT);
        found = ricercaEvento(eventi, locali);
    } else
        found = entity;
    
	if(found != NULL) {
        char* nomeLoc = NULL;
        locale* foundLoc = ricercaLocaleByCodice(locali, found->locale);
        if (foundLoc != NULL)
            nomeLoc = foundLoc->nome;
        else
            nomeLoc = "null";

        setColor(GIALLO);
        printf("\n\
        \n%-20s : %d\
        \n%-20s : %-30s\
        \n%-20s : %02d/%02d/%d\
        \n%-20s : %d:%02d\
        \n%-20s : %d:%02d\
        \n%-20s : %-30s\
        \n%-20s : %-30s\
        \n%-20s : %.2f\
        \n%-20s : %.2f\n\n", 
        "Codice", found->codice,
        "Nome evento", found->nome,
        "Data", found->data.giorno, found->data.mese, found->data.anno,
        "Ora di inizio:", found->oraInizio.ora, found->oraInizio.minuto,
        "Ora di fine:", found->oraFine.ora, found->oraFine.minuto,
        "Ospite", found->ospite,
        "Locale", nomeLoc,
        "Costo prima fila", found->costo_prima_fila,
        "Costo ultima fila", found->costo_ultima_fila);
        setColor(DEFAULT);
                
    } else {
        setColor(GIALLO);
        printf("[!] Non e' stato trovato nessun evento corrispondente.\n");
        setColor(DEFAULT);
    }

}
    
/**
 * Il programma visualizza con il medesimo modulo di registrazione, l'interfaccia eventi, mostrando inoltre l'attuale valore che possiede ogni campo. 
 * è possibile modificarne il contenuto (tranne del codice) oppure inviare il carattere di End-Of-File se si vuol lasciare inalterato il campo.
 * 
 * @param [in] eventi : struct contentente (tra i vai campi) l'indirizzo all'array di struct utilizzato per effettuare operazioni concernenti gli eventi. 
 * @param [in] locali : struct contentente, tra gli altri campi, l'indirizzo all'array di struct utilizzato per effettuare operazioni concernenti ai locali.
 * 
 * @return 0 : l'evento cercato non è stato trovato
 * @return 1 : valore che indica il successo dell'operazione di modifica
 * @return -1 : se l'array di struct concernente agli eventi è nullo.
 */
int modificaEvento(eventi* eventi, locali* locali) {

    evento* found;

    if (eventi->array == NULL)
		return -1;

	system("cls");
    setColor(VIOLA);
    printf("\
			\t\n***********************************************\
			\t\n*               MODIFICA EVENTO               *\
			\t\n***********************************************\n");
    setColor(DEFAULT);

    found = ricercaEvento(eventi, locali);
    fflush(stdin);
    
    if (found != NULL) {
        
        evento temp;
        temp.codice = found->codice;
        int differenza;

        setColor(ROSSO);
		puts("\n[!] Premere CTRL + Z se si vuol lasciare inalterato il valore!\n");
        setColor(GIALLO);

        int error = 0;
        int error2 = 0;
        /**************
        * Nome evento
        **************/
        do {
            inizializzaStringa(temp.nome, 50);
            if (error) {
                fflush(stdin);
                setColor(ROSSO);
                printf("[!] Puoi inserire solo lettere, numeri, spazi e &!\n");
                setColor(GIALLO);
                error = 0;
            }

            if (error2) {
                fflush(stdin);
                setColor(ROSSO);
                printf("[!] Il nome deve avere minimo 4 caratteri e massimo 49.\n");
                setColor(GIALLO);
                error2 = 0;
            }

            printf("[Attuale: %s] Modifica nome evento: ", found->nome);			
            scanf("%[^\n]", &temp.nome);

            if (feof(stdin)) {
				strcpy(temp.nome, found->nome);
				fflush(stdin);
			}

            if (stringaValida(temp.nome, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 &") == 1)
                error = 1;
            if (strlen(temp.nome)>49 || strlen(temp.nome)<4)
                error2 = 1;

        } while(error != 0 || error2 != 0);
        fflush(stdin);
                
        /**************
        * Data evento
        **************/
        int scelta = -1;
		do {
            
            if(found->data.giorno==0)
				printf("[Attuale: Non registrata] Modificare la Data dell'Evento? (0 : no | 1 : si): ");
			else
				printf("[Attuale: %02d/%02d/%d] Modificare la Data dell'Evento? (0 : no | 1 : si): ", found->data.giorno, found->data.mese, found->data.anno);
			
            error=scanf("%d", &scelta);
			if (error != 1 || scelta < 0 || scelta > 1){
				fflush(stdin);
				setColor(ROSSO);
				printf("[!] Puoi inserire solo un numero tra quelli indicati.\n");
				setColor(GIALLO);
				error=0;
			}
		} while (scelta < 0 || scelta > 1 || error != 1);
		fflush(stdin);
		error=0;

		if (scelta) {

			int ritorno;
			do {
				fflush(stdin);
				ritorno = inserisciData(&temp.data);
			} while(ritorno == -1);
            setColor(GIALLO);
			
		} else {
			temp.data.giorno = found->data.giorno;
			temp.data.mese = found->data.mese;
			temp.data.anno = found->data.anno;
		}
		fflush(stdin);
        /**************
        * Orario evento
        **************/
		do {
            printf("[Orario inizio attuale: %02d:%02d]\n[Orario fine attuale: %02d:%02d]\nModificare l'orario dell'evento? (0 : no | 1 : si): ", found->oraInizio.ora, found->oraInizio.minuto, found->oraFine.ora, found->oraFine.minuto);
			error=scanf("%d", &scelta);
			if (error != 1 || scelta < 0 || scelta > 1){
				fflush(stdin);
				setColor(ROSSO);
				printf("[!] Puoi inserire solo un numero tra quelli indicati.\n");
				setColor(GIALLO);
				error=0;
			}
		} while (scelta < 0 || scelta > 1 || error != 1);
		fflush(stdin);
		error=0;

        if(scelta){
            do{    

                if(error){
                    setColor(ROSSO);
                    printf("[!] Hai inserito una durata dell'evento non valida.\n");
                    setColor(DEFAULT);
                    error=0;
                }      

                printf("Indica l'orario di inizio dell'evento:\n");
                
                do {
                    printf("\tOra: ");
                    error=scanf("%d", &temp.oraInizio.ora);
                    if (error != 1 || temp.oraInizio.ora < 0 || temp.oraInizio.ora > 23) {
                        fflush(stdin);
                        setColor(ROSSO);
                        printf("[!] L'ora indicata non e' valida.\n");
                        setColor(GIALLO);
                    }
                } while(error != 1 || temp.oraInizio.ora < 0 || temp.oraInizio.ora > 23);
                error=0;
                fflush(stdin);

                do {
                    printf("\tMinuti: ");
                    error=scanf("%d", &temp.oraInizio.minuto);
                    if (error != 1 || temp.oraInizio.minuto < 0 || temp.oraInizio.minuto > 59) {
                        fflush(stdin);
                        setColor(ROSSO);
                        printf("[!] I minuti indicati non sono validi.\n");
                        setColor(GIALLO);
                    }
                } while(error != 1 || temp.oraInizio.minuto < 0 || temp.oraInizio.minuto > 59);
                error=0;
                fflush(stdin);
                    
                printf("Indica l'orario di fine dell'evento:\n");
                    
                do {
                    printf("\tOra: ");
                    error=scanf("%d", &temp.oraFine.ora);
                    if (error != 1 || temp.oraFine.ora < 0 || temp.oraFine.ora > 23) {
                        fflush(stdin);
                        setColor(ROSSO);
                        printf("[!] L'ora indicata non e' valida.\n");
                        setColor(GIALLO);
                    }
                } while(error != 1 || temp.oraFine.ora < 0 || temp.oraFine.ora > 23);
                error=0;
                fflush(stdin);

                do {
                    printf("\tMinuti: ");
                    error=scanf("%d", &temp.oraFine.minuto);
                    if (error != 1 || temp.oraFine.minuto < 0 || temp.oraFine.minuto > 59) {
                        fflush(stdin);
                        setColor(ROSSO);
                        printf("[!] I minuti indicati non sono validi.\n");
                        setColor(GIALLO);
                    }
                } while(error != 1 || temp.oraFine.minuto < 0 || temp.oraFine.minuto > 59);
                error=0;

                differenza = differenzaOrari(temp.oraInizio, temp.oraFine);
                if (differenza < 1 || differenza > 4) {
                    error=1;
                }
                
            } while(error != 0);
            fflush(stdin);

        }else {
            temp.oraInizio.ora=found->oraInizio.ora;
            temp.oraInizio.minuto=found->oraInizio.minuto;
            temp.oraFine.ora=found->oraFine.ora;
            temp.oraFine.minuto=found->oraFine.minuto;
        }
        /**************
        * Ospite
        **************/ 
        do {
            inizializzaStringa(temp.ospite, 50);

            if (error) {
                fflush(stdin);
                setColor(ROSSO);
                printf("[!] Puoi inserire solo lettere, numeri, spazi e '&'!\n");
                setColor(GIALLO);
                error = 0;
            }

            if (error2) {
                fflush(stdin);
                setColor(ROSSO);
                printf("[!] Il nome dell'ospite deve avere minimo 2 caratteri e massimo 49.\n");
                setColor(GIALLO);
                error2 = 0;
            }

            printf("[Attuale: %s] Modifica ospite evento: ", found->ospite);
            scanf("%[^\n]", &temp.ospite);
            
            if (feof(stdin)) {
				strcpy(temp.ospite, found->ospite);
				fflush(stdin);
			}

            if (stringaValida(temp.ospite, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz &1234567890") == 1)
                error = 1;
            if (strlen(temp.ospite)>49 || strlen(temp.ospite)<2)
                error2 = 1;

        } while(error != 0 || error2 != 0);
        fflush(stdin);

        /**************
        * Locale
        **************/
        char nome[20] = {'\0'};
        locale* locale = NULL;

        do {

            if (error) {
                fflush(stdin);
                setColor(ROSSO);
                printf("[!] Non e' stato trovato alcun locale col nome '%s' !\n", nome);
                setColor(GIALLO);
                error = 0;
            }

            do {
                inizializzaStringa(nome, 20);

                if (error) {
                    fflush(stdin);
                    setColor(ROSSO);
                    printf("[!] Puoi inserire solo lettere, numeri e &!\n");
                    setColor(GIALLO);
                    error = 0;
                }

                if (error2) {
                    fflush(stdin);
                    setColor(ROSSO);
                    printf("[!] Il nome deve avere minimo 4 caratteri e massimo 19.\n");
                    setColor(GIALLO);
                    error2 = 0;
                }
                printf("\n\nNel sistema sono presenti i seguenti locali: \n");
                for(int i=0; i < locali->size; i++){
                    printf("%-12s %20s | %-15s %2d | %-15s %2d\n", 
                    "Nome locale:", locali->array[i].nome, 
                    "Numero file:", locali->array[i].file, 
                    "Numero posti:", locali->array[i].posti);
                }
                char* nomeLoc = NULL;
                locale = ricercaLocaleByCodice(locali, found->locale);
                if (locale != NULL)
                    nomeLoc = locale->nome;
                else
                    nomeLoc = "null";
                printf("[Attuale: %s] Modifica locale: ", nomeLoc);			
                scanf("%[^\n]", &nome);

                if (feof(stdin)) {
                    locale = ricercaLocaleByCodice(locali, found->locale);
                    fflush(stdin);
                    break;
                } else
                    locale = NULL;

                if (stringaValida(nome, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 &") == 1)
                    error = 1;
                if (strlen(nome)>19 || strlen(nome)<4)
                    error2 = 1;

            } while(error != 0 || error2 != 0);

            if (locale == NULL)
                locale = ricercaLocale(locali, nome);

            if (locale == NULL)
                error = 1;

        } while(error != 0);
        temp.locale = locale->codice;
        /******************
        * Costo prima fila
        *******************/
        do { 
            printf("[Attuale: %.2f$] Modifica costo prima fila: ", found->costo_prima_fila);
            error=scanf("%f", &temp.costo_prima_fila);
            if (feof(stdin)) {
                temp.costo_prima_fila = found->costo_prima_fila;
                fflush(stdin);
                break;
            }
            if (error!= 1 || temp.costo_prima_fila < 1) {
                fflush(stdin);
                setColor(ROSSO);
                printf("[!] Il costo indicato non e' valido.\n");
                setColor(GIALLO);
            }
        } while(error!= 1 || temp.costo_prima_fila < 1);
        error = 0;
        fflush(stdin);
        /******************
        * Costo ultima fila
        *******************/
        do{ 
            printf("[Attuale: %.2f$] Modifica costo ultima fila: ", found->costo_ultima_fila);
            error=scanf("%f", &temp.costo_ultima_fila);
            if (feof(stdin)) {
                temp.costo_ultima_fila = found->costo_ultima_fila;
                fflush(stdin);
                break;
            }
            if (error!= 1 || temp.costo_ultima_fila < 1) {
                fflush(stdin);
                setColor(ROSSO);
                printf("[!] Il costo indicato non e' valido.\n");
                setColor(DEFAULT);
            }

            if(temp.costo_ultima_fila>temp.costo_prima_fila){
                fflush(stdin);
                setColor(ROSSO);
                printf("[!] Il costo dell'ultima fila non puo' essere maggiore di quello della prima fila.\n");
                setColor(DEFAULT);
            }
        } while(error!= 1 || temp.costo_ultima_fila < 1 || temp.costo_ultima_fila>temp.costo_prima_fila);
        error = 0;
        fflush(stdin);

        memcpy(found, &temp, sizeof(evento));

    } else {
        return 0;
    }

    return 1;
}


/** 
 * Il programma esegue una ricerca di evento, e chiede una conferma per l'eliminazione. Successivamente l'evento trovato viene spostato in ultima posizione
 * e viene fatto un realloc del vettore dinamico che così esclude l'evento da eliminare.
 * 
 * @param [in] eventi: struct contentente l'indirizzo all'array di struct (utilizzato per effettuare operazioni concernenti agli eventi) e la gradezza della stessa.
 * 
 * @return 0: l'evento cercato non è stato trovato
 * @return 1: valore che indica il successo dell'operazione di eliminazione
 * @return -2: si è deciso di non eliminare l'evento
 * @return -1: se l'array di struct concernente i locali è nullo.
 */
int eliminaEvento(eventi* eventi, evento* entity, locali* locali) {

    if (eventi->array == NULL)
		return -1;

    fflush(stdin);
	system("cls");
    setColor(VIOLA);
    printf("\
			\t\n***********************************************\
			\t\n*               ELIMINA EVENTO                *\
			\t\n***********************************************\n");
    setColor(DEFAULT);

    evento* found;
    if (entity == NULL)
        found = ricercaEvento(eventi, locali);
    else
        found = entity;
    
    if (found != NULL) {

        visualizzaEvento(eventi, found, locali);

        int error;
        int scelta;
        do {
            setColor(ROSSO);	
		    printf("\n\nVuoi veramente eliminare questo evento? (0 : no | 1 : si): ");		
		    error = scanf("%d", &scelta);
		    if (error != 1 || scelta < 0 || scelta > 1){
				fflush(stdin);
				printf("[!] Puoi inserire solo un numero tra quelli indicati.\n");
				error=0;
		    }
	    } while(error != 1 || scelta < 0 || scelta > 1);
        setColor(DEFAULT);
        if (scelta) {

            evento temp;

            /*
             * Viene effettuato lo swap tra l'evento trovato e l'elemento in ultima posizione per poi eliminarlo
             */

            memcpy(&temp, found, sizeof(evento));
            memcpy(found, &eventi->array[eventi->size - 1], sizeof(evento));
            memcpy(&eventi->array[eventi->size - 1], &temp, sizeof(evento));
            eventi->array = (evento*) realloc(eventi->array, sizeof(evento) * --eventi->size);
        } else
            return 2;

    } else
        return 0;
    

    return 1;
}

