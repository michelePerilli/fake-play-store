/*
 * Cronologia.c
 *
 *
 *      Author: FRANCESCO SOPRANO and MICHELE PERILLI
 */


#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>

#include "Tools.h"
#include "Cronologia.h"
#include "Apps.h"
#include "Recovery.h"



/*Apre il file binario della cronologia, risolvendo eventuali problemi come l'assenza stessa del file.*/

FILE* apriCronologiaSP()
{

	if ( isFileExists( CRONOLOGIA_SP_COPIA ) ){
		// Se il file binario di copia esiste vuol dire che l'utente e' uscito dal programma senza scegliere se salvare o meno,
		// quindi annullo le modifiche effettuate nella scorsa sessione del programma ripristinando il file originale..

		if ( ripristinaCronologiaOriginale() == -1 ) puts("Errore nel richiamo di 'system'");

		else remove( CRONOLOGIA_SP_COPIA ); // Se il ripristino ha successo posso eliminare il file di copia.

	}



	FILE* fileCronologia = NULL;// Dichiaro e inizializzo il puntatore al file che restituiro'

	if ( isFileExists( CRONOLOGIA_SP ) )
		fileCronologia = fopen( CRONOLOGIA_SP, "rb+");		// Se il file Cronologia.sp ad accesso casuale esiste lo apro
	else
		fileCronologia = creaCronologiaSP();				// altrimenti lo ricreo leggendo dal file testuale di backup



	switch ( creaCronologiaTemp() ){ // Creo il file cronologia di copia, che servira' per poter annullare le modifiche
		case -1:
			puts("Errore nel richiamo di 'system'");
			break;
		case 0:
			// Cronologia.sp non trovato
			break;
	}


	return fileCronologia; // Ritorno il puntatore al file
}



/*Leggendo dal file di backup della cronologia, crea un file ad accesso casuale per la cronologia.*/

FILE* creaCronologiaSP()
{
	Cronologia record;						//
	Cronologia recordPulito;				// Dichiaro e pulisco due record cronologia, uno accogliera'
	clearRecordCronologia( &record );		// la riga letta da file, l'altro sara' un record pulito per inizializzare la riga
	clearRecordCronologia( &recordPulito ); //


	FILE* fileCronologia_bak = fopen( CRONOLOGIA_BAK, "r" );	// poi apro in lettura il file testuale di backup
	FILE* fileCronologia_sp;

	if ( fileCronologia_bak == NULL ){
		/* Se il file testuale di backup non esiste, ritorno NULL */

		fileCronologia_sp = NULL;

		puts("Cronologia.bak non trovato.");



	} else {
		/* Altrimenti porto in cima i puntatori di riga e inizio a leggere */
		fileCronologia_sp = fopen( CRONOLOGIA_SP, "wb+" ); // Creo il file Cronologia.sp, se esiste lo azzera,

		rewind( fileCronologia_sp );
		rewind( fileCronologia_bak );

		char riga[DIMENSIONE_RIGA_DA_LEGGERE];

		while( !feof(fileCronologia_bak) ){
			/* Finchè non siamo a fine file...
			 * - Leggo una riga (sapendo che la fgets si ferma al '\n' non andrò sicuramente a leggere oltre il record selezionato)
			 * - Uso la funzione strtok per spezzettare la mia riga letta in token, passando come parametro quale debba essere il separatore
			 * - Sapendo che strtok restituisce il puntatore al primo token creato, inizio a copiarlo nel campo nome, per poi riusare strtok con campo nullo
			 *   per passare al token successivo, che a sua volta verrà copiato in cognome con strcmp... e così via fino all'ultimo campo del record letto.
			 * - */

			clearString( riga, DIMENSIONE_RIGA_DA_LEGGERE );

			if( fgets( riga, DIMENSIONE_RIGA_DA_LEGGERE, fileCronologia_bak) != NULL ){
				// se la fgets legge qualcosa, prendo la riga e la spezzo in token ogni volta che inconto un ';'

				char* token = strtok( riga, ";" );
				strcpy( record.email, token );

				token = strtok(NULL, ";");
				strcpy( record.nomeApp, token );

				token = strtok(NULL, ";");
				record.isInstalled = strtol(token, NULL, 0);

				token = strtok(NULL, "\n");
				record.voto = (Voto) strtol(token, NULL, 0);


				/* Una volta ottenuto il record completo nella nostra struct, passiamo alla scrittura sul file binario: */

				fwrite(&recordPulito, sizeof(Cronologia), 1, fileCronologia_sp );	//
				fseek( fileCronologia_sp, -sizeof(Cronologia), SEEK_CUR );			// Scrivo prima un record pulito e poi il record appena letto dal file testuale
				fwrite(&record, sizeof(Cronologia), 1, fileCronologia_sp );			//
			}

		}
		fwrite(&recordPulito, sizeof(Cronologia), 1, fileCronologia_sp ); // A fine scrittura file binario, ci metto una riga vuota che useremo come FEOF

	}

	fclose( fileCronologia_bak ); /* Finite le operazioni chiudo il file di backup */
	return fileCronologia_sp; 	   /* e restituisco il puntatore a Cronologia.sp */
}



/* Pulisce il record di tipo Cronologia passato come parametro.*/

void clearRecordCronologia( Cronologia* record )
{
	//pulisco le stringhe
	clearString( record->email , strlen( record->email ) );
	clearString( record->nomeApp , strlen( record->nomeApp ) );

	record->isInstalled = false;//non installata
	record->voto = vNOPE;//nessun voto
}



/* Legge il record in posizione 'riga' dal file cronologia e lo restituisce*/

Cronologia leggiCronologia( int riga )
{
	FILE* fileCronologia = NULL;

	Cronologia record; // record Cronologia temporaneo dove salvare la riga

	if ( isFileExists( CRONOLOGIA_SP ) ){
		fileCronologia = fopen( CRONOLOGIA_SP,"rb+");
		//se il file Cronologia.sp esiste lo apro...

		fseek( fileCronologia, sizeof(Cronologia) * riga, SEEK_SET );	// Sposto il puntatore del file alla riga passata come parametro
		fread( &record, sizeof(Cronologia), 1, fileCronologia );		// e copio la riga nel record temporaneo

	}

	fclose( fileCronologia ); //chiudo il file

	return record; //ritorno il record temporaneo
}



/* Posizionandosi nella riga data da parametro sul file cronologia, scrive il record passato come parametro.*/

void modificaCronologia( Cronologia record, int riga )
{
	FILE* fileCronologia = NULL;

	if ( isFileExists( CRONOLOGIA_SP ) ){
		fileCronologia = fopen( CRONOLOGIA_SP,"rb+");
			//se il file Cronologia.sp esiste...

		fseek( fileCronologia, sizeof(Cronologia) * riga, SEEK_SET );//sposto il puntatore del file alla riga passata  come parametro
		fwrite(&record, sizeof(Cronologia), 1, fileCronologia );//modifica la riga scrivendo il record Cronologia passato come parametro

	}

	fclose( fileCronologia );//chiudo il file
}



/*Scrive il record passato come parametro nel file appendendolo agli altri gia esistenti*/

void aggiungiCronologia( Cronologia record )
{

	FILE* fileCronologia = NULL;

	if ( isFileExists( CRONOLOGIA_SP ) ){
		fileCronologia = fopen( CRONOLOGIA_SP,"rb+");
		//se il file Cronologia.sp esiste...

		Cronologia recordPulito;
		clearRecordCronologia( &recordPulito );

		fseek( fileCronologia, -sizeof(Cronologia), SEEK_END );  //sposto il puntatore del file a fine file
		fwrite(&record, sizeof(Cronologia), 1, fileCronologia ); //scrivo il contenuto del record Cronologia passato come parametro

		fwrite(&recordPulito, sizeof(Cronologia), 1, fileCronologia ); //scrivo un record vuoto

	}

	fclose( fileCronologia ); //chiudo il file

}



/* Legge dal file e prende tutti i record relativi all'app passata come parametro.*/

ElencoCronologia getCronologiaApp( const App* app )
{
	ElencoCronologia risultati;

	risultati.lunghezza = contaOccorrenzaAppinCronologia( app );					// Conto quante volte appare l'app nel file cronologia e in base a
	risultati.posizioni = ( int* ) calloc( risultati.lunghezza, sizeof( int ) );	// quel valore, dichiaro un ElencoCronologia che conterra' le posizioni di quell'app sul file

	int i = 0, j = 0;

	Cronologia record;

	do{

		clearRecordCronologia( &record );

		// Leggo l'i-esima riga del file cronologia
		record = leggiCronologia( i );

		if ( strcmp( record.nomeApp, app->nome ) == 0 && record.email[0] != '\0' ){
		// Se non siamo a fine file e abbiamo trovato il nome della nostra app in questa riga, mi salvo la riga dentro l'elenco
			risultati.posizioni[j] = i;
			j++;
		}

		i++;
	// e ripeto fin che non sono a fine file.
	} while( record.email[0] != '\0' );

	return risultati;
}



/* Legge dal file e prende tutti i record relativi all'utente passato come parametro.*/

ElencoCronologia getCronologiaUtente( const Account* utente )
{
	ElencoCronologia risultati;

	risultati.lunghezza = contaOccorrenzaUtenteinCronologia( utente );				// Conto quante volte appare l'utente sul file cronologia e in base a quel
	risultati.posizioni = ( int* ) calloc( risultati.lunghezza, sizeof( int ) );	// Valore dichiaro un elenco cronologia che conterra' tutte le posizioni dell'utente nel file

	int i = 0, j = 0;

	Cronologia record;

	do{

		clearRecordCronologia( &record );

		// Leggo l'i-esima riga del file cronologia
		record = leggiCronologia( i );

		if ( strcmp( record.email, utente->credenziali.email ) == 0 && record.email[0] != '\0' ){
		// Se non siamo a fine file e abbiamo trovato la email del nostro utente in questa riga, mi salvo la riga dentro l'elenco
			risultati.posizioni[j] = i;
			j++;
		}

		i++;

	// e ripeto finche' non sono a fine file.
	} while( record.email[0] != '\0' );


	return risultati;
}



/* Leggendo il file cronologia carico nel vettore di app i voti e i download relativi ad ogni app. Ogni riga letta nel file cronologia corrisponde ad
 * una app nel vettore di app,*/

void getDettagliApps( App apps[] )
{

	unsigned int i = 0;

	Cronologia temp;

	do{

		clearRecordCronologia( &temp );

		// Leggo la i-esima riga del file cronologia
		temp = leggiCronologia( i );


		/* Qui avevo bisogno di scoprire l'indice dell'app nel vettore conoscendo il suo indirizzo, quindi ho fatto
		 * ricorso all'algebra dei puntatori e effettuando una sottrazione ho ottenuto l'indice che volevo. Una volta trovato l'indice di quell'app
		 * trovata nella i-esima riga del file cronologia, uso quello appena letto dal file per aggiornare i dati dell'app nel vettore di app */

		int indiceAppCorrente = ricercaApp( apps, temp.nomeApp, rNOME ).vettoreApp[0] - &apps[0];

		apps[ indiceAppCorrente ].totDownload++; //Aumento quindi il numero di downlaod di quell'app

		if( temp.voto > vNOPE ) {
			// Se il voto e' stato assegnato, aumento il totale dei voti e sommo il voto alla somma parziale.
			apps[ indiceAppCorrente ].totVoti++;
			apps[ indiceAppCorrente ].sommaVoti += temp.voto;
		}


		i++;

	// Continuo finche' non siamo a fine file
	} while ( temp.email[0] != '\0' );

}



/* Conta quante volte appare l'app nel file cronologia.*/

int contaOccorrenzaAppinCronologia( const App* app )
{

	//azzero il contatore delle occorrenze dell'app nel File Cronologia.sp
	int occorrenze = 0;

	//record Cronologia temporaneo
	Cronologia record;

	//parto dall'inizio del file
	int i = 0;

	do {

		//pulisco il record Cronologia temporaneo
		clearRecordCronologia( &record );

		//leggo la riga, la copio nel record temporaneo e mi sposto di una riga avanti nel file incrementando l'indice
		record = leggiCronologia( i++ );

		//se trovo il nome dell'app passata come parametro nel file incremento le occorrenze
		if ( strcmp( record.nomeApp, app->nome ) == 0 ) occorrenze++;


	// Continuo fino a che non arrivo a leggere un record che inizia con '\0', cioe' l'FEOF personale ideato in creaFileCronologiaSP()
	} while( record.email[0] != '\0' );


	//ritorno le occorenze dellapp nel file Cronologia.sp
	return occorrenze;
}


/* Conta quante volte appare l'utente nel file cronologia.*/
int contaOccorrenzaUtenteinCronologia( const Account* utente )
{

	//azzero il contatore delle occorrenze dell'utente nel File Cronologia.sp
	int occorrenze = 0;

	//record Cronologia temporaneo
	Cronologia record;

	//parto dall'inizio del file
	int i = 0;

	do {

		//pulisco il record Cronologia temporaneo
		clearRecordCronologia( &record );

		//leggo la riga, la copio nel record temporaneo e mi sposto di una riga avanti nel file incrementando l'indice
		record = leggiCronologia( i++ );

		//se trovo la email dell'utente passato come parametro nel file incremento le occorrenze
		if ( strcmp( record.email, utente->credenziali.email ) == 0 ) occorrenze++;


	//ripeto fin quando non siamo arrivati a fine file
	} while( record.email[0] != '\0' );

	//ritorno le occorenze dell'utente nel file Cronologia.sp
	return occorrenze;
}



/* Cerca l'app nel file cronologia e ne restituisce la posizione della riga.*/

int cercaAppInElencoCronologia( const App* app, ElencoCronologia elenco )
{

	int posizione = -1; // Posizione iniziale, se ritornera' questo valore vuol dire che l'app non sara' nella cronologia

	for ( unsigned int i = 0; i < elenco.lunghezza; i++ ){
	// scorro nell'elencoCronologia passato come parametro..

		// Leggo sul file cronologia la riga che dice il mio elenco in posizione i-esima
		Cronologia record = leggiCronologia( elenco.posizioni[i] );

		if ( strcmp( app->nome, record.nomeApp ) == 0 ){
		//se trova il nome dell'app passata come parametro...

			posizione = i;//salvo la posizione nel
			i = elenco.lunghezza;//esco dal ciclo

		}
	}

	return posizione;//ritorno la posizione dell'app nel file Cronologia.sp
}



/* Stampa le info della cronologia riguardante una specifica app.*/

void stampaInfoCronologiaApp( const App* app )
{
	system("cls");

	ElencoCronologia elenco = getCronologiaApp( app ); // Leggo dal file cronologia tutto quello riguardante l'app che ho nel parametro

	puts(" Dove ti trovi: Menu di Accesso / Menu Admin / Apps / Dettagli avanzati");

	printf("\n\n ***** Utenti che hanno scaricato %s: *****\n\n", app->nome);

	for ( unsigned int i = 0; i < elenco.lunghezza; i++ )
		printf("\t- %s\n", leggiCronologia( elenco.posizioni[i] ).email );

	puts("\nPremi un tasto per tornare indietro");
	getch();

}


