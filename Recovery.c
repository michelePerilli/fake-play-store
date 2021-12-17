/*
 * Recovery.c
 *
 *
 *      Author: FRANCESCO SOPRANO and MICHELE PERILLI
 */

#include <stdlib.h>
#include <string.h>
#include "Apps.h"
#include "Tools.h"
#include "Account.h"
#include "Cronologia.h"

#define RECORD_ASSOCIAZIONI_UTENTI_APP 750
#define LUNGHEZZA_MASSIMA_COMANDO 100



/* Elimina Cronologia.sp e Cronologia.sp_ diventa il nuovo Cronologia.sp, in altre parole elimina il file modificato e
 * rinomina Cronologia.sp_ in Cronologia.sp, cosi da annullare tutte le modifiche effettuate durante l'esecuzione del programma. */

int ripristinaCronologiaOriginale( )
{

	char comando[LUNGHEZZA_MASSIMA_COMANDO];			// Dichiaro e inizializzo la stringa che conterra'
	clearString( comando, LUNGHEZZA_MASSIMA_COMANDO );	// il comando che inviero' a system();

	// Comando da costruire = @echo f | xcopy [origine] [destinazione] /Y > log.txt"

	strcpy( comando, "@echo f | xcopy " );	// Costruisco il comando man mano
	strcat( comando, CRONOLOGIA_SP_COPIA );	// con strcat successive, usando
	strcat( comando, " " );					// il parametro > log.txt, che consiste
	strcat( comando, CRONOLOGIA_SP );		// nel scrivere il log.txt tutti
	strcat( comando, " /Y > log.txt" );		// gli output dell'esecuzione del comando system

	int esito = system( comando ); // System restituisce -1 se fallisce l'esecuzione del comando.

	if ( esito != -1 ) return system( "del Database\\Cronologia.sp_"); // Se non fallisce l'esecuzione, elimino il file copia e ritorno l'esisto dell'eliminazione
	else return esito; // Altrimenti ritorno l'esito di prima.
}



/* Creo il file Cronologia.sp di copia, ovvero Cronologia.sp_, il quale sarebbe una copia esatta dell'originale. Verra' usato nel caso vengano
 * effettuate modifiche sul file che vorranno essere annullate */

int creaCronologiaTemp( )
{
	if ( isFileExists( CRONOLOGIA_SP )){
		// Se il file Cronologia.sp esiste..

		char comando[LUNGHEZZA_MASSIMA_COMANDO];			// Dichiaro e inizializzo la stringa che conterra'
		clearString( comando, LUNGHEZZA_MASSIMA_COMANDO );	// il comando che inviero' a system();

		// Comando da costruire = @echo f | xcopy [origine] [destinazione] /Y > log.txt"

		strcpy( comando, "@echo f | xcopy " );	// Costruisco il comando man mano
		strcat( comando, CRONOLOGIA_SP );	// con strcat successive, usando
		strcat( comando, " " );					// il parametro > log.txt, che consiste
		strcat( comando, CRONOLOGIA_SP_COPIA );		// nel scrivere il log.txt tutti
		strcat( comando, " /Y > log.txt" );		// gli output dell'esecuzione del comando system

		return system( comando ); // Eseguo il comando e restituisco l'esito

	} else return 0; // Se il file cronologia.sp non esiste restituisco 0
}



/* Salva definitivamente le modifiche effettuate sui rispettivi file, riscrive tutto il file account usando il vettore di account,
 * tutto il file app usando il vettore di app. Poi
 */

int applicaModifiche( FILE* fileAccount, Account accounts[], FILE* fileApp, App apps[])
{
	int esito = 0; // esito = 0 ( positivo ) della funzione, valore da restituire verra' modificato non appena si verifica un errore.

	if ( aggiornaFileAccount(fileAccount, accounts) != 0 ){
		puts( "Errore, salvataggio dati accounts fallito.");
		esito = -1; // esito negativo se fallisce l'aggiornamento del file account
	}

	if ( aggiornaFileApp(fileApp, apps) != 0 ) {
		puts( "Errore, salvataggio dati apps fallito.");
		esito = -1; // esito negativo se fallisce l'aggiornamento del file account
	}


	if ( remove( CRONOLOGIA_SP_COPIA ) == -1 )
	{
		puts( "\nErrore, file cronologia non trovato");
		esito = -1; // esito negativo se fallisce l'eliminazione del file cronologia di copia
	}

	return esito;
}







