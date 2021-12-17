/*
 * 	main.c
 *
 *
 *      Author: FRANCESCO SOPRANO and MICHELE PERILLI
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <conio.h>
#include "Interfaccia.h"
#include "Tools.h"
#include "Recovery.h"
#include "AccountHead.h"
#include "AppHead.h"


#define LOGO_DLL "logo.dll"



int main()
{
	srand( time( NULL ) );

	system("mode con cols=150"); // Setto la dimensione della console

	//apro i file Apps, Accounts e Cronologia
	FILE* fileApp = apriAppsSP();

	FILE* fileAccount = apriAccountsSP();

	FILE* fileCronologia = apriCronologiaSP();

	/* Se l'apertura di almeno uno tre fileAccount o di fileApp è fallita, si impedisce l'accesso alle funzioni del programma */
	if ( fileAccount == NULL || fileApp == NULL || fileCronologia == NULL ) {

		puts("\nQualcosa e' andato storto, file non trovati.");
		puts("Premi un tasto per chiudere il programma.");

		getch();

	} else {
		
		App apps[MASSIMO_NUMERO_APP];				// Vettori di strutture in cui
		Account accounts[MASSIMO_NUMERO_ACCOUNT];	// caricheremo i file per lavorarci


		/* Carichiamo in vettori le app e gli account per poi presentare la richiesta di accesso o registrazione con menuPrincipale() */
		inizializzaApps( fileApp, apps );
		inizializzaAccounts( fileAccount, accounts );

		stampaIntro( LOGO_DLL );

		getDettagliApps( apps );
		getch();

		/* Per adesso questo file non servirà più, verrà aperto a tempo debito */
		fclose( fileCronologia );

		while ( menuPrincipale( fileAccount, fileApp, accounts , apps ) != 0 ); /* Riesegui la funzione fino a che non restituisce 0 ( vedi Account.c -> int menuPrincipale(...); )*/
												  /* Se la funzione menuPrincipale ritorna 0, vuol dire che l'utente vuole chiudere il programma     */
		getch();
	}


	fclose(fileAccount);	// Chiusura dei file rimasti aperti
	fclose(fileApp);		// prima di terminare il programma

	return 0;
}



	
