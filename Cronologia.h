/*
 * Cronologia.h
 *
 *
 *      Author: FRANCESCO SOPRANO and MICHELE PERILLI
 */

#include <stdio.h>

#include "CronologiaHead.h"

#ifndef CRONOLOGIA_H_

/* Uso come identificativo di dichiarazione avvenuta della libreria INTERFACCIA_H_
 * - se questa libreria non è mai stata dichiarata, verrà definita la costante che impedirà
 * 	 la reinclusione di questa stessa libreria	*/

	#define CRONOLOGIA_H_



	/*
	 * FUNZIONI PER LA GESTIONE DELLE RELAZIONI ACCOUNTS-APPS
	 */

	/**
	 * Apre il file binario della cronologia, risolvendo eventuali problemi come l'assenza stessa del file.
	 *
	 * @return Puntatore al file binario della cronologia.
	 * @warning Se il file non esiste verra' creato leggendo dal file di backup, il quale dovra' per forza essere
	 * 			esistente per poter proseguire con il programma, altrimenti ritornerà NULL
	 */
	FILE* apriCronologiaSP();



	/**
	 * Leggendo dal file di backup della cronologia, crea un file ad accesso casuale per la cronologia.
	 *
	 * @return Puntatore al file binario della cronologia.
	 */
	FILE* creaCronologiaSP();



	/**
	 * Pulisce il record di tipo Cronologia passato come parametro.
	 *
	 * @param record Cronologia da pulire.
	 */
	void clearRecordCronologia( Cronologia* record );






	/*
	 * FUNZIONI DI LAVORO SU FILE
	 */

	/**
	 * Legge il record in posizione 'riga' dal file cronologia e lo restituisce
	 *
	 * @param riga Posizione da cui leggere un record dal file cronologia.
	 * @return Record letto dal file cronologia.
	 */
	Cronologia leggiCronologia( int riga );



	/**
	 * Posizionandosi nella riga data da parametro sul file cronologia, scrive il record passato come parametro.
	 *
	 * @param record Record di cronologia da scrivere nella posizione data nel file.
	 * @param riga Posizione nel file in cui scrivere il record dato.
	 */
	void modificaCronologia( Cronologia record, int riga );



	/**
	 *  Scrive il record passato come parametro nel file appendendolo agli altri gia esistenti
	 *
	 * @param record Record da aggiungere al file.
	 */
	void aggiungiCronologia( Cronologia record );



	/**
	 * Legge dal file e prende tutti i record relativi all'utente passato come parametro.
	 *
	 * @param utente Puntatore all'utente di cui vogliamo estrarre i dati dal file cronologia.
	 * @return ElencoCronologia contenente tutti i record relativi alle azioni dell'utente,
	 * @return NULL Se non sono stati trovati record relativi all'utente.
	 */
	ElencoCronologia getCronologiaUtente( const Account* utente );



	/**
	 * Legge dal file e prende tutti i record relativi all'app passata come parametro.
	 *
	 * @param app Puntatore all'app di cui vogliamo estrarre i dati dal file cronologia.
	 * @return ElencoCronologia contenente tutti i record relativi all'app data da parametro,
	 * @return NULL Se non sono stati trovati record relativi all'app.
	 */
	ElencoCronologia getCronologiaApp( const App* app );



	/**
	 * Leggendo il file cronologia carico nel vettore di app i voti e i download relativi ad ogni app.
//	 *
	 * @param apps Vettore di app da completare con i dati presenti su file.
	 */
	void getDettagliApps( App apps[] );



	/**
	 * Conta quante volte appare l'app nel file cronologia.
	 *
	 * @param app Puntatore all'app di cui vogliamo sapere quante volte appare nel file cronologia.
	 * @return Occorrenza dell'app nel file cronologia.
	 */
	int contaOccorrenzaAppinCronologia( const App* app );



	/**
	 * Conta quante volte appare l'utente nel file cronologia.
	 *
	 * @param utente Puntatore all'utente di cui vogliamo sapere quante volte appare nel file cronologia.
	 * @return Occorrenza dell'utente nel file cronologia.
	 */
	int contaOccorrenzaUtenteinCronologia( const Account* utente );






	/*
	 * FUNZIONI PER LA RICERCA
	 */

	/**
	 * Cerca l'app nel file cronologia e ne restituisce la posizione della riga.
	 *
	 * @param app Puntatore all'app da cercare.
	 * @param elenco ElencoCronologia in cui cercare l'app.
	 * @return Posizione dell'app nell'elenco;
	 * @return -1 Se l'app non e' presente nell'elenco.
	 */
	int cercaAppInElencoCronologia( const App* app, ElencoCronologia elenco);






	/*
	 * FUNZIONI PER L' OUTPUT
	 */

	/**
	 * Stampa le info della cronologia riguardante una specifica app.
	 *
	 * @param app Puntatore ad app di cui vogliamo stampare le info
	 */
	void stampaInfoCronologiaApp( const App* app );

#endif /* CRONOLOGIA_H_ */





