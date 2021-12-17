/*
 * Recovery.h
 *
 *
 *      Author: FRANCESCO SOPRANO and MICHELE PERILLI
 */


#ifndef RECOVERY_H_
	#define RECOVERY_H_

	#define LUNGHEZZA_MASSIMA_COMANDO 100



	/*
	 * FUNZIONI PER IL RECUPERO DATI
	 */

	/**
	 * Annulla le modifiche effettuate sul file cronologia.sp
	 *
	 * @return Esito della chiamata al comando system();
	 */
	int ripristinaCronologiaOriginale( );



	/**
	 * Crea un file binario della cronologia clone, da usare nel caso l'utente voglia annullare le modifiche
	 *
	 * @return Esito della chiamata al comando system();
	 */
	int creaCronologiaTemp( );




	/**
	 * Chiamata quando l'utente decide di salvare le modifiche che ha effettuato, salva su file sia Accounts che Apps e
	 * elimina il file di recovery convalidando cosi' le modifiche effettuate.
	 *
	 * @param fileAccount File binario degli account su cui salvare le modifiche.
	 * @param accounts Vettore di account su cui sono state effettuate le modifiche.
	 * @param fileApp File binario delle app su cui salvare le modifiche.
	 * @param apps Vettore delle app su cui sono state effettuate le modifiche.
	 * @return -1 Se l'esito del salvataggio e' negativo
	 * @return 0 Se il salvataggio e' avvenuto con successo
	 */
	int applicaModifiche( FILE* fileAccount, Account accounts[], FILE* fileApp, App apps[]);



#endif /* RECOVERY_H_ */
