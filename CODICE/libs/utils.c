#include "utils.h"

#include "utenti.h"
#include "locali.h"
#include "eventi.h"
#include "prenotazioni.h"
#define ANNO_MIN 1920
#define ANNO_MAX 2200

/**
 * La funzione carica in memoria i dati realtivi al file indexs.dat che contengono gli indici AUTO_INCREMENT per Eventi, Prenotazioni e Locali
 * 
 * @param [in] indexs : puntatore a struct index
 * 
 */
void caricaIndexs(struct index* indexs) {

    FILE* fp;

    if ((fp = fopen("./files/indexs.dat", "r")) != NULL) {
        char buffer[100];
        fgets(buffer, sizeof(buffer), fp);

        char* index;

        char* line = buffer;
        char** input = &line;

        index = myParser(input, "|");
		indexs->evento = atoi(index);

        index = myParser(input, "|");
		indexs->locale = atoi(index);

        index = myParser(input, "|");
		indexs->prenotazione = atoi(index);

        fclose(fp);
    } else {
        indexs->evento = 1;
        indexs->locale= 1;
        indexs->prenotazione = 1;
        salvaIndexs(indexs);
    }  
}


/**
 * La funzione salva su file i dati realtivi alla struct index che contengono gli indici AUTO_INCREMENT per Eventi, Prenotazioni e Locali
 * 
 * @param [in] indexs : puntatore a struct index
 * 
 */
void salvaIndexs(struct index* indexs) {

    FILE* fp;

    if ((fp = fopen("./files/indexs.dat", "w")) != NULL) {
        fprintf(fp, "%d|%d|%d", indexs->evento, indexs->locale, indexs->prenotazione);
        fclose(fp);
    }
}


/**
 * La funzione gestisce un file già aperto leggendo riga per riga il contenuto ed incrementando una variabile accumulatrice per ogni riga.
 * Infine viene resettato il puntatore ad inizio file per poter essere nuovamente utilizzato.
 * 
 * @param [in] filePointer : puntatore a file già aperto
 * 
 * @return lines : numero di righe presenti nel file
 * 
 */
int contaLinee(FILE* filePointer) {

    if (filePointer == NULL) {
        return -1;
    }

    fseek(filePointer, 0, SEEK_SET);

    char ch;
    unsigned int lines = 0;
    while(!feof(filePointer)) {
        ch = fgetc(filePointer);
        if(ch == '\n') {
            lines++;
        }
    }
    // Si reimposta il puntatore all'inizio perchè il file è stato letto completamente
    fseek(filePointer, 0, SEEK_SET);
    return lines;
} 



/**
 * La funzione effettua un controllo di validità su un array di caratteri per verificare che quest'ultimi rientrino nell'alfabeto imposto. 
 * 
 * @param [in] string : vettore di caratteri sottoposto a un controllo di validità in base all'alfabeto imposto.
 * @param [in] alphabet : vettore di caratteri che contiene l'alfabeto consentito per un dato. 
 * 
 * @return 1: se un carattere di string non rientra nell'alfabeto imposto.
 * @return 0: se tutti i caratteri di string rientrano nell'alfabeto imposto.
 */
int stringaValida(const char string[], const char* alphabet) {

    unsigned int size = strlen(string);

    for (int i=0; i<size; i++)
        if(!strchr(alphabet, string[i]))
            return 1;

    return 0;
}

/**
 * Funzione custom di strtok in quanto, l'attuale strktok non consente di leggere un campo vuoto (presenza di doppio delimiter: ;;)
 * Viene passato il puntatore del puntatore alla line da parserizzare, successivamente viene trovato ogni singolo carattere di delimitazione e viene estrapolato
 * il campo eliminandolo dalla linea che successivamente viene restituito
 * 
 * @param [in] newString : stringa da parserizzare che otterrà il valore della stringa meno il campo analizzato
 * @param [in] delimiter : carattere che rappresenta il delimitatore del file csv
 * 
 * @return token: campo estratto dalla stringa
 */
char* myParser(char** newString, char* delimiter) {

    char* string = *newString;
    char* delimiterFound = NULL;
    char* token = NULL;
    int tokLenght = 0;

    if (!string) 
        return NULL;

    delimiterFound = strstr(string, delimiter);

    if(delimiterFound) {
        tokLenght = delimiterFound - string;
    } else {
        tokLenght = strlen(string);
    }

    token = malloc(tokLenght + 1);
    memcpy(token, string, tokLenght);
    token[tokLenght] = '\0';

    *newString = delimiterFound ? delimiterFound + strlen(delimiter) : NULL;

    return token;
}



/**
 * La funzione trasforma tutte le lettere maiuscole, di un array di caratteri, in lettere minuscole.
 * 
 * @param [in]  string : array di caratteri.
 * 
 * @return newString : nuovo array di caratteri con solo lettere minuscole.
 * 
 */
char* myLower(const char string[]) {
    
    unsigned int size = strlen(string);
    char* newString = calloc(size+1, sizeof(char));

    for(int i=0; i < size; i++)
        newString[i] = tolower(string[i]);

    return newString;
}



/**
 * La funzione cambia il colore del testo in base al colore ricevuto come parametro.
 * 
 * @param [in] colore: enum che indica i vari colori.
 * 
 */
void setColor(enum colori colore) {
    switch (colore) {
        case ROSSO:
            printf("\033[91m");
            break;
        case GIALLO:
            printf("\033[93m");
            break;
        case VERDE:
            printf("\033[0;32m");
            break;
        case CELESTE:
            printf("\033[94m");
            break;
        case VIOLA:
            printf("\033[95m");
            break;
        default:
            printf("\033[0m");
            break;
    }
}


/**
 * La funzione inizializza con il carattere '\0' l'array di caratteri ricevuto come parametro.
 * 
 * @param [in] strigna : array di caratteri.
 * @param [in] size : grandezza dell'array.
 * 
 */
void inizializzaStringa(char stringa[], int size) {

   for(int i=0; i<size; i++) {
       stringa[i] = '\0';
   }

}


/**
 * La funzione conta le cifre del numero ricevuto come parametro. Utilizzata per verificare la validità dell'anno di una data.
 * 
 * @param [in] numero: numero intero.
 * 
 * @return i+1 : numero di cifre di cui è composto il numero. (+1 perchè se il numero è a una sola cifra sarà minore di 10 quindi i varrà 0).
 */
int contaCifre(int numero) {
	
	int i = 0;
	
	while(numero > 10) {
		numero /= 10;
		i++;
	}

	return i+1;
		
}


/**
 * La funzione permette la registrazione di una data, su cui effettua un controllo di validità.
 * 
 * @param [in] memory: struct di tipo data.
 * 
 * @return 0: se la data è corretta.
 * @return -1: se la data non è corretta.
 */
int inserisciData(data* memory) {

    data temp;
    int error=0;
    int lunghezza;

    do {
        printf("\tInserisci giorno: ");			
		error = scanf("%d", &temp.giorno);
        if (error!=1 || temp.giorno < 1 || temp.giorno > 31) {
            fflush(stdin);
            setColor(ROSSO);
            printf("\n\t[!] Il giorno indicato non e' valido.\n");
            setColor(DEFAULT);
        }
    } while(error!=1 || temp.giorno < 1 || temp.giorno > 31);
    
    error=0;
    fflush(stdin);
    
    do {
        printf("\tInserisci mese: ");			
		error = scanf("%d", &temp.mese); 
        if(error!=1 || temp.mese < 1 || temp.mese > 12) {
            fflush(stdin);
            setColor(ROSSO);
            printf("\n\t[!] Il mese indicato non e' valido.\n");
            setColor(DEFAULT);
        }
    } while (error !=1 || temp.mese < 1 || temp.mese > 12);

    switch(temp.mese) {			
		case 4:
		case 6:
		case 9:
		case 11:
			if ( temp.giorno > 30 ) {
                setColor(ROSSO);
                printf("\n\t[!] Il giorno indicato non e' compreso nel mese scelto.\n");
                setColor(DEFAULT);
                return -1;
            }
			break;
		case 2:
			if ( temp.giorno > 28) {
                setColor(ROSSO);
                printf("\n\t[!] Il giorno indicato non e' compreso nel mese scelto.\n");
                setColor(DEFAULT);
                return -1;
            }
			break;
		default:
			break;
    }

    error=0;
    fflush(stdin);
    
    do {
        printf("\tInserisci anno: ");			
		error = scanf("%d", &temp.anno); 
        lunghezza=contaCifre(temp.anno);

        if (error != 1 || temp.anno < ANNO_MIN || temp.anno > ANNO_MAX || lunghezza != 4) {
            fflush(stdin);
            setColor(ROSSO);
            printf("\n\t[!] L'anno indicato non e' valido.\n");
            setColor(DEFAULT);         
        } 
    } while(error != 1 || temp.anno < ANNO_MIN || temp.anno > ANNO_MAX || lunghezza != 4);

    memcpy(memory, &temp, sizeof(data));
        
    return 0;
}


/**
 * 
 * La funzione suddivide, ricorsivamente, l'array in più partizioni e chiama la funzione mergeXXXX dove le X cambiano a seconda del tipo di merge.
 * 
 * @param [in] memory : puntatore all'array di struct che si vuole ordinare.
 * @param [in] TypeMerge : enum che indica la struct che si vuole trattare (utente, evento, locale, ecc.).
 * @param [in] inizio :  intero che indica l'indice dell'inizio dell'array.
 * @param [in] fine :  intero che indica l'indice della fine dell'array.
 * 
 */
void mergeSort(void* memory, enum TypeMerge type, int inizio, int fine) {

	int meta;

	if(inizio < fine) {
		meta = (inizio+fine) / 2;
		mergeSort(memory, type, inizio, meta);
		mergeSort(memory, type, meta+1, fine);
		switch (type) {
			case _utenti:
				mergeUtenti(memory, inizio, meta, fine);
				break;
            case _locali:
                mergeLocali(memory, inizio, meta, fine);
                break;
            case _eventi:
                break;
            case _prenotazioni:
                break;
            default:
                break;
		}	
	}	
}


/**
 * La funzione calcola la differenza tra due orari, ovvero struct con i campi ora e minuto.
 * 
 * @param [in] inizio : primo orario.
 * @param [in] fine : secondo orario.
 * 
 * @return differenza : differenza tra due orari sottoforma di numero in virgola mobile.
 * 
 */
float differenzaOrari (orario inizio, orario fine) {
    float temp[2] = {0};
    float differenza = 0;

    if(inizio.ora < fine.ora){
        temp[0]=fine.ora-inizio.ora;

        if(inizio.minuto <= fine.minuto){
            temp[1]=fine.minuto-inizio.minuto;
        } else {
            temp[1]=(60-inizio.minuto)+fine.minuto;
            temp[0]--;
        }


    } else {
        temp[0]=fine.ora+(24-inizio.ora);

        if(inizio.minuto <= fine.minuto){
            temp[1]=fine.minuto-inizio.minuto;
        } else {
            temp[1]=(60-inizio.minuto)+fine.minuto;
            temp[0]--;
        }
    }
    
    if(temp[1]==60)
        temp[1]=0;

    differenza = temp[0]+(temp[1]/100);

    return differenza;

}