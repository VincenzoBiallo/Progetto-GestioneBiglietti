#include "prenotazioni.h"
#include <time.h>

/**
 * La funzione apre il file di riferimento, legge riga per riga il file CSV, le analizza e le inserisce in elementi di un vettore dinamico
 * che viene generato in base al numero di righe che vengono lette dal file.
 * 
 * @param [in] prenotazioni : puntatore a struct prenotazioni
 * @param [in] indexs : puntatore a struct index
 * 
 * @return 0 : il vettore è stato caricato con successo
 * @return -1 : impossibile aprire il file
 * 
 */
int caricaPrenotazioni(prenotazioni* prenotazioni, struct index* indexs) {

    FILE* fp;
    
    if ((fp = fopen("./files/prenotazioni.csv", "r")) != NULL) {

        char buffer[1000];
        int lines = contaLinee(fp);
        if (lines == -1) 
            return -1;    

        prenotazioni->size = lines;
        prenotazioni->array = calloc(prenotazioni->size, sizeof(prenotazione));

        int count = 0;

        // Viene letta ogni riga del file CSV e viene caricata in un buffer temporaneo di 1000 caratteri
		while (fgets(buffer, sizeof(buffer), fp)) {

            char* token;
            char* line = buffer;
            char** input = &line;

            token = myParser(input, ";");
			prenotazioni->array[count].numero = atoi(token);

            token = myParser(input, ";");
			prenotazioni->array[count].evento = atoi(token);

            token = myParser(input, ";");
			strcpy(prenotazioni->array[count].nickname, token);

            token = myParser(input, ";");
			prenotazioni->array[count].fila = atoi(token);

            token = myParser(input, ";");
			prenotazioni->array[count].n_poltrona = atoi(token);

            token = myParser(input, ";");
			prenotazioni->array[count].importo = atof(token);

            count++;
        }

        prenotazioni->index = &indexs->prenotazione;
        fclose(fp); 
    } else {
        setColor(ROSSO);
		printf("[!] Impossibile aprire il file prenotazioni.csv\n");
		setColor(DEFAULT);
        FILE* fp = fopen("./files/prenotazioni.csv", "w");
        fclose(fp);
        return -1;
    }

    return 0;
}

/**
 * La funzione cancella e ricrea il file di riferimento inserendo una riga per ogni struct nel vettore dinamico. In seguito viene distrutto il vettore e viene richiamata
 * la funzione caricaPrenotazioni
 * 
 * @param [in] prenotazioni : puntatore a struct prenotazioni
 * @param [in] indexs : puntatore a struct index
 * 
 * @return 0 : il vettore è stato salvato su file
 * @return -1 : il vettore dinamico non esiste oppure non è stato possibile salvare il file
 * 
 */
int salvaPrenotazioni(prenotazioni* prenotazioni, struct index* indexs) {

    if (prenotazioni->array == NULL) {
        setColor(GIALLO);
		printf("[!] Il vettore delle prenotazioni non esiste in memoria!\n");
		setColor(DEFAULT);
        FILE* fp = fopen("./files/prenotazioni.csv", "w");
        fclose(fp);
        return -1;
    }

    FILE* fp;
    
    if ((fp = fopen("./files/prenotazioni.csv", "w")) != NULL) {

        for (int i=0; i<prenotazioni->size;i++) {
            fprintf(fp, "%d;", prenotazioni->array[i].numero);
            fprintf(fp, "%d;", prenotazioni->array[i].evento);
            fprintf(fp, "%s;", prenotazioni->array[i].nickname);
            fprintf(fp, "%d;", prenotazioni->array[i].fila);
            fprintf(fp, "%d;", prenotazioni->array[i].n_poltrona);
            fprintf(fp, "%.2f\n", prenotazioni->array[i].importo);
        }
        fclose(fp);
    } else {
        setColor(ROSSO);
		printf("[!] Impossibile salvare il file prenotazioni.csv\n");
		setColor(DEFAULT);
        return -1;
    }

    free(prenotazioni->array);
    caricaPrenotazioni(prenotazioni, indexs);
    
    return 0;
}

/**
 * La funzione visualizza il biglietto per partecipare ad un evento con i relativi dati.
 * 
 * @param [in] nomeEvento : array di caratteri che contiene il nome dell'evento a cui il biglietto si riferisce.
 * @param [in] nBiglietto : intero che indica il numero del biglietto.
 * @param [in] dataPrenotazione : struct che contiene la data della prenotazione.
 * @param [in] nickname : array d caratteri che contiene il nickname dell'utente che ha acquistato il biglietto.
 * @param [in] nFila : intero che indica il numero della fila prenotata.
 * @param [in] nPoltrona : intero che india il numero della poltrona prenotata.
 * @param [in] importo : numero in virgola mobile che indica l'importo pagato per l'acquisto del biglietto.
 */
void visualizzaBiglietto(char* nomeEvento, int nBiglietto, data dataEvento, char* nickname, int nFila, int nPoltrona, char* nomeLocale, float importo){
    
    system("cls");
    setColor(CELESTE);
    printf("\
            \t\n***********************************************\
            \t\n*               VENDI BIGLIETTO               *\
            \t\n***********************************************\n");
    setColor(DEFAULT);
    printf("\n%25s TICKET\
            \n==================================================\
            \n|| NUMERO BIGLIETTO: %3d\
            \n|| DATA: %02d/%02d/%d\
            \n||\
            \n||                       NICKNAME: %s\
            \n||                       LOCALE: %s\
            \n||                       FILA: %7d\
            \n||                       POLTRONA: %3d\
            \n||                       IMPORTO: %3.2f\
            \n||\
            \n==================================================\
            \n", nomeEvento, nBiglietto, dataEvento.giorno, dataEvento.mese, dataEvento.anno, nickname, nomeLocale, nFila, nPoltrona, importo); 
    printf("\n\n");

}

/**
 * La funzione sposta in ultima posizione la prenotazione da eliminare(rimborsare) ed effettua un realloc del vettore dinamico, escludendo la prenotazine da eliminare.
 * 
 * @param [in] prenotazioni : struct contenente, tra gli altri campi, l'indirizzo all'array di struct utilizzato per effettuare operazioni concernenti alle prenotazioni.
 * @param [in] entity : indirizzo a una struct prenotazione.
 * 
 * @return -1 : se il vettore delle prenotazioni è nullo.
 * @return 1 : se l'eliminazione è avvenuta con successo.
 * @return 0 : se l'eliminazione non è avvenuta.
 * 
 */
int rimborsaBiglietto(prenotazioni* prenotazioni, prenotazione* entity) {

    if (prenotazioni->array == NULL)
		return -1;

    prenotazione* found = entity;
    prenotazione temp;
    
    if (found != NULL) {
        memcpy(&temp, found, sizeof(prenotazione));
        memcpy(found, &prenotazioni->array[prenotazioni->size - 1], sizeof(prenotazione));
        memcpy(&prenotazioni->array[prenotazioni->size - 1], &temp, sizeof(prenotazione));
        prenotazioni->array = (prenotazione*) realloc(prenotazioni->array, sizeof(prenotazione) * --prenotazioni->size);
     } else
        return 0;
    
    return 1;
}

/**
 * La funzione calcola la differenza tra due date in secondi, se il secondo parametro vale NULL confronta la prima data con la data odierna.
 * 
 * @param [in] data : struct che rappresenta una data, viene passata come parametro per stabilire se è una data passata o futura.
 * @param [in] entity : indirizzo di struct data.
 * 
 * @return 1 : se la prima data è maggiore della seconda, oppure se inserita solo 1 data quest'ultima è una data futura.
 * @return 0 : se la prima data è minore della seconda, oppure se inserita solo 1 data quest'ultima è una data passata.
 */
int differenzaDate(data data, struct data* entity){

    struct tm date1;
    
	date1.tm_year = data.anno - 1900;
	date1.tm_mon = data.mese - 1;
	date1.tm_mday = data.giorno;
	date1.tm_hour = 0;
	date1.tm_min = 0;
	date1.tm_sec = 1;
	date1.tm_isdst = -1; //per far si chè il codice della libreria di runtime del linguaggio C calcoli se è attiva l'ora legale o l'ora solare.

	int secondi=_mktime64(&date1); // gestisce da 1 gennaio 1970 al 23:59:59 a 31 dicembre 3000

    if(entity != NULL) {
        struct tm date2;

        date2.tm_year = entity->anno - 1900;
        date2.tm_mon =  entity->mese - 1;
        date2.tm_mday = entity->giorno;
        date2.tm_hour = 0;
        date2.tm_min = 0;
        date2.tm_sec = 1;
        date2.tm_isdst = -1;

        int secondi2=_mktime64(&date2);
        
        if(secondi > secondi2 )
		    return 1;
        else
            return 0; 

    }

	if(secondi > time(NULL) )
		return 1;
    else
        return 0; 
}

/**
 * La funzione visualizza uan simulazione delle sala, evidenziando in rosso i posti già prenotati, in verde quelli liberi oppure evidenzia i posti liberi in giallo se si tratta di un evento passato, quindi non prenotabile.
 * 
 * @param [in] prenotazioni : struct contenente, tra gli altri campi, l'indirizzo all'array di struct utilizzato per effettuare operazioni concernenti alle prenotazioni.
 * @param [in] evento : struct evento.
 * 
 */
void visualizzaDisponibilitaSala(prenotazioni* prenotazioni, eventi* eventi, evento* entity, locali* locali) {

    int i, j;

    evento* evento;
    if (entity == NULL) {
        system("cls");
        setColor(CELESTE);
        printf("\
            \t\n***********************************************\
            \t\n*               VISUALIZZA SALA               *\
            \t\n***********************************************\n");
        setColor(DEFAULT);
        evento = ricercaEvento(eventi, locali);
        if(evento==NULL){
            setColor(ROSSO);
            printf("[!] L'evento indicato non esiste.\n");
            setColor(DEFAULT);
            return;
        }
    } else
        evento = entity;

    locale* locale = ricercaLocaleByCodice(locali, evento->locale);
        
    if(differenzaDate(evento->data, NULL)==1){

        printf("\nLOCALE: %s\n", locale->nome);
        for (j=1; j<=locale->posti;j++){
            if(j==1)
                printf("   ");
            printf(" %2d ", j);
        }

        for(i = 1; i<=locale->file; i++) {		
                
            if(i==1){
                printf("\n");	
                printf("%2d |", i);
            } else 
                printf("%2d |", i);

            for (j = 1; j<=locale->posti; j++) {	
                if((ricercaPrenotazione(prenotazioni, i, j, evento->codice))==NULL){
                    setColor(VERDE);
                    printf(" 0 ");
                    setColor(DEFAULT);
                    printf("|");
                }else {
                    setColor(ROSSO);
                    printf(" 0 ");
                    setColor(DEFAULT);
                    printf("|");
                }
                if( j == locale->posti)
                        printf("\n");
            }
        }
    }else{
        setColor(GIALLO);
        printf("[!] L'evento '%s' e' un evento passato.\n", evento->nome);
        setColor(DEFAULT);

        printf("\nLOCALE: %s\n", locale->nome);
        for (j=1; j<=locale->posti;j++){
            if(j==1)
                printf("   ");
             printf(" %2d ", j);
        }

        for(i = 1; i<=locale->file; i++) {		
                
            if(i==1){
                printf("\n");	
                printf("%2d |", i);
            } else 
                printf("%2d |", i);
            for (j = 1; j<=locale->posti; j++) {	
                if((ricercaPrenotazione(prenotazioni, i, j, evento->codice))==NULL){
                    setColor(GIALLO);
                    printf(" 0 ");
                    setColor(DEFAULT);
                    printf("|");
                }else {
                    setColor(ROSSO);
                    printf(" 0 ");
                    setColor(DEFAULT);
                    printf("|");
                }
                if( j == locale->posti)
                    printf("\n");
                }
        }

    }
}

/**
 * La funzione visualizza una lista degli eventi prenotabili (quindi quelli futuri, escludendo quelli passati) e il modulo di vendita di un biglietto ad un utente, ne effettua la vendita dopo la sua conferma.
 * 
 * @param [in] eventi : struct contentente (tra i vai campi) l'indirizzo all'array di struct utilizzato per effettuare operazioni concernenti gli eventi. 
 * @param [in] locali: struct contentente, tra gli altri campi, l'indirizzo all'array di struct utilizzato per effettuare operazioni concernenti ai locali.
 * @param [in] utenti: struct contentente l'indirizzo all'array di struct (utilizzato per effettuare operazioni concernenti gli utenti) e la gradezza della stessa.
 * @param [in] prenotazioni : struct contenente, tra gli altri campi, l'indirizzo all'array di struct utilizzato per effettuare operazioni concernenti alle prenotazioni.
 * 
 * @return 1 : se il biglietto è stato venduto correttamente.
 * @return 0 : se l'utente non conferma la vendita del biglietto.
 * @return 2 : se l'evento selezionato è sold out.
 */
int vendiBiglietto(eventi* eventi, locali* locali, utenti* utenti, prenotazioni* prenotazioni) {

    prenotazione temp;
    prenotazione* ritorno;
    evento* found;
    char nomeEvento[50];
    int error = 0, error2 = 0, sentinelNick = 0;

    data tempData = {0, 0, 0};

    visualizzaBiglietto(" ", 0, tempData , " ", 0, 0, " ", 0);
    setColor(GIALLO);
    printf("Gli eventi prenotabili sono i seguenti:\n");
    for(int i=0; i<eventi->size;i++){
        if(differenzaDate(eventi->array[i].data, NULL)==1){
            printf("%-13s %s | %-13s %02d/%02d/%4d | %-13s %s\n",
                   "Nome evento:", eventi->array[i].nome,
                   "Data evento:", eventi->array[i].data.giorno, eventi->array[i].data.mese, eventi->array[i].data.anno,
                   "Ospite:", eventi->array[i].ospite);
        }
    }
    setColor(DEFAULT);
    printf("\n\n");
    /**************
	 * Nome evento
	**************/
    do{
        fflush(stdin);
        do {
            inizializzaStringa(nomeEvento, 50);
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

            printf("Inserisci nome evento: ");			
            scanf("%[^\n]", &nomeEvento);

            if (stringaValida(nomeEvento, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 &") == 1)
                error = 1;

            if (strlen(nomeEvento)>49 || strlen(nomeEvento)<4)
                error2 = 1;

        } while(error != 0 || error2 != 0);

        found=ricercaEventoByString(eventi, nomeEvento, 2, locali);

        if (found != NULL){
            
            //Controllo se un evento è soldout
            locale* locTemp = ricercaLocaleByCodice(locali, found->codice);
            int count=0;
            for(int f=1; f<=locTemp->file; f++){
                for(int p=1; p<=locTemp->posti; p++){
                    if(ricercaPrenotazione(prenotazioni, f, p, found->codice)!=NULL)
                        count++;
                }
            }

            if(count==(locTemp->file * locTemp->posti)){
                return 2;
            }

            /**************
	        * Codice evento
	        **************/
            temp.evento=found->codice;
            strcpy(nomeEvento, found->nome);
            if(differenzaDate(found->data, NULL)!=1){
                setColor(ROSSO);
                printf("[!] Hai selezionato un evento passato.\n");
                setColor(DEFAULT);
                found=NULL;
            }
                
        }
        else {
            setColor(GIALLO);
            printf("[!] Non e' stata trovata alcuna corrispondenza con il nome: %s.\n", nomeEvento);
            setColor(DEFAULT);
        }

        

    }while(found == NULL);
    fflush(stdin);


    /**************
	* Numero biglietto
	**************/
    temp.numero = (*(prenotazioni->index))++;

    /**************
	* Nickname
	**************/
    locale* localeFound = ricercaLocaleByCodice(locali, found->locale);
    visualizzaBiglietto(nomeEvento, temp.numero, found->data , " ", 0, 0, localeFound->nome, 0);
    do{
        sentinelNick=0;
        do{
            inizializzaStringa(temp.nickname, 16);

            if (error) {
                fflush(stdin);
                setColor(ROSSO);
                printf("[!] Puoi inserire solo lettere!\n");
                setColor(DEFAULT);
                error = 0;
            }

            if (error2) {
                fflush(stdin);
                setColor(ROSSO);
                printf("[!] Il nickname deve avere minimo 4 caratteri e massimo 15.\n");
                setColor(DEFAULT);
                error2 = 0;
            }

            printf("Inserisci nickname: ");			
            scanf("%[^\n]", &temp.nickname);

            if (stringaValida(temp.nickname, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz") == 1)
                error = 1;
            if (strlen(temp.nickname)>15 || strlen(temp.nickname)<4)
                error2 = 1;

        } while(error != 0 || error2 != 0);

        if (esisteUtente(utenti, temp.nickname) == 0){
            setColor(ROSSO);
            printf("[!] L'utente '%s' non e' stato trovato.\n", temp.nickname);
            setColor(DEFAULT);
            fflush(stdin);
            sentinelNick=1;
        }
        
    }while(sentinelNick==1);
    fflush(stdin);
    
    /**************
	* Fila e Posto
	**************/
    visualizzaBiglietto(nomeEvento, 1, found->data , temp.nickname, 0, 0, localeFound->nome, 0);
    visualizzaDisponibilitaSala(prenotazioni, eventi, found, locali);
    do{
         do{
            printf("\n[Il locale dell'evento possiede %d file] Inserisci numero fila: ", localeFound->file);
            error=scanf("%d", &temp.fila);
            if (error != 1 || temp.fila < 1 || temp.fila>localeFound->file){
                fflush(stdin);
                setColor(ROSSO);
                printf("[!] Puoi inserire solo numeri che rispettino le grandezze del locale.\n");
                setColor(DEFAULT);
            }
        }while(error != 1 || temp.fila < 1 || temp.fila>localeFound->file);
        error=0;
        fflush(stdin);

        do{
            printf("[Il locale dell'evento possiede %d poltrone per fila] Inserisci numero poltrona: ", localeFound->posti);
            error=scanf("%d", &temp.n_poltrona);
            if (error != 1 || temp.n_poltrona < 1 || temp.n_poltrona>localeFound->posti){
                fflush(stdin);
                setColor(ROSSO);
                printf("[!] Puoi inserire solo numeri che rispettino le grandezze del locale.\n");
                setColor(DEFAULT);
            }
        }while(error != 1 || temp.n_poltrona < 1 || temp.n_poltrona>localeFound->posti);
        ritorno=ricercaPrenotazione(prenotazioni, temp.fila, temp.n_poltrona, temp.evento);
        if (ritorno!=NULL){
            setColor(ROSSO);
            printf("[!] Il posto selezionato non e' disponibile!\n");
            setColor(DEFAULT);
        }
    }while(ritorno!=NULL);

    error=0;
    fflush(stdin);


    /*******************
	* Importo biglietto
	********************/
    float imptemp = (float) ( ( (float)( temp.fila - 1) / ( localeFound->file - 1) ) * ((float)( found->costo_prima_fila - found->costo_ultima_fila)) );
    temp.importo = (float)found->costo_prima_fila - (float) imptemp;
    visualizzaBiglietto(nomeEvento, 1, found->data , temp.nickname, temp.fila, temp.n_poltrona, localeFound->nome, temp.importo);

    //conferma vendita biglietto
    int scelta;
    do{
        setColor(GIALLO);
        printf("Desideri confermare la vendita? (0 : no | 1 : si): ");
        setColor(DEFAULT);
        error=scanf("%d", &scelta);
        if (error != 1 || scelta < 0 || scelta > 1){
            setColor(ROSSO);
            printf("[!] L'opzione indicata non e' valida.\n");
            setColor(DEFAULT);
            fflush(stdin);
        }
    }while(error != 1 || scelta < 0 || scelta > 1);

    if(scelta){
        prenotazioni->array = (prenotazione*) realloc(prenotazioni->array, sizeof(prenotazione) * ++prenotazioni->size);
        memcpy(&prenotazioni->array[prenotazioni->size - 1], &temp, sizeof(temp));
        return 1;
    }else
        return 0; 

}


/**
 * La funzione ricerca una prenotazione di un determinato evento, considerando la fila, la poltrona e il codice dell'evento.
 * 
 * @param [in] prenotazioni : struct contenente, tra gli altri campi, l'indirizzo all'array di struct utilizzato per effettuare operazioni concernenti alle prenotazioni.
 * @param [in] fila : numero fila ricercata.
 * @param [in] posto : numero posto ricercato.
 * @param [in] codiceEvento : codice evento del quale è ricercata la prenotazione.
 * 
 * @return prenotazioni->array[i] : indirizzo del puntatore alla locazione di memoria in cui è stata trovata la prenotazione.
 * @return NULL : se la prenotazione non è stata trovata o il vettore delle prenotazioni è nullo.
 */
prenotazione* ricercaPrenotazione(prenotazioni* prenotazioni, int fila, int posto, int codiceEvento) {

    if (prenotazioni->array == NULL) {
        return NULL;
    }

    for(int i=0; i<prenotazioni->size; i++){
        if(fila==prenotazioni->array[i].fila && posto==prenotazioni->array[i].n_poltrona && codiceEvento==prenotazioni->array[i].evento)
            return &prenotazioni->array[i];
    }

    return NULL;
}

/**
 * La funzione ricerca una prenotazione in base al suo numero (utilizzata solo a livello operativo).
 * 
 * @param [in] prenotazioni : struct contenente, tra gli altri campi, l'indirizzo all'array di struct utilizzato per effettuare operazioni concernenti alle prenotazioni.
 * @param [in] numero : numero della prenotazione
 * 
 * @return NULL : se il vettore delle prenotazioni è nullo oppure non è stata trovata nessuna prenotazione con il numero indicato.
 * @return prenotazioni->array[i] : se la prenotazione è stata trovata e viene restituito il puntatore alla locazione di memoria interessata dell'array delle prenotazioni.
 */
prenotazione* ricercaPrenotazioneByNumero(prenotazioni* prenotazioni, int numero) {

    if (prenotazioni->array == NULL) {
        setColor(GIALLO);
		printf("[!] Il vettore deglle Prenotazioni e' nullo!\n");
		setColor(DEFAULT);
        return NULL;
    }

    for(int i=0; i<prenotazioni->size; i++){
        if(prenotazioni->array[i].numero == numero)
            return &prenotazioni->array[i];
    }

    return NULL;
}

/**
 * La funzione visualizza le informazioni riguardanti gli utenti che hanno comprato un biglietto di un evento ricercato in input (senza rindondanze).
 * 
 * @param [in] prenotazioni : struct contenente, tra gli altri campi, l'indirizzo all'array di struct utilizzato per effettuare operazioni concernenti alle prenotazioni.
 * @param [in] eventi : struct contentente (tra i vai campi) l'indirizzo all'array di struct utilizzato per effettuare operazioni concernenti gli eventi. 
 * @param [in] utenti: struct contentente l'indirizzo all'array di struct (utilizzato per effettuare operazioni concernenti gli utenti) e la gradezza della stessa.
 * @param [in] locali: struct contentente, tra gli altri campi, l'indirizzo all'array di struct utilizzato per effettuare operazioni concernenti ai locali.
 * 
 */
void visualizzaSpettatoriEvento(prenotazioni* prenotazioni, eventi* eventi, utenti* utenti, locali* locali) {

    system("cls");
    setColor(VIOLA);
    printf("\
			\t\n***********************************************\
			\t\n*       VISUALIZZA SPETTATORI EVENTO          *\
			\t\n***********************************************\n\n");
    setColor(DEFAULT);

    evento* evento = ricercaEvento(eventi, locali);
    char nickname[16];
    char* buffer = calloc(utenti->size, sizeof(nickname));
    unsigned int occorrenza = 0; 
    int exist = 0;
    int error = 0, error2 = 0;

    for (int i=0; i<prenotazioni->size; i++) {

        if (prenotazioni->array[i].evento == evento->codice) {
            inizializzaStringa(nickname, 16);
            strcpy(nickname, prenotazioni->array[i].nickname);

            for (int j=0; j<=(occorrenza/sizeof(nickname)); j++) {
                if (strcmp(myLower(nickname), myLower(&buffer[j * sizeof(nickname)])) == 0) {
                    exist = 1;
                    break;
                }           
            }

            if (exist) {
                exist = 0;
                continue;
            }

            strcpy(&buffer[occorrenza], nickname);
            occorrenza += sizeof(nickname);
        }
    }

    if (occorrenza >= 1) {
        printf("Utenti partecipanti all'evento '%s' in data '%02d/%02d/%02d':\n", evento->nome, evento->data.giorno, evento->data.mese, evento->data.anno);
        for (int k=0; k<(occorrenza/sizeof(nickname)); k++) {
            visualizzaUtente(utenti, ricercaUtente(utenti, &buffer[k * sizeof(nickname)]));
        }        
    } else {
        setColor(ROSSO);
        printf("Nessun utente partecipante all'evento '%s'!\n", evento->nome);
        setColor(DEFAULT);
    }
    
    free(buffer);
}

/**
 * La funzione visualizza le informazioni riguardanti gli eventi di cui un utente ha acquistato il biglietto (senza rindondanze).
 * 
 * @param [in] prenotazioni : struct contenente, tra gli altri campi, l'indirizzo all'array di struct utilizzato per effettuare operazioni concernenti alle prenotazioni.
 * @param [in] eventi : struct contentente (tra i vai campi) l'indirizzo all'array di struct utilizzato per effettuare operazioni concernenti gli eventi. 
 * @param [in] utenti: struct contentente l'indirizzo all'array di struct (utilizzato per effettuare operazioni concernenti gli utenti) e la gradezza della stessa.
 * @param [in] locali: struct contentente, tra gli altri campi, l'indirizzo all'array di struct utilizzato per effettuare operazioni concernenti ai locali.
 * 
 */
void visualizzaEventiUtente(prenotazioni* prenotazioni, eventi* eventi, utenti* utenti, locali* locali) {

    system("cls");
    setColor(CELESTE);
    printf("\
			\t\n***********************************************\
			\t\n*       VISUALIZZA EVENTI DI UN UTENTE        *\
			\t\n***********************************************\n\n");
    setColor(DEFAULT);

    struct eventi copiaDiEventi;
    memcpy(&copiaDiEventi, eventi, sizeof(struct eventi));
    mergeSort(&copiaDiEventi, _eventi, 0, copiaDiEventi.size -1);

    char nickname[16];
    int* buffer = calloc(prenotazioni->size, sizeof(int));
    unsigned int occorrenza = 0;
    int codEvento = 0;
    int exist = 0;
    int error = 0, error2 = 0;

    do {
        exist=0;
        do {
            inizializzaStringa(nickname, 16);
            if (error) {
                fflush(stdin);
                setColor(ROSSO);
                printf("[!] Puoi inserire solo lettere!\n");
                setColor(DEFAULT);
                error = 0;
            }
            if (error2) {
                fflush(stdin);
                setColor(ROSSO);
                printf("[!] Il nickname deve avere minimo 4 caratteri e massimo 15.\n");
                setColor(DEFAULT);
                error2 = 0;
            }
            printf("Inserisci nickname: ");			
            scanf("%[^\n]", &nickname);

            if (stringaValida(nickname, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz") == 1)
                error = 1;
            if (strlen(nickname) > 15 || strlen(nickname) < 4)
                error2 = 1;

        } while(error != 0 || error2 != 0);
        fflush(stdin);

        if (esisteUtente(utenti, nickname) == 0){
                setColor(ROSSO);
                printf("[!] L'utente '%s' non e' stato trovato.\n", nickname);
                setColor(DEFAULT);
                fflush(stdin);
                exist=1;
        }
    }while(exist==1);
    
    for (int i=0; i<prenotazioni->size; i++) {

        if (strcmp(myLower(prenotazioni->array[i].nickname), myLower(nickname)) == 0) {

            codEvento = prenotazioni->array[i].evento;

            for (int j=0; j<prenotazioni->size; j++) {
                if (buffer[j] == codEvento) {
                    exist = 1;
                    break;
                }           
            }

            if (exist) {
                exist = 0;
                continue;
            }

            buffer[occorrenza++] = codEvento;
        }
    }

    if (occorrenza >= 1) {
        ordinaEventi(eventi, buffer, occorrenza);
        printf("L'utente '%s' ha partecipato ai seguenti eventi:\n", nickname);
        for (int k=0; k<occorrenza; k++)
            visualizzaEvento(eventi, ricercaEventoByCodice(eventi, buffer[k]), locali);
    } else {
        setColor(ROSSO);
        printf("L'utente '%s' non ha partecipato a nessun evento!\n", nickname);
        setColor(DEFAULT);
    }
    
    free(buffer);
}

/**
 * La funzione visualizza a schermo l'incasso totale di un evento ricercato in input.
 * 
 * @param [in] eventi : struct contentente (tra i vai campi) l'indirizzo all'array di struct utilizzato per effettuare operazioni concernenti gli eventi. 
 * @param [in] prenotazioni : struct contenente, tra gli altri campi, l'indirizzo all'array di struct utilizzato per effettuare operazioni concernenti alle prenotazioni.
 * @param [in] locali: struct contentente, tra gli altri campi, l'indirizzo all'array di struct utilizzato per effettuare operazioni concernenti ai locali.
 * 
 */
void visualizzaIncassoEvento(eventi* eventi, prenotazioni* prenotazioni, locali* locali) {

    float incassoTot = 0;
    int count = 0;
    system("cls");
    setColor(VIOLA);
    printf("\
			\t\n***********************************************\
			\t\n*          VISUALIZZA INCASSO EVENTO          *\
			\t\n***********************************************\n\n");
    setColor(DEFAULT);

    evento* found=ricercaEvento(eventi, locali);
    if(found==NULL){
        setColor(ROSSO);
        printf("[!] L'evento inserito non esiste.\n");
        setColor(DEFAULT);
        return;
    }

    for(int i=0; i<prenotazioni->size; i++){

        if(prenotazioni->array[i].evento==found->codice){
            incassoTot+=prenotazioni->array[i].importo;
            count++;
        }
    }

    setColor(GIALLO);
    printf("\n\nL'evento '%s', con '%d' prenotazioni, ha registrato il seguente incasso: %.2f$.\n", found->nome, count, incassoTot);
    setColor(DEFAULT);

}

/**
 * La funzione effettua un ordinamento degli eventi (inseriti in un buffer temporaneo) in base alla data o al costo del biglietto.
 * Il parametro di ordinamento è scelto dall'utente in input.
 * 
 * @param [in] eventi : struct contentente (tra i vai campi) l'indirizzo all'array di struct utilizzato per effettuare operazioni concernenti gli eventi. 
 * @param [in] memory : puntatore al buffer temporaneo di codici degli eventi.
 * @param [in] size : grandezza del buffer.
 * 
 */
void ordinaEventi(eventi* eventi, int* memory, const unsigned int size) {

    evento* evento1 = NULL;
    evento* evento2 = NULL;
    int temp;
    int scelta=0, error=0;

    do{
        printf("\n 1.Data\
                \n 2.Costo biglietto\
                \nIndica un parametro per una visualizzazione ordinata:");
        error=scanf("%d", &scelta);
        if(error != 1 || scelta < 1 || scelta > 2){
            setColor(ROSSO);
            printf("[!] Indica un'opzione valida.\n");
            setColor(DEFAULT);
            fflush(stdin);
        }
    }while(error != 1 || scelta < 1 || scelta > 2);

    if(scelta == 1) {
        for (int i=1; i<size; i++) {
            for (int j=0; j<size-1; j++) {
                evento1 = ricercaEventoByCodice(eventi, memory[j]);
                evento2 = ricercaEventoByCodice(eventi, memory[j+1]);
                
                if(differenzaDate(evento1->data, &evento2->data) == 1) {
                    temp = memory[j];
                    memory[j] = memory[j+1];
                    memory[j+1] = temp;
                }

            }

        }
    } else if (scelta == 2) {
        for (int i=1; i<size; i++) {
            for (int j=0; j<size-1; j++) {
                evento1 = ricercaEventoByCodice(eventi, memory[j]);
                evento2 = ricercaEventoByCodice(eventi, memory[j+1]);
                
                if((float)evento1->costo_prima_fila > (float)evento2->costo_prima_fila) {
                    temp = memory[j];
                    memory[j] = memory[j+1];
                    memory[j+1] = temp;
                }

            }

        }
    }

}