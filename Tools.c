/*
 * Tools.c
 *
 *
 *      Author: FRANCESCO SOPRANO and MICHELE PERILLI
 */


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#include "AccountHead.h"
#include "Tools.h"




/*
 * FUNZIONI MATEMATICHE
 */

/* Inizializzo il divisore e la dimensione, poi iniziero' con divisioni successive fin quando la divisione intera tra il parametro e il divisore e' diverso da zero.
 * A ogni passo del ciclo aumento di uno la dimensione, e aggiungo uno zero al divisore, moltiblicandolo per 10. Il caso limite in cui il parametro sia zero, comporta
 * direttamente il settare la dimensione a 1.
 *
 * Esempio:
 * contaCifre( 53 ) == 2
 */

int contaCifre( int n )
{

	int divisore = 1, dimensione = 0;

	while ( n / divisore != 0) {
		// Effettuo divisioni successive finche' la divisione intera non da' zero

		dimensione++;
		divisore *= 10;

	}

	if ( n == 0 ) dimensione = 1; // Caso limite, settaggio dimensione a 1.

	return dimensione; // Ritorno la quantita' di cifre del parametro.
}



/* Calcola la media dei voti, avendo la loro somma e la loro quantita'. Il singolo voto puo' andare da 1 a 5, quindi
 * dovrebbe essere impossibile avere quantita' negativa o uguale a 0, per quei casi limite restituisce -1, invece nei casi giusti restituisce la media.
 */

float mediaVoti ( float somma, float quantita )
{
	float media = 0; //azzero la media

	if ( quantita > 0  ){ //se la quantità è maggiore di 0 la media è somma/quantita

		media = somma/quantita;

	} else {

		if ( somma == 0 ) media = 0;// se la somma dei voti e la quantita sono 0 la media è 0
		else media = -1;//altrimenti se la quantità è 0 e la somma diversa da 0 ritorno -1 ( valore negativo, quindi fuori dal range della media che deve essere sempre positiva )

	}

	return media;//ritorno la media
}






/*
 * FUNZIONI PER LA MANIPOLAZIONE STRINGHE
 */

/* Mette il carattere vuoto in 'len' celle della stringa per evitare di avere dati indesiderati
 *
 * Esempio:
 *
 * char str[] = "Hello world";
 * clearString( str, strlen(str) );
 *
 * --> str = "".
 * */

void clearString( char vet[], int len )
{
  for (unsigned char j = 0; j < len; j++)
    vet[j] = '\0';
}



/* Rimuove tutti gli spazi presenti a inizio e a fine stringa data in input modificandola opportunamente.
 *
 * Esempio:
 *
 * char str[] = "  Ciao ";
 * vogliaSpaziDagliEstremi( str );
 *
 * --> str = "Ciao".
 */

void togliSpaziDagliEstremi( char stringa[] )
{
	int spaziDestra = 0;	// Azzero i contatori
	int spaziSinistra = 0;	// degli spazi


	for( int s = 0; s < strlen( stringa ); s++ ){
		// Scorro da sinistra a destra fin quando non trovo un carattere diverso dallo spazio, salvandomi quanti spazi ho trovato.

		if( stringa[s] != ' ' ) s = strlen( stringa );
		else spaziSinistra++;

	}

	for( int d = strlen( stringa ) - 1; d > 0; d-- ){
		// Scorro da destra a sinistra fin quando non trovo un carattere diverso dallo spazio, salvandomi quanti spazi ho trovato.

		if( stringa[d] != ' ' ) d = 0;
		else spaziDestra++;

	}


	for ( int i = 0; i < strlen( stringa ); i++ )
		// Scorro tutta la stringa ricopiando il primo carattere non spazio, nella prima cella e cosi via.
		stringa[i] = stringa[i + spaziSinistra];

	// Poi pulisco la stringa partendo dalla posizione data dalla lunghezza di quella di partenza, meno il numero di spazi agli estremi.
	clearString( stringa + ( strlen( stringa ) - spaziDestra ), spaziDestra );

}



/* Usando la stringa data come parametro, viene creata un'altra stringa della stessa dimensione e contenuto
 * per poi mettere asterichi dal dal secondo carattere fino al penultimo
 *
 * Esempio:
 *
 * char str[] = "password";
 * printf("%s", oscuraPassword( str ) );
 *
 * --> verra' stampato "p******d"
 */

const char* oscuraPassword( const char* password )
{
	// Alloco una stringa della stessa dimensione di quella come parametro
	char* pass_temp = ( char* ) calloc( strlen( password ), sizeof( char ) );

	// Copio il contenuto del parametro nel suo duplicato
	strcpy( pass_temp, password );

	for ( unsigned char i = 1; i < (strlen( password ) - 1 ); i++ ) // Metto asterichi dal dal secondo fino
		pass_temp[i]= '*';											// al penultimo carattere

	return pass_temp;
}






/*
 * FUNZIONI PER LA CONVERSIONE
 */



/* Alloco una stringa della stessa dimensione della stringa data come parametro, dopodiche' copio tutti i caratteri dalla stringa originale
 * nella stringa duplicato dopo averli resi minuscoli. Alla fine del processo restituisco la stringa 'abbassata'
 *
 * Esempio:
 *
 * char str[] = "Pippo";
 * printf("%s", toLower( str ) );
 *
 * --> verra' stampato "pippo"
 * */

const char* toLower( const char vet[] )
{
	char* stringa = ( char* ) calloc( strlen( vet ) + 1, sizeof( char ) ); // Allocazione della stringa della stessa lunghezza di quella data da parametro

    for (unsigned char j = 0; j < strlen( vet ) ; j++)	// Fino a che non mi trovo a fine file, copio il carattere della stringa originale
	    stringa[j] = tolower( vet[j] );				// e lo copio nella stessa posizione, ma nella nuova stringa dopo averlo reso minuscolo

    stringa[strlen( vet )] = '\0';

	return stringa;
}






/*
 * FUNZIONI PER L'INPUT
 */

/* Chiede un input di una stringa di lunghezza massima len, effettua quindi il controllo sulla lunghezza e se serve pulisce il buffer di stdin.
 * Restituisce la stringa che rispettera' ricuramente il vingolo di lunghezza massima dato come parametro poiche' se l'utente sfora la lunghezza,
 * richiedero' di nuovo l'input, cioe' continuera' a chiedere una stringa giusta fin quando non rispetta il vincolo del parametro*/

const char* inputStringa(int len, const char* testo)
{
	printf("%s", testo); // Inizia stampando il testo dato come parametro, quindi personalizzabile

	char* stringa = (char*) calloc(len + 2, sizeof(char));
	/* Alloco spazio in memoria di due caratteri in più della lunghezza massima consentita,
	 * poichè fgets mette direttamente alla fine della stringa il terminatore, questo comporta una piccola perdita
	 * di informazioni se aggiungo solo un carattere anzicchè due:
	 * nel caso peggiore, in questo caso, ottengo quindi, tramite la fgets, una stringa di massimo un carattere in più non terminatore
	 * alla lunghezza massima data dal parametro, su quel carattere farò il controllo se l'utente ha sforato o meno */

	do {

		clearString(stringa, len + 2);	// Pulisco la stringa
		fgets(stringa, len + 2, stdin);	// e procedo con l'input

		if ( stringa[len] != '\0' && stringa[len] != '\n' )	//
		{													// Controllo sull'ultimo carattere
			while ( getchar() != '\n');						// per capire se l'utente ha sforato
			printf("\t\t Input troppo lungo, max %d caratteri.\n%s", len, testo);							// il limite di lunghezza
		}													//

	} while ( stringa[len] != '\0' && stringa[len] != '\n' );
	/* Con questa condizione ho intenzione di controllare il carattere successivo a quello che dovrebbe essere il finale:
	 * se questo carattere non e' un carattere nullo o un ritorno carrello, l'utente ha sforato, quindi rientro nel ciclo*/


	for ( unsigned int i = 0; i < strlen(stringa); i++ ) // Scorro tutta la stringa prima di restituirla, alla ricerca
		if ( stringa[i] == '\n' ) stringa[i] = '\0';	 // di caratteri '\n' che costituirò con il carattere nullo

	togliSpaziDagliEstremi( stringa );

	return stringa;
}



/* Controlla se la stringa data come parametro e' un numero, se il flag e' settato su vero verra' considerato valido anche il punto */

bool areDigits ( const char vet[], bool isFloat )
{
	bool soloNumeri = true;

	int punti = 0;

	//scorro tutta la stringa
	for ( unsigned int i = 0; i < strlen( vet ); i++ ){

		//se è un float
		if ( isFloat ){

			//se ci sono punti incrementi la variabili punti
			if ( vet[i] ==  '.' ) punti++;

			//se non ci sono numeri o è presente più di un punto
			if( ( !isdigit( vet[i] ) && vet[i] !=  '.' ) || punti > 1 ){
				soloNumeri = false;//setto il flag con false
				i = strlen( vet );//esco dal ciclo
			}

		} else {//se invece è un intero

			//se non ci sono numeri
			if( !isdigit( vet[i] ) ){
				soloNumeri = false;//setto il flag con false
				i = strlen( vet );//esco dal ciclo
			}

		}
	}

	if ( vet[0] == '\0' ) soloNumeri = false;

	return soloNumeri;
}



/* Dichiaro una stringa di dimensione pari al numero di cifre del numero massimo che l'utente può inserire, per poi pulirla con una funzione esterna.
 * Chiedo un input una stringa: se quella stringa, convertita in intero, e' minore del vincolo minimo o maggiore del massimo,
 * ripetero' richiedendo una stringa in input, fin quando non si rispetti il criterio dato nel parametro. */

int inputIntero( int min, int max, const char* testo )
{
	int cifre = contaCifre( max ); // Accolgo in 'cifre' il numero di cifre del vincolo massimo

	char input[ cifre ];			// Dichiaro una stringa di quella dimesione
	clearString( input, cifre );	// e la inizializzo

	do{                                            								// Avendo il range da chiedere in input,
																				// prendo in input come una stringa e controllo
		strcpy( input , inputStringa( cifre, testo ) );							// se, una volta convertita in intero, sia compresa tra min e max
																				//
	} while( atoi( input ) < min || atoi( input ) > max || !areDigits( input, false ) );	// altrimenti ritorno a inizio do-While

	return atoi( input ); // Se invece è compresa tra min e max, la restituisco.
}



/* Dichiaro una stringa di dimensione pari al numero di cifre del numero massimo che l'utente può inserire, per poi pulirla con una funzione esterna.
 * Chiedo un input una stringa: se quella stringa, convertita in float, e' minore del vincolo minimo o maggiore del massimo,
 * ripetero' richiedendo una stringa in input, fin quando non si rispetti il criterio dato nel parametro. */

float inputFloat(float min, float max, const char* testo)
{
	int cifre = contaCifre( (int) max) + 3; // Accolgo in 'cifre' il numero di cifre del vincolo massimo

	char input[cifre];			// Dichiaro una stringa di quella dimesione aumentata di 3, cioè il punto + 2 cifre decimali
	clearString(input, cifre);	// e la inizializzo

	do{                                            								// Avendo il range da chiedere in input,
																				// prendo in input come una stringa e controllo
		strcpy(input , inputStringa(cifre, testo) );							// se, una volta convertita in intero, sia compresa tra min e max
																				//
	} while(atof(input) < min || atof(input) > max || !areDigits( input, true ) );		// altrimenti ritorno a inizio do-While

	return atof(input); // Se invece è compresa tra min e max, la restituisco.
}



/* Genera random un numero definito di lettere maiuscole, per poi chiederle in input. Può essere vista anche come il famoso:
 * " Dimostra di non essere un robot ". Una otp generata puo' essere ad esempio "OSPEW" */

int inputOTP()
{
	char* OTP = genOTP( DIMENSIONE_OTP );//genero una parola random

	printf("\n\t\t --> %s <--", OTP );

	if ( strcmp( inputStringa( DIMENSIONE_OTP, "\n\t OTP -> "), OTP ) == 0 ){
		//se l'utente inserisce la parola corretta

		return 1;	// Esito positivo

	} else return 0;// Esito negativo
}






/*
 * FUNZIONI PER IL CONTROLLO CORRETTEZZA STRINGHE
 */

/* Controllo la validita' della stringa data come parametro come email. Scorro tutti i caratteri della stringa tenendo traccia del numero di caratteri illegali,
 * delle @ e dei punti presenti dopo la @. Fatto cio':
 * ritorno 0 (giusto) se ci sono sia caratteri prima che dopo la chiocciola, sia prima che dopo il punto e se non ci sono caratteri illegali
 * ritorno 1 (errore) se non c'e' una sola @,
 * ritorno 2 (errore) se non ci sono punti dopo la @
 * ritorno 3 (errore) se non ci sono caratteri prima o dopo della @ o dopo il punto del dominio
 * ritorno 4 (errore) se ci sono caratteri non ammessi nella email, cioe' il punto-e-virgola e lo spazio
 */

int controllaEmail(const char* email)
{

	int caratteri_pre_chiocciola = 0, caratteri_post_punto = 0, caratteri_illegali = 0;
	int punto = 0, chiocciola = 0;


	/* Inizio scorrendo tutti i caratteri nella email, incrementando in modo opportuno i miei contatori
	 *  che utilizzerò*/
	for ( unsigned int i = 0; i < strlen(email); i++) {

		if( email[i] == ';' || email[i] == ' ') caratteri_illegali++; // Incrementro caratteri_illegali se trovo un punto-e-virgola o uno spazio

		if( email[i] == '@' ) chiocciola++; // Tengo traccia anche di quante @ ci sono

		if( email[i] == '.' ) {
			if (chiocciola > 0) punto++; // Incremento il contatore dei punti se abbiamo gia trovato la @
			else punto = 0;				 // Altrimenti lo resta 0.
		}
	}


	if ( chiocciola != 1 ) return 1;	// Ritorno 1 (errore) se non c'e' una sola @
	else if ( punto == 0 ) return 2;	// Ritorno 2 (errore) se non ci sono punti dopo la @( se ne necessita almeno uno per avere un dominio corretto )
	else {

		/* Conteggio di quanti caratteri precedono la chiocciola */
		for( unsigned int i = 0; email[i] != '@' && i < strlen(email); i++ ) caratteri_pre_chiocciola++;

		/* Conteggio di quanti caratteri ci sono dopo il*/
		for( unsigned int i = strlen(email) - 1; email[i] != '.' && i > 0; i--) caratteri_post_punto++;

		/* Passaggio puramente aritmetrico per calcolare i caratteri tra il punto del dominio e la chiocciola */
		int caratteri_pre_punto = strlen(email) - caratteri_pre_chiocciola - caratteri_post_punto - 2;


		/* Se ci sono sia caratteri prima che dopo la chiocciola, sia prima che dopo il punto e se non ci sono caratteri illegali */
		if ( caratteri_pre_chiocciola > 0 &&
				  caratteri_pre_punto > 0 &&
				 caratteri_post_punto > 0 && caratteri_illegali == 0 ) return 0;

		/* Se ci sono caratteri illegali */
		else if ( caratteri_illegali > 0 ) return 4;

		/* se non ci sono caratteri prima o dopo della @ o dopo il punto del dominio */
		else return 3;
	}
}



/* Effettua il test di correttezza password sulla stringa data in input, ritorna 0 se supera il test, altrimenti ritorna un non-zero in base al tipo di errore.
 * Una password e' considerata giusta se ha almeno una lettera minuscola, una maiuscola e un numero, ad esempio "Ciao!0"*/

int controllaPassword( const char* password )
{
	int maiuscole = 0, minuscole = 0, numeri = 0, speciali = 0;//azzero tutti i contatori

	int flagPuntoVirgola = 0;

	for ( unsigned int i = 0; password[i] != '\0'; i++ ){
		/* Scorro su tutta la stringa e incremento i contatori corrispondenti se ci sono numeri,
		 * maiuscole, minuscole, speciali e setto a 1 il flag se c'è un punto e virgola */

		if ( isdigit( password[i] ) ) numeri++;

		if ( isupper( password[i] ) ) maiuscole++;

		if ( islower( password[i] ) ) minuscole++;

		if ( !isalnum( password[i] ) && password[i] != ';' ) speciali++;

		if ( password[i] == ';' ) flagPuntoVirgola = 1;

	}

	/* In base ai test restituisce un valore relativo all'esito */
	if ( flagPuntoVirgola == 1 ) return 1;

	if ( speciali == 0 ) return 2;

	if ( numeri == 0) return 3;

	if ( maiuscole == 0 ) return 4;

	if ( minuscole == 0 ) return 5;

	return 0; //password corretta
}



/* Testo se la stringa come parametro possa essere usata come un nome o un cognome. Inizio scorrendo tutti i caratteri di questa stringa
 * alla ricerca di numeri o di caratteri speciali, incrementando appositi contatori.
 * ritorno 0 (giusto) se ci sono solo lettere
 * ritorno 1 (errore) se ci sono numeri, dopo aver avvisato l'utente
 * ritorno 2 (errore) se ci sono caratteri speciali ( sono esclusi spazio e apice ), dopo aver avvisato l'utente
 */

int testAnagrafica( char stringa[] )
{

	unsigned int len = strlen(stringa);
	unsigned int numeri = 0, speciali = 0;
	int flagErrore = 0;

	if ( len < 3 ){

		flagErrore = 3;

	} else {

		for( unsigned int i = 0; i < len; i++ ){
			/* Scorro tutti i caratteri alla ricerca di numeri o di caratteri speciali, incrementando appositi contatori */
			if ( isdigit( stringa[i] ) ) numeri++;
			if ( !isalnum( stringa[i] ) && stringa[i] != ' ' && stringa[i] != '\'' ) speciali++;
		}

		flagErrore = 0; // Flag in cui salvo il tipo di errore da restituire

		if ( numeri > 0) flagErrore = 1; // 1. Se ci sono numeri

		else if ( speciali > 0) flagErrore = 2; // 2. Se ci sono caratteri speciali escluso spazio e apice

		else flagErrore = 0; // 0. Se e' tutto ok
	}

	switch ( flagErrore ){ /* In base all'errore avviso l'utente*/
		case 1:
			puts("\n\t Non possono esserci dei numeri nel nome e/o nel cognome.");
			break;
		case 2:
			puts("\n\t Non possono esserci caratteri speciali nel nome e/o nel cognome.");
			break;
		case 3:
			puts("\n\t Il campo del nome e/o del cognome non puo' avere meno di 3 lettere.");
			break;
	}

	return flagErrore;
}






/*
 * FUNZIONI DI GENERAZIONE RANDOM
 */

/* Prende come parametro due valori, restituisce un valore intero casuale compreso tra i due valori dati come parametro */

int genRandom(int min, int max)
{
	return rand() % (max - min + 1) + min;
}



/* Alloca una stringa di dimensione pari al valore del parametro, in ogni cella della stringa viene inserita una lettera maiuscola
 * generando casualmente il suo codice ascii, che sarà compreso in un range definito da costanti. Poi restituisce la stringa allocata in partenza */

char* genOTP( unsigned int dim )
{
	char* otp = ( char* ) calloc( dim, sizeof( char ) ); // Dichiaro la stringa

	for ( unsigned char i = 0; i < dim; i++)											// In ogni cella ci inserisco una
		otp[i] = genRandom( ASCII_MINIMO_CARATTERE_OTP, ASCII_MASSIMO_CARATTERE_OTP );	// lettera maiuscola casuale

	return otp;
}






/*
 * FUNZIONI PER FILES
 */

/* Verifica l'esistenza sul disco di un file, dando come parametro il percorso
 * restituendo vero se esite, falso se non esiste.
 */

bool isFileExists( char nomeFile[] )
{
	bool flag = false;
	FILE* f = NULL;//inizializzo il puntatore al file con NULL

	if ( (f = fopen( nomeFile, "r+" )) == NULL ) flag = false;//se il file non esiste, il flag è false
	else flag = true;//se esiste il flag è true

	fclose( f );//chiudo il file

	return flag;//ritorno il flag
}







