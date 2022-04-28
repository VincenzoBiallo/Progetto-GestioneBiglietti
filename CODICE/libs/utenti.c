#include "utenti.h"

/**
 * La funzione apre il file di riferimento, legge riga per riga il file CSV, le analizza e le inserisce in elementi di un vettore dinamico
 * che viene generato in base al numero di righe che vengono lette dal file.
 * 
 * @param [in] utenti : puntatore a struct utenti
 * 
 * @return 0 : il vettore è stato caricato con successo
 * @return -1 : impossibile aprire il file
 * 
 */
int caricaUtenti(utenti* utenti) {

    FILE* fp;
    
    if ((fp = fopen("./files/utenti.csv", "r")) != NULL) {

        char buffer[1000];
        int lines = contaLinee(fp);
        if (lines == -1) 
            return -1;    

        utenti->size = lines;
        utenti->array = calloc(utenti->size, sizeof(utente));

        int count = 0;

        // Viene letta ogni riga del file CSV e viene caricata in un buffer temporaneo di 1000 caratteri
		while (fgets(buffer, sizeof(buffer), fp)) {

            char* token;
            char* line = buffer;
            char** input = &line;

            token = myParser(input, ";");
			strcpy(utenti->array[count].nickname, token);

            token = myParser(input, ";");
			strcpy(utenti->array[count].email, token);

            token = myParser(input, ";");
			strcpy(utenti->array[count].cognome, token);

            token = myParser(input, ";");
			strcpy(utenti->array[count].nome, token);

            token = myParser(input, ";");
			utenti->array[count].genere = atoi(token);         

            token = myParser(input, ";");
            if (token != NULL) {
                int day = 0;
                int month = 0;
                int year = 0;
                sscanf(token, "%d/%d/%d", &day, &month, &year);
                utenti->array[count].dataNascita.giorno = day;
                utenti->array[count].dataNascita.mese = month;
                utenti->array[count].dataNascita.anno = year;                      
            }
            
            token = myParser(input, ";");
			strcpy(utenti->array[count].telefono, token);

            token = myParser(input, ";");
			utenti->array[count].genereMusicale = atoi(token);

            count++;
        }
        fclose(fp); 
    } else {
        setColor(ROSSO);
		printf("[!] Impossibile aprire il file utenti.csv\n");
		setColor(DEFAULT);
		FILE* fp = fopen("./files/utenti.csv", "w");
		fclose(fp);
        return -1;
    }

    return 0;
}

/**
 * La funzione cancella e ricrea il file di riferimento inserendo una riga per ogni struct nel vettore dinamico. In seguito viene distrutto il vettore e viene richiamata
 * la funzione caricaUtenti
 * 
 * @param [in] utenti : puntatore a struct utenti
 * 
 * @return 0 : il vettore è stato salvato su file
 * @return -1 : il vettore dinamico non esiste oppure non è stato possibile salvare il file
 * 
 */
int salvaUtenti(utenti* utenti) {

    if (utenti->array == NULL) {
        setColor(GIALLO);
		printf("[!] Il vettore degli utenti non esiste in memoria!\n");
		setColor(DEFAULT);
		FILE* fp = fopen("./files/utenti.csv", "w");
		fclose(fp);
        return -1;
    }

    FILE* fp;
    
    if ((fp = fopen("./files/utenti.csv", "w")) != NULL) {

        for (int i=0; i<utenti->size;i++) {
            fprintf(fp, "%s;", utenti->array[i].nickname);
            fprintf(fp, "%s;", utenti->array[i].email);
            fprintf(fp, "%s;", utenti->array[i].cognome);
            fprintf(fp, "%s;", utenti->array[i].nome);
            fprintf(fp, "%d;", utenti->array[i].genere);
            if (utenti->array[i].dataNascita.anno != 0)
                fprintf(fp, "%d/%d/%d;", utenti->array[i].dataNascita.giorno, utenti->array[i].dataNascita.mese, utenti->array[i].dataNascita.anno);
            else
                fprintf(fp, ";", NULL);
            fprintf(fp, "%s;", utenti->array[i].telefono);
            fprintf(fp, "%d\n", utenti->array[i].genereMusicale);
        }
        fclose(fp);
    } else {
        setColor(ROSSO);
		printf("[!] Impossibile salvare il file utenti.csv\n");
		setColor(DEFAULT);
        return -1;
    }

    free(utenti->array);
    caricaUtenti(utenti);
    return 0;
}

/**
 * La funzione visualizza a schermo il modulo di inserimento dati e permette la registrazione di un nuovo utente sul file CSV apposito.
 * 
 * @param [in] utenti: struct contentente l'indirizzo all'array di struct (utilizzato per effettuare operazioni concernenti gli utenti) e la gradezza della stessa.
 * 
 * @return 1: valore che indica il successo dell'operazione di registrazione.
 * @return -1: se l'array di struct concernente gli utenti è nullo.
 * 
 */
int aggiungiUtente(utenti* utenti) {

	int error, error2;
    utente temp;
	int scelta = 0;

	if (utenti->array == NULL)
		return -1;

    system("cls");
	setColor(VERDE);
    printf("\
			\t\n***********************************************\
			\t\n*               REGISTRA UTENTE               *\
			\t\n***********************************************\n");
	setColor(DEFAULT);
	
	/**************
	* Nickname
	**************/
	error = error2 = 0;
	do {

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

	if (esisteUtente(utenti, temp.nickname))
		return -1;

	fflush(stdin);

	/**************
	* EMAIL
	**************/
	error = error2 = 0;

	do {

		inizializzaStringa(temp.email, 64);

		if (error) {
			fflush(stdin);
			setColor(ROSSO);
			printf("[!] Puoi inserire solo lettere, numeri! Formato: xxxx@xxxx.xxx\n");
			setColor(DEFAULT);
			error = 0;
		}

		if (error2) {
			fflush(stdin);
			setColor(ROSSO);
			printf("[!] L'e-mail deve avere minimo 12 caratteri e massimo 63.\n");
			setColor(DEFAULT);
			error2 = 0;
		}

		printf("Inserisci e-mail: ");
		scanf("%[^\n]", &temp.email);

		if (!strchr(temp.email, '@'))
			error = 1;

		if (!strchr(temp.email, '.'))
			error = 1;

		if (stringaValida(temp.email, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789.@") == 1)
			error = 1;

		if(strlen(temp.email) < 12 || strlen(temp.email) > 63)
			error2 = 1;

	} while(error != 0 || error2 != 0);

	fflush(stdin);

	/**************
	* Cognome
	**************/
	do {

		inizializzaStringa(temp.cognome, 20);

		if (error) {
			fflush(stdin);
			setColor(ROSSO);
			printf("[!] Puoi inserire solo lettere e spazi.\n");
			setColor(DEFAULT);
			error = 0;
		}

		if (error2) {
			fflush(stdin);
			setColor(ROSSO);
			printf("[!] Il cognome deve avere minimo 2 caratteri e massimo 15.\n");
			setColor(DEFAULT);
			error2 = 0;
		}

		printf("Inserisci cognome: ");			
		scanf("%[^\n]", &temp.cognome);

		if (stringaValida(temp.cognome, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz ") == 1)
			error = 1;

		if (strlen(temp.cognome) < 2 || strlen(temp.cognome) > 15)
			error2 = 1;

	} while(error != 0 || error2 != 0);

	fflush(stdin);

	/**************
	* Nome
	**************/
	do {

		inizializzaStringa(temp.nome, 20);

		if (error) {
			fflush(stdin);
			setColor(ROSSO);
			printf("[!] Puoi inserire solo lettere e spazi.\n");
			setColor(DEFAULT);
			error = 0;
		}

		if (error2) {
			fflush(stdin);
			setColor(ROSSO);
			printf("[!] Il nome deve avere minimo 4 caratteri e massimo 15.\n");
			setColor(DEFAULT);
			error2 = 0;
		}

		printf("Inserisci nome: ");			
		scanf("%[^\n]", &temp.nome);

		if (stringaValida(temp.nome, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz ") == 1)
			error = 1;

		if (strlen(temp.nome) < 4 || strlen(temp.nome) > 15)
			error2 = 1;

	} while(error != 0 || error2 != 0);

	fflush(stdin);

	/**************
	* Genere
	**************/
	printf("%16s : %d\n", "Maschio", 0);
	printf("%16s : %d\n", "Femmina", 1);
	printf("%16s : %d\n", "Non Specificato", 2);
	do {
		fflush(stdin);
		printf("Inserisci genere: ");			
		error = scanf("%d", &temp.genere);
		if (error != 1 || temp.genere < 0 || temp.genere > 2){
				fflush(stdin);
				setColor(ROSSO);
				printf("[!] Puoi inserire solo un numero tra quelli indicati.\n");
				setColor(DEFAULT);
				error=0;
		}
	} while(error != 1 || temp.genere < 0 || temp.genere > 2);

	fflush(stdin);

	/**************
	* Data di nascita
	**************/
	do {	
		printf("Inserire Data di Nascita? (0 : no | 1 : si): ");		
		error = scanf("%d", &scelta);
		if (error != 1 || scelta < 0 || scelta > 1) {
				fflush(stdin);
				setColor(ROSSO);
				printf("[!] Puoi inserire solo un numero tra quelli indicati.\n");
				setColor(DEFAULT);
				error = 0;
		}
	} while(error != 1 || scelta < 0 || scelta > 1);

	if (scelta) {

		int ritorno;

		do {
			fflush(stdin);
			ritorno = inserisciData(&temp.dataNascita);
		} while(ritorno == -1);

	} else {
		temp.dataNascita.giorno = 0;
		temp.dataNascita.mese = 0;
		temp.dataNascita.anno = 0;
	}
	
	fflush(stdin);

	/**************
	* Telefono
	**************/
	error = 0;
	inizializzaStringa(temp.telefono, 20);
	do {
		if (error) {
			fflush(stdin);
			setColor(ROSSO);
			printf("[!] Puoi inserire solo numeri!\n");
			setColor(DEFAULT);
			error = 0;
		}
		if (error2) {
			fflush(stdin);
			setColor(ROSSO);
			printf("[!] Il numero di telefono deve avere minimo 10 caratteri e massimo 19.\n");
			setColor(DEFAULT);
			error2 = 0;
		}

		printf("Inserisci Telefono: ");
		scanf("%[^\n]", &temp.telefono);

		if (stringaValida(temp.telefono, "0123456789\n") == 1)
			error = 1;
		
		if (strlen(temp.telefono) < 10 || strlen(temp.telefono) > 19)
			error2 = 1;

	} while(error != 0 || error2 != 0);
	fflush(stdin);
	/**************	
	* Genere Musicale
	**************/	
	printf("%12s : %d\n", "Blues", 0);
	printf("%12s : %d\n", "Classica", 1);
	printf("%12s : %d\n", "Heavy Metal", 2);
	printf("%12s : %d\n", "Hip Hop", 3);
	printf("%12s : %d\n", "Jazz", 4);
	printf("%12s : %d\n", "Pop", 5);
	printf("%12s : %d\n", "Punk", 6);
	printf("%12s : %d\n", "Rock", 7);
	printf("%12s : %d\n\n", "Altro", 8);

	do {
		printf("Inserisci Genere Musicale: ");
		error=scanf("%d", &temp.genereMusicale);
		if(error != 1 || temp.genereMusicale < 0 || temp.genereMusicale > 8 ) {
			fflush(stdin);
			setColor(ROSSO);
			printf("[!] L'opzione indicata non e' valida.\n");
			setColor(DEFAULT);
		}
	} while(error != 1 || temp.genereMusicale < 0 || temp.genereMusicale > 8);
	fflush(stdin);

	utenti->array = (utente*) realloc(utenti->array, sizeof(utente) * ++utenti->size);
	memcpy(&utenti->array[utenti->size - 1], &temp, sizeof(temp));

    return 1;
}


/**
 * La funzione visualizza i dati relativi a un utente, cercato per nickname, chaimando l'apposita funzione di ricerca.
 * 
 * @param [in] utenti: struct contentente l'indirizzo all'array di struct (utilizzato per effettuare operazioni concernenti gli utenti) e la gradezza della stessa.
 * 
 */
void visualizzaUtente(utenti* utenti , utente* entity) {

	int error = 0, error2 = 0;

	char nickname[16];
	utente* user = NULL;

	if (entity == NULL) {

		system("cls");
		setColor(VERDE);
    	printf("\
			\t\n***********************************************\
			\t\n*               VISUALIZZA UTENTE             *\
			\t\n***********************************************\n");
		setColor(DEFAULT);
	
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
			if (strlen(nickname)>15 || strlen(nickname)<4)
				error2 = 1;

		} while(error != 0 || error2 != 0);
		fflush(stdin);

		user = ricercaUtente(utenti, nickname);
	} else {
		user = entity;
	}
	
	if (esisteUtente(utenti, user->nickname)) {

		setColor(GIALLO);
		printf("\n\n%-20s : %-30s\n", "Nickname", user->nickname);
		printf("%-20s : %-30s\n", "E-Mail", user->email);
		printf("%-20s : %-30s\n", "Cognome", user->cognome);
		printf("%-20s : %-30s\n", "Nome", user->nome);
		printf("%-20s : %-30s\n", "Genere", user->genere == 0 ? "Maschio" : (user->genere == 1 ? "Femmina" : "Non Specificato"));
		if (user->dataNascita.anno != 0)
			printf("%-20s : %02d/%02d/%d\n", "Data Nascita", user->dataNascita.giorno, user->dataNascita.mese, user->dataNascita.anno);
		else
			printf("%-20s : %s\n", "Data Nascita", "Non registrata");
		printf("%-20s : %-30s\n", "Telefono", user->telefono);
		printf("%-20s : %-30s\n", "Genere Musicale", user->genereMusicale == 0 ? "Blues" : 
				(user->genereMusicale == 1 ? "Classica" : 
				(user->genereMusicale == 2 ? "Heavy metal" : 
				(user->genereMusicale == 3 ? "Hip pop" : 
				(user->genereMusicale == 4 ? "Jazz" : 
				(user->genereMusicale == 5 ? "Pop" : 
				(user->genereMusicale == 6 ? "Punk" : 
				(user->genereMusicale == 7 ? "Rock" : "Altro") ) ) ) ) ) ) );
		setColor(DEFAULT);
	} else {
		setColor(GIALLO);
		printf("L'utente '%s' non esiste!\n", nickname);
		setColor(DEFAULT);
	}
	
}

/** Il programma visualizza con il medesimo modulo di registrazione, l'interfaccia utenti, mostrando inoltre l'attuale valore che possiede ogni campo. 
 * è possibile modificarne il contenuto (tranne del nickname) oppure inviare il carattere di End-Of-File se si vuol lasciare inalterato il campo
 * 
 * @param [in] locali: struct contentente l'indirizzo all'array di struct (utilizzato per effettuare operazioni concernenti ai locali) e la gradezza della stessa.
 * 
 * @return 0: il locale cercato non è stato trovato
 * @return 1: valore che indica il successo dell'operazione di modifica
 * @return -1: se l'array di struct concernente gli utenti è nullo.
 */
int modificaUtente(utenti* utenti) {

	if (utenti->array == NULL)
		return -1;

	int error = 0, error2 = 0;
	char nickname[16];
	system("cls");
	setColor(VERDE);
    printf("\
			\t\n***********************************************\
			\t\n*               MODIFICA UTENTE               *\
			\t\n***********************************************\n");
	setColor(DEFAULT);

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
		if (strlen(nickname)>15 || strlen(nickname)<4)
			error2 = 1;

	} while(error != 0 || error2 != 0);
	fflush(stdin);

	if (esisteUtente(utenti, nickname) == 1) {

		utente* user = ricercaUtente(utenti, nickname);
		utente temp;

		strcpy(temp.nickname, user->nickname);

		setColor(ROSSO);
		puts("\n[!] Premere CTRL + Z se si vuol lasciare inalterato il valore!\n");

		setColor(GIALLO);

		/**************
		 * EMAIL
		 **************/
		do {
			inizializzaStringa(temp.email, 64);
			if (error) {
				fflush(stdin);
				setColor(ROSSO);
				printf("[!] Puoi inserire solo lettere, numeri! Formato: xxxx@xxxx.xxx\n");
				setColor(GIALLO);
				error = 0;
			}
			if (error2) {
				fflush(stdin);
				setColor(ROSSO);
				printf("[!] L'e-mail deve avere minimo 12 caratteri e massimo 63.\n");
				setColor(GIALLO);
				error2 = 0;
			}

			printf("[Attuale: %s] Modifica e-mail: ", user->email);
			scanf("%[^\n]", &temp.email);

			if (feof(stdin)) {
				strcpy(temp.email, user->email);
				fflush(stdin);
			}

			if (!strchr(temp.email, '@'))
				error = 1;

			if (!strchr(temp.email, '.'))
				error = 1;

			if (stringaValida(temp.email, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789.@") == 1)
				error = 1;
			
			if(strlen(temp.email) < 12 || strlen(temp.email) > 63)
				error2 = 1;

		} while(error != 0 || error2 != 0);
		fflush(stdin);

		/**************
		 * Cognome
		 **************/
		do {
			inizializzaStringa(temp.cognome, 20);
			if (error) {
				fflush(stdin);
				setColor(ROSSO);
				printf("[!] Puoi inserire solo lettere e spazi!\n");
				setColor(GIALLO);
				error = 0;
			}
			if (error2) {
				fflush(stdin);
				setColor(ROSSO);
				printf("[!] Il cognome deve avere minimo 2 caratteri e massimo 15.\n");
				setColor(GIALLO);
				error2 = 0;
			}

			printf("[Attuale: %s] Modifica cognome: ", user->cognome);			
			scanf("%[^\n]", &temp.cognome);

			if (feof(stdin)) {
				strcpy(temp.cognome, user->cognome);
				fflush(stdin);
			}
			if (stringaValida(temp.cognome, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz ") == 1)
				error = 1;
			if (strlen(temp.cognome) < 2 || strlen(temp.cognome) > 15)
				error2 = 1;

		} while (error != 0 || error2 != 0);
		fflush(stdin);
		/**************
		 * Nome
		 **************/

		do {
			inizializzaStringa(temp.nome, 20);
			if (error) {
				fflush(stdin);
				setColor(ROSSO);
				printf("[!] Puoi inserire solo lettere e spazi!\n");
				setColor(GIALLO);
				error = 0;
			}
			if (error2) {
				fflush(stdin);
				setColor(ROSSO);
				printf("[!] Il nome deve avere minimo 4 caratteri e massimo 15.\n");
				setColor(GIALLO);
				error2 = 0;
			}

			printf("[Attuale: %s] Modifica nome: ", user->nome);			
			scanf("%[^\n]", &temp.nome);
			if (feof(stdin)) {
				strcpy(temp.nome, user->nome);
				fflush(stdin);
			}

			if (stringaValida(temp.nome, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz ") == 1)
				error = 1;
			if (strlen(temp.nome) < 4 || strlen(temp.nome) > 15)
				error2 = 1;
		} while(error != 0 || error2 != 0);
		fflush(stdin);
		/**************
		 * Genere
		 **************/
		setColor(DEFAULT);
		printf("\nOpzioni Disponibili:");
		printf("\n%-16s : %d\n", "Maschio", 0);
		printf("%-16s : %d\n", "Femmina", 1);
		printf("%-16s : %d\n\n", "Non Specificato", 2);
		setColor(GIALLO);

		do {
			fflush(stdin);
			printf("[Attuale: %d] Modifica genere: ", user->genere);		
			error= scanf("%d", &temp.genere);
			if (feof(stdin)) {
				temp.genere = user->genere;
				fflush(stdin);
				break;
			}

			if (error != 1 || temp.genere < 0 || temp.genere > 2){
				fflush(stdin);
				setColor(ROSSO);
				printf("[!] Puoi inserire solo un numero tra quelli indicati.\n");
				setColor(GIALLO);
				error=0;
			}
			

		} while(temp.genere < 0 || temp.genere > 2 || error != 1);
		fflush(stdin);
		error=0;

		/**************
		 * Data
		 **************/
		int scelta = -1;
		do {
			if(user->dataNascita.giorno==0)
				printf("[Data di Nascita Attuale: Non registrata]\n");
			else
				printf("[Data di Nascita Attuale: %02d/%02d/%02d]\n", user->dataNascita.giorno, user->dataNascita.mese, user->dataNascita.anno);
			printf("Modificare Data di Nascita? (0 : no | 1 : si): ");		
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
				ritorno = inserisciData(&temp.dataNascita);
			} while(ritorno == -1);
			
		} else {
			temp.dataNascita.giorno = user->dataNascita.giorno;
			temp.dataNascita.mese = user->dataNascita.mese;
			temp.dataNascita.anno = user->dataNascita.anno;
		}
		fflush(stdin);
		
		/**************
		 * Numero
		 **************/

		do {
			inizializzaStringa(temp.telefono, 20);
			if (error) {
				fflush(stdin);
				setColor(ROSSO);
				printf("[!] Puoi inserire solo numeri e prefissi!\n");
				setColor(GIALLO);
				error = 0;
			}
			if (error2) {
				fflush(stdin);
				setColor(ROSSO);
				printf("[!] Il numero di telefono deve avere minimo 10 caratteri e massimo 19.\n");
				setColor(GIALLO);
				error2 = 0;
			}
			printf("[Attuale: %s] Modifica numero di telefono: ", user->telefono);
			scanf("%[^\n]", &temp.telefono);
			if (feof(stdin)) {
				strcpy(temp.telefono, user->telefono);
				fflush(stdin);
			}

			if (stringaValida(temp.telefono, "0123456789") == 1)
				error = 1;
			if (strlen(temp.telefono) < 10 || strlen(temp.telefono) > 19)
				error2 = 1;

		} while(error != 0 || !feof(stdin) || error2 != 0);
		fflush(stdin);

		/**************
		 * Genere Musicale
		 **************/
		setColor(DEFAULT);
		printf("\nOpzioni Disponibili:\n");
		printf("%-12s : %d\n", "Blues", 0);
		printf("%-12s : %d\n", "Classica", 1);
		printf("%-12s : %d\n", "Heavy Metal", 2);
		printf("%-12s : %d\n", "Hip Hop", 3);
		printf("%-12s : %d\n", "Jazz", 4);
		printf("%-12s : %d\n", "Pop", 5);
		printf("%-12s : %d\n", "Punk", 6);
		printf("%-12s : %d\n", "Rock", 7);
		printf("%-12s : %d\n\n", "Altro", 8);
		setColor(GIALLO);

		do {
			printf("[Attuale: %d] Modifica Genere Musicale: ", user->genereMusicale);		
			error=scanf("%d", &temp.genereMusicale);
			if (feof(stdin)) {
				temp.genereMusicale = user->genereMusicale;
				fflush(stdin);
				break;
			}
			if(error != 1 || temp.genereMusicale < 0 || temp.genereMusicale > 8 ) {
				fflush(stdin);
				setColor(ROSSO);
				printf("[!] L'opzione indicata non e' valida.\n");
				setColor(GIALLO);
			}
		} while(temp.genereMusicale < 0 || temp.genereMusicale > 8 || error != 1);	
		memcpy(user, &temp, sizeof(temp));

	} else	
		return 0;

	fflush(stdin);
	return 1;
}

/**
 * La funzione ricerca nel vettore dinamico il nickname dato in input, se ne trova una corrispondenza ne ritorna il puntatore.
 * 
 * @param [in] utenti : struct contentente l'indirizzo all'array di struct (utilizzato per effettuare operazioni concernenti gli utenti) e la gradezza della stessa.
 * @param [in] nickname : array di caratteri contenente il nickname.
 * 
 * @return NULL : Il vettore è nullo oppure non è stata trovata alcuna corrispondenza.
 * @return *utente : Viene trovato l'utente e restituito il puntatore nell'array dinamico.
 * 
 */
utente* ricercaUtente(utenti* utenti, char* nickname) {

	if (utenti->array == NULL) {
		setColor(GIALLO);
		printf("[!] Il vettore degli Utenti è nullo!\n");
		setColor(DEFAULT);
		return NULL;
	}

    for (int i=0; i<utenti->size; i++) {
        if (strcmp(myLower(utenti->array[i].nickname), myLower(nickname)) == 0) {
            return &utenti->array[i];
        }
    }

	return NULL;
}

/**
 * La funzione verifica se già esiste un utente con il nickname indicato, richiamando la funzione ricercaUtente.
 * 
 * @param [in] utenti : struct contentente l'indirizzo all'array di struct (utilizzato per effettuare operazioni concernenti gli utenti) e la gradezza della stessa.
 * @param [in] nickname : array di caratteri contenente il nickname.
 * 
 * @return -1 : se il puntatre all'array delle struct utente è nullo.
 * @return 1 : se l'utente è stato trovato.
 * @return 0 : se l'utente non è stato trovato.
 * 
 */
int esisteUtente(utenti* utenti, char* nickname) {

	if (utenti->array == NULL)
		return -1;

	if (ricercaUtente(utenti, nickname))
		return 1;

	return 0;
}

/**
 * La funzione lavora insieme alla funzione mergeSort, si occupa dell'ordinamento vero e proprio dell'array, confrontando gli elementi delle varie partizioni create.
 * 
 * @param [in] utenti : struct contentente l'indirizzo all'array di struct (utilizzato per effettuare operazioni concernenti gli utenti) e la gradezza della stessa.
 * @param [in] inizio : intero che indica l'indice dell'inizio dell'array.
 * @param [in] meta : intero che indica l'indice della metà dell'array.
 * @param [in] fine : intero che indica l'indice della fine dell'array.
 * 
 */
void mergeUtenti(utenti* utenti, int inizio, int meta, int fine) {
	
	int i = inizio;
	int j = meta + 1;
	int k = 0;
	utente* temp = calloc(utenti->size, sizeof(utente));

	while (i<=meta && j<=fine){
		if(strcmp(utenti->array[i].nickname, utenti->array[j].nickname) < 0){
			memcpy(&temp[k], &utenti->array[i], sizeof(utente));
			i++;
		} else {
			memcpy(&temp[k], &utenti->array[j], sizeof(utente));
			j++;
		}
		k++;
	}
	
	while (i<=meta){
		memcpy(&temp[k], &utenti->array[i], sizeof(utente));
		i++;
		k++;
	}
	
	while (j<=fine){
		memcpy(&temp[k], &utenti->array[j], sizeof(utente));
		j++;
		k++;
	}
	
	for(k=inizio; k<=fine; k++) {
		memcpy(&utenti->array[k], &temp[k-inizio], sizeof(utente));
	}

	free(temp);
}

/**
 * La funzione visualizza i dati di tutti gli utenti, secondo l'ordine alfabetico dei relativi nickname.
 * 
 * @param [in] utenti : struct contentente l'indirizzo all'array di struct (utilizzato per effettuare operazioni concernenti gli utenti) e la gradezza della stessa.
 * 
 */
void visualizzaUtenti(utenti* utenti) {

	mergeSort(utenti, _utenti, 0, utenti->size - 1);

	for (int i=0; i<utenti->size; i++){
		printf("%-20s %s \n %-20s %s \n %-20s %s \n %-20s %s \n %-20s %2d \n %-20s %d/%d/%d \n %-20s %s \n %-20s %2d\n\n\n\n", 
                "Nickname:", utenti->array[i].nickname, 
                "E-mail:", utenti->array[i].email, 
                "Cognome:", utenti->array[i].cognome,
				"Nome:", utenti->array[i].nome,
				"Genere:",  utenti->array[i].genere,
				"Data di nascita:",  utenti->array[i].dataNascita.giorno, utenti->array[i].dataNascita.mese, utenti->array[i].dataNascita.anno,
				"Numero di telefono:", utenti->array[i].telefono,
				"Genere musicale:", utenti->array[i].genereMusicale);
	}

	caricaUtenti(utenti);

}