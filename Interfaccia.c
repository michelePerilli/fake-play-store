/*
 * Interfaccia.c
 *
 *
 *      Author: FRANCESCO SOPRANO and MICHELE PERILLI
 */

#include <stdlib.h>
#include <ctype.h>
#include <conio.h>
#include <string.h>

#include "Apps.h"
#include "Account.h"
#include "Tools.h"
#include "Cronologia.h"
#include "Recovery.h"

/*
 * APPS
 */

/* Visualizza il menu della singola app, in base se invocata da un admin o da un utente variano leggermente le funzioni disponibili:
 * Utente: Dis/Installa, Votare, Info App
 * Admin: Quali utenti l'hanno scaricata, Elimina l'app, Info App
 *
 * Verra' distinto da un utente o un admin in base al puntatore ad utente, il quale se sara' NULL allora sara' l'admin, altrimenti sara' un utente semplice*/

int menuApp( App* app, Account* utente, App apps[] )
{
	system("cls");

	int riga = 0;
	bool installata = false;
	ElencoCronologia lista;

	// In base al chiamante della funzione, visualizzo il percorso in cui l'utilizzatore del programma si trova
	if ( utente != NULL)
		puts(" Posizione: Menu di Accesso / Menu Utente / Ricerca / Menu App");
	else
		puts(" Posizione: Menu di Accesso / Menu Admin / Ricerca / Menu App");

	// Stampo nome e sviluppatore dell'app
	printf("\n\n\t %s - by %s:\n\n", app->nome, app->sviluppatore);


	if ( utente != NULL ) {
		// Se l'utente non e' l'amministratore..

		lista = getCronologiaUtente( utente ); 			//
		riga = cercaAppInElencoCronologia( app, lista );//


		if ( riga == -1 ){
			// Se non e' stata mai installata controllo il prezzo e imposto il flag..

			if( app->costo > 0 )
				printf( "\t [1] Installa con %.2f $\n", app->costo );
			else
				puts( "\t [1] Installa" );

			installata = false;

		} else {
			// se e' stata gia installata leggo dal file e imposto il flag in base a se e' installata o meno..

			Cronologia record = leggiCronologia( lista.posizioni[riga] );

			if ( record.isInstalled ){
				puts( "\t [1] Disinstalla" );
				installata = true;
			} else {
				puts( "\t [1] Re-Installa" );
				installata = false;
			}

		}

		puts( "\t --------------------" );
		puts( "\t [2] Lascia un voto" );
		puts( "\t --------------------" );
		puts( "\t [3] Info App" );
		puts( "\t --------------------" );
		puts( "\t [4] Indietro\n" );

	}  else {
		// Se e' un amministratore il menu' sara' un po' diverso

		puts( "\t [1] Chi ha scaricato questa app?" );
		puts( "\t --------------------" );
		puts( "\t [2] Elimina" );
		puts( "\t --------------------" );
		puts( "\t [3] Info App" );
		puts( "\t --------------------" );
		puts( "\t [4] Indietro\n" );

	}

	switch( inputIntero(1, 4, "\t Input -> ") ){ // chiedo un input numerico, in base all'inserimento e al tipo di utente, scelgo la funzione
		case 1: // Dis/installa app o mostra cronologia app
		{
			if ( utente != NULL )
				// Se l'utente non e' l'amministratore..
				installaApp( app, utente, installata, apps );
			else
				stampaInfoCronologiaApp( app );
			break;
		}
		case 2: // Vota app o Elimina App
		{
			if( utente != NULL ){
				// Se l'utente non e' l'amministratore..
				if ( installata == true )
					lasciaUnVoto( lista.posizioni[riga] );
				else {
					puts("\n\t Devi installarla prima di poterla votare");
					getch();
				}

			} else {
				// Se l'utente e' l'amministratore..
				if ( eliminaApp( app ) == 1 )
					puts("\n\t Eliminazione app avvenuta correttamente!");
				else
					puts("\n\t Eliminazione app fallita!");

				getch();
				return 0;
			}
			break;
		}
		case 3: // Mostra info app
		{
			infoApp( app );

			break;
		}
		default: // Indietro
			return 0;
			break;
	}

	return 1;


}

/* Dopo una ricerca, viene generato un ElencoApp che passato a questo menu', si da la possibilita' all'utente passato come parametro di scegliere
 * l'app che desidera e di entrare nel suo menu', per poi decidere che altra funzione usare.*/
int menuAppTrovate( ElencoApp elencoApp, Account* utente, App apps[] )
{
	system("cls");
	puts(" Posizione: Menu di Accesso / Menu Utente / Ricerca");

		stampaElencoApp( elencoApp );//stampo l'elenco delle app trovate

		//visualizzo le possibili scelte di ordinamento
		printf( "\n\n\t\t>Ordina per:" );
		printf( "\n\t\t [%d] Nome", 				elencoApp.lunghezza );
		printf( "\n\t\t [%d] Con piu' Downloads",	elencoApp.lunghezza + 1 );
		printf( "\n\t\t [%d] Meglio votate",		elencoApp.lunghezza + 2 );
		printf( "\n\t\t [%d] Sviluppatore",			elencoApp.lunghezza + 3 );
		printf( "\n\t\t [%d] Genere",				elencoApp.lunghezza + 4 );
		printf( "\n\t\t [%d] Economiche",			elencoApp.lunghezza + 5 );
		printf( "\n\t\t [%d] Dimensione", 			elencoApp.lunghezza + 6 );

		printf("\n\n\t\t [%d] Indietro\n\n",		elencoApp.lunghezza + 7 );


		//input della scelta
		int input = inputIntero( 0, elencoApp.lunghezza + 7, "\t\t Input -> " );


		if ( input < elencoApp.lunghezza ){
			// Se l'input ricade su un elemento del vettore..

			while ( menuApp( elencoApp.vettoreApp[input], utente, apps ) != 0 ); //entro nel menù dell'app scelta
			if ( utente == NULL ) return 0; // se e' un amministratore, in questo punto lo faccio tornare indietro

		} else if ( input == elencoApp.lunghezza + 7 ){
			// Se sceglie indietro..

			return 0;//esce dal menù

		} else {
			// Altrimenti in base alla scelta del tipo ordinamento ordino le app
			switch ( input - elencoApp.lunghezza ) {
				case 0:
				{
					ordinaApp( elencoApp, oNOME );
					break;
				}
				case 1:
				{
					ordinaApp( elencoApp, oDOWNLOAD );
					break;
				}
				case 2:
				{
					ordinaApp( elencoApp, oVOTO );
					break;
				}
				case 3:
				{
					ordinaApp( elencoApp, oSVILUPPATORE );
					break;
				}
				case 4:
				{
					ordinaApp( elencoApp, oGENERE );
					break;
				}
				case 5:
				{
					ordinaApp( elencoApp, oPREZZO );
					break;

				}
				case 6:
				{
					ordinaApp( elencoApp, oDIMENSIONE );
					break;
				}
			}
		}

	return 1;
}


/* Visualizza il menu' contenente le app che l'utente passato come parametro ha installato almeno una volta, potendo scegliere di reinstallare quelle
 * gia disinstallate oppure disinstallare quelle attualmente installate. */
int menuLeMieApp( ElencoCronologia lista, Account* utente, App apps[] )
{
	system("cls");

	puts(" Posizione: Menu di Accesso / Menu Utente / Profilo / Le mie app");

	char stato[sizeof( "Disinstallata" ) / sizeof( char )]; // conterra' "Installata" o "Disinstallata"


	printf( "\n\t %-49s %-29s %-10s", "Nome", "Stato", "Voto");
	printf( "\n\t --------------------------------------------------------------------------------------------" );

	for( int i = 0; i < lista.lunghezza; i++){
		// Per ogni app nell'elenco..

		Cronologia record = leggiCronologia( lista.posizioni[i] ); //Leggo il record dal file in posizione giusta


		if ( record.isInstalled == false ) strcpy ( stato, "Disinstallata");	// In base al fatto se risulta attualmente
		else strcpy ( stato, "Installata   " );									// installata o meno stampo diversamente

		printf( "\n\t [%d] %-45s %-20s %10d", i, record.nomeApp, stato, record.voto );
		printf( "\n\t --------------------------------------------------------------------------------------------" );
	}

	printf("\n\t [%d] Indietro\n\n", lista.lunghezza);


	int input = inputIntero(0, lista.lunghezza, "\t Input -> "); // Chiedo l'input della scelta..


	if ( input != lista.lunghezza){
		// Se non ha scelto di tornare indietro..

		Cronologia record = leggiCronologia( lista.posizioni[input] ); // Leggo il record in quella posizione

		App* app = ( App* ) ricercaAppIntegrale( apps, record.nomeApp ); // Estraggo il puntatore di quell'app sapendo il nome

		while ( menuApp( app, utente, apps ) != 0); // ed entro nel menu dell'app


	} else {
		// Se ha scelto di tornare indietro..
		return 0;

	}

	return 1;
}



/* Visualizza il menu' contenente tutte le app, su cui l'utente puo' interagire, scegliendo quella che vuole.*/

void menuTutteLeApp( Account* utente, App apps[] )
{
	ElencoApp tutteLeApp = ricercaApp( apps, "", rNOME ); // Elenco di app contenente tutte le app

	while ( menuAppTrovate( tutteLeApp, utente, apps ) != 0 ); // Menu per quelle app
}



/* Visualizza il menu' per poter scegliere la ricerca app che l'utente preferisce: ricerca per Nome, Genere e Sviluppatore.*/

void menuCercaApp( Account* utente, App apps[] )
{
	system("cls");
	puts(" Posizione: Menu di Accesso / Menu Utente / Ricerca");

	// Visualizzo i vari criteri di ricerca per l'app

	puts("\n\n ************MENU DI RICERCA APP***********");
	puts("\n\n\t Effettua la ricerca per:\n");
	puts( "\t [1] Titolo" );
	puts( "\t --------------------" );
	puts( "\t [2] Sviluppatore" );
	puts( "\t --------------------" );
	puts( "\t [3] Genere" );
	puts( "\t --------------------" );
	puts( "\t [4] Indietro\n" );

	ElencoApp elenco;
	switch( inputIntero(1, 4, "\t Input -> ") ){
		//in base alla scelta ricerco le app e visualizzo il menù delle app trovate altrimenti avviso l'utente che la sua ricerca non ha portato alcun risultato
		case 1:
		{
			puts( "\n\n\t> Ricerca App per titolo:");

			elenco = ricercaAppTempoReale( apps, rNOME ); // riempio l'elenco con i risultati della ricerca

			break;
		}
		case 2:
		{
			puts( "\n\n\t> Ricerca App per sviluppatore:");

			elenco = ricercaAppTempoReale( apps, rSVILUPPATORE ); // riempio l'elenco con i risultati della ricerca

			break;
		}
		case 3:
		{
			puts( "\n\n\t> Ricerca App per genere:");

			elenco = ricercaAppTempoReale( apps, rGENERE ); // riempio l'elenco con i risultati della ricerca

			break;
		}

	}

	// Una volta effettuata la ricerca e avendo i risultati, li mostro
	if ( elenco.vettoreApp == NULL ){
		printf("\n\n\t\t La ricerca non ha prodotto alcun risultato\n " );
		puts("\n\t\t Premi un tasto per tornare indietro.");
		getch();
	} else {
		system("cls");
		while ( menuAppTrovate( elenco, utente, apps ) != 0 ); // Menu per quei risultati
	}


}



/* Visualizza l'elenco delle miglioli app, per maggior numero di download o per miglior voto.*/

void menuTopApps( Account* utente, App apps[], TipoTop tipo )
{

	ElencoApp elenco = ricercaApp( apps, "", rNOME );//elencoApp temporaneo contentente tutte le app presenti nello store

	//in base al tipo di classifica top ordino le app
	if ( tipo == TOPDOWNLOAD )
		ordinaApp( elenco, oDOWNLOAD );
	else
		ordinaApp( elenco, oVOTO );

	elenco.lunghezza = NUMERO_APP_NELLA_TOP;


	while ( menuAppTrovate( elenco, utente, apps ) != 0 );//visualizzo il menù delle app trovate
}




int menuAppSuggerite( Account accounts[], Account* utente,  App apps[] )
{
	system("cls");
		puts(" Posizione: Menu di Accesso / Menu Utente / Suggerisci App\n");

		puts("\n\n\tCosa vuoi fare?:\n");

		//visualizzo all'utente tra quali suggerimenti di app può scegliere
		puts( "\t [1] Suggerisci per categorie installate" );
		puts( "\t --------------------" );
		puts( "\t [2] Suggerisci per utenti simili" );
		puts( "\t --------------------" );
		puts( "\t [3] Ultime app aggiunte allo store" );
		puts( "\t --------------------" );
		puts( "\t [4] Indietro\n" );

		//in base alla scelta del tipo di suggerimento..-
		switch( inputIntero(1, 4, "\t Input -> ") ){
				case 1:
				{
					int* n = getFrequenzeCategorie( elencoCronologiaToElencoApp( getCronologiaUtente( utente ), apps ) );

					ElencoApp elenco = consigliaAppPerCategoria( n, apps );//elencoApp temporaneo con le app consigliate in base alle categorie che ha installato

					if( elenco.lunghezza == 0 ){//vuol dire che non ha ancora installato apps quindi non si possono consigliare applicazioni
						puts( "\n\t Non hai installato alcuna app" );
						puts( "\n\t Premi un tasto per tornare indietro" );
						getch();
					}else
						while ( menuAppTrovate( elenco, utente, apps ) != 0 );//visualizzo il menù per quei risultati

					break;
				}
				case 2:
				{

					ElencoCronologia consigli = consigliaAppPerEta( accounts, utente->fasciaEta );//elencoApp temporaneo con le app di utente con la sua stessa età

					ElencoApp listaConsigli = elencoCronologiaToElencoApp( consigli, apps );

					while ( menuAppTrovate( listaConsigli, utente, apps ) != 0 );//visualizzo il menù per quei risultati



					break;
				}
				case 3:
				{
					ElencoApp elenco =  ultimeAppAggiunte( apps );//elencoApp temporaneo contente le ultime app aggiunte allo store

					if ( elenco.vettoreApp == NULL ) puts("\n\t Non ci sono apps aggiunte allo store");
					else
						while ( menuAppTrovate( elenco, utente, apps ) != 0 );//visualizzo il menù per quei risultati

					break;
				}
				default:
					return 0;//esco dal menù
					break;

			}
			return 1;

}



/* Menu in cui l'utente sceglie quale funzione usare sulle app: es. Ricerca, Migliori app, Suggerimento app...*/

int menuApps( Account accounts[], Account* utente, App apps[] )
{
	system("cls");
	puts(" Posizione: Menu di Accesso / Menu Utente / Apps\n");

	puts("\n\n\tCosa vuoi fare?:\n");

	puts( "\t [1] Tutte le App" );
	puts( "\t --------------------" );
	puts( "\t [2] App piu' scaricate" );
	puts( "\t --------------------" );
	puts( "\t [3] App piu' apprezzate" );
	puts( "\t --------------------" );
	puts( "\t [4] App suggerite" );
	puts( "\t --------------------" );
	puts( "\t [5] Cerca App" );
	puts( "\t --------------------" );
	puts( "\t [6] Indietro\n" );


	switch( inputIntero(1, 6, "\t Input -> ") ){
		// In base all'input scelgo la procedura da utilizzare
			case 1:
			{

				menuTutteLeApp( utente, apps );
				break;
			}
			case 2:
			{
				menuTopApps( utente, apps, TOPDOWNLOAD );

				break;
			}
			case 3:
			{
				menuTopApps( utente, apps, TOPVOTO );
				break;
			}
			case 4:
			{
				menuAppSuggerite( accounts, utente, apps );
				break;
			}
			case 5:
			{
				menuCercaApp( utente, apps );
				break;
			}
			default:
				return 0;
				break;
		}
	return 1;
}





/*
 * UTENTE
 */

/* Menu che racchiude le funzioni che l'utente puo' effettuare su se stesso, ad esempio vedere le suo info, reimpostare la password, vedere le sue app, ecc..*/

int menuProfilo( Account* utenteTrovato, Account* utente, App apps[] )
{
	system("cls");

	if ( utente != NULL )//se l'utente non è admin..
		puts(" Posizione: Menu di Accesso / Menu Utente / Profilo");
	else
		puts(" Posizione: Menu di Accesso / Menu Admin / Profilo di un utente");//se l'utente è l'admin...




	puts("\n\n\tCosa vuoi fare?:\n");



	if ( utente != NULL ){//se non è admin...

		//visualizzo tutte le scelte possibili
		puts( "\t [1] Info Utente" );
		puts( "\t --------------------" );
		puts( "\t [2] Le mie App" );
		puts( "\t --------------------" );
		puts( "\t [3] Ricarica" );
		puts( "\t --------------------" );
		puts( "\t [4] Reimposta password" );
		puts( "\t --------------------" );
		puts( "\t [5] Elimina Account" );
		puts( "\t --------------------" );
		puts( "\t [6] Indietro\n" );

		//in base alla scelta può...
		switch( inputIntero(1, 6, "\t Input -> ") ){
			case 1:
			{
				puts( "\n\n\t> Info utente:");
				infoUtente( utente ); //visualizzare le informazioni relative al suo account ( nome, cognome, email, redito....)

				break;
			}
			case 2://visualizzare le app installate da quando si è registrato( anche quelle disinstallate )
			{
				ElencoCronologia elenco = getCronologiaUtente( utente );//elencoCronologia temporaneo che contiene le app sue app installate

				if ( elenco.lunghezza == 0 ){//se non ha app installato nessun'app avviso l'utente
					puts("\n\t Non hai installato alcuna app");
					puts("\n\t Premi un tasto per tornare indietro");
					getch();
				}
				else//altrimenti apro il menù corrispondente
					while ( menuLeMieApp( elenco, utente, apps ) != 0);

				break;
			}
			case 3://ricaricare il conto
			{
				if ( ricarica( utente ) == 1 )
					puts("\n\t Ricaricato con successo!");
				else
					puts("\n\t Ricarica annullata!");

				getch();
				break;
			}
			case 4://reimpostare la propria passworrd
			{
				if ( reimpostaPassword( utente ) == 1 )
					puts("\n\t Password reimpostata con successo!");
				else
					puts("\n\t Operazione annullata!");

				getch();
				break;
			}
			case 5://eliminare il suo account
			{
				if ( eliminaUtente( utente ) == 1 ) {

					puts("\n\n\t Eliminazione dell'account riuscita.");
					getch();
					return 0;

				} else{

					puts("\n\n\t Eliminazione dell'account fallita.");
					getch();

				}


				break;
			}
			case 6:
			{
				return 0;//uscire dal menù
				break;
			}
		}

	} else {//se l'utente è l'admin...

		//visualizzo cosa può fare con un utente..
		puts( "\t [1] Info Utente" );
		puts( "\t --------------------" );
		puts( "\t [2] Le sue App" );
		puts( "\t --------------------" );
		puts( "\t [3] Elimina Account" );
		puts( "\t --------------------" );
		puts( "\t [4] Indietro\n" );


		//in base alla scelta può....
		switch( inputIntero(1, 4, "\t Input -> ") ){
			case 1://visualizzare le sue informazioni
				puts( "\n\n\t> Info utente:");
				infoUtente( utenteTrovato );

				break;
			case 2://visualizzare cosa ha installato
			{
				ElencoCronologia elenco = getCronologiaUtente( utenteTrovato );

				if ( elenco.lunghezza == 0 ){
					puts("\n\t Non hai installato alcuna app");
					puts("\n\t Premi un tasto per tornare indietro");
					getch();
				}
				else
					while ( menuLeMieApp( elenco, utenteTrovato, apps ) != 0);

				break;
			}
			case 3://eliminare il suo account
			{
				while( eliminaUtente( utenteTrovato ) != 1 );
				return 0;
				break;
			}
			case 4:
			{
				return 0;//uscire dal menù
				break;
			}
		}
	}

	return 1;
}

/* Menu in cui l'utente effettua la scelta se usare funzioni per le apps oppure su se stesso.*/

int menuUtente( Account accounts[], Account* utente, App apps[])
{
	system("cls");
	puts(" Posizione: Menu di Accesso / Menu Utente\n");

	printf("\n ============= BENTORNATO %s %s !! =============\n\n\n\tCosa vuoi fare?\n\n", utente->datiAnagrafici.nome, utente->datiAnagrafici.cognome);

	//visualizzo le possibili scelte
	puts( "\t [1] Apps");
	puts( "\t --------------------" );
	puts( "\t [2] Profilo" );
	puts( "\t --------------------" );
	puts( "\t [3] LogOut\n" );

	//in base alla scelta può...
	switch( inputIntero(1, 4, "\t Input -> ") ){
		case 1://visualizzare il menù relativo alle apps
		{
			while ( menuApps( accounts, utente, apps ) != 0 );
			break;
		}
		case 2://visualizzare il menù relativo al suo profilo
		{
			while ( menuProfilo( NULL ,utente, apps ) != 0 && utente->isActive == true );
			break;
		}
		case 3://fare il logout e salvare le modifiche
		{
			printf("\n\t Sei sicuro di voler uscire? Le modifiche verranno salvate. (Y-N)->");
			if ( tolower( getche() ) == 'y' ) return 0;
			break;
		}
		default:
				break;
	}
	return 1;
}



/* Visualizza il menu' contente gli utenti trovati da un'altra funzione, cioe' presente il ElencoAccount.*/

int menuUtentiTrovati( ElencoAccount elencoAccount, App apps[] )
{
	system("cls");

	puts(" Prosizione: Menu di Accesso / Menu Admin / Ricerca utenti");


	//stampo l'elenco degli utenti attivi
	stampaElencoAccount( elencoAccount );

	//visualizzo i possibili criteri di ordinamento
	printf( "\n\n\t\t>Ordina per:" );
	printf( "\n\t\t [%d] Nome", elencoAccount.lunghezza );
	printf( "\n\t\t [%d] Cognome", elencoAccount.lunghezza + 1 );
	printf( "\n\t\t [%d] Email", elencoAccount.lunghezza + 2 );

	printf("\n\n\t\t [%d] Indietro\n\n", elencoAccount.lunghezza + 3 );

	int input = inputIntero( 0, elencoAccount.lunghezza + 3, "\t\t Input -> " );//input della scelta


	if ( input < elencoAccount.lunghezza ){
		// Se sceglie una delle app nell'elenco..

		while ( menuProfilo( elencoAccount.vettoreUtenti[input], NULL, apps ) != 0 );//visualizzo il menù dell'utente scelto
		return 0;

	} else if ( input == elencoAccount.lunghezza + 3 ){
		// Se sceglie indietro
		return 0;//esce dal menù

	} else {
		// Altrimenti in base alla scelta del tipo ordinamento ordino gli utenti
		switch ( input - elencoAccount.lunghezza ) {
			case 0:
			{
				ordinaAccount( elencoAccount, uNOME );
				break;
			}
			case 1:
			{
				ordinaAccount( elencoAccount, uCOGNOME );
				break;
			}
			case 2:
			{
				ordinaAccount( elencoAccount, uEMAIL );
				break;
			}

		}
	}

	return 1;
}



/* Visualizza il menu' contenente tutti gli account attivi*/

void menuTuttiGliAccount( Account accounts[], App apps[] )
{
	ElencoAccount tuttiGliAccount = getElencoUtentiAttivi ( accounts );

	while ( menuUtentiTrovati( tuttiGliAccount, apps ) != 0 );
}



/* Menu in cui l'utente sceglie se accedere al suo profilo oppure se registrarsi nel caso non ne abbia uno. E' possibile scegliere anche l'accesso in amministratore
 * se si e' a conoscenza delle credenziali che solo gli sviluppatori del programma conoscono.*/

int menuPrincipale( FILE* fileAccount, FILE* fileApp, Account accounts[] , App apps[] )
{
	system("cls");

	puts("\n\n ============= BENVENUTO IN PLAYSTORE!! ============= ");

	//visualizzo tutte le scelte possibili
	puts("\n\n\tCosa vuoi fare?:\n");
	puts( "\t [1] Accedi" );
	puts( "\t --------------------" );
	puts( "\t [2] Registrati" );
	puts( "\t --------------------" );
	puts( "\t [3] Amministratore Database" );
	puts( "\t --------------------" );
	puts( "\t [4] Esci e Salva\n" );

	puts( "\n\t\t Note: Se chiudi il programma dalla X in alto a destra, perderai le modifiche che effettui nel " );
	puts( "\t\t\tprogramma, quindi ricordati di venire a salvare se vuoi che le tue modifiche siano permanenti.\n\n" );

	//in base alla scelta l'utente può:...
	switch( inputIntero(1, 4, "\t Input -> ") ){
		case 1:
		{
			puts( "\n\n\t> Effettua l'accesso:\n");

			accediUtente( accounts, apps );//accedere allo store

			break;
		}
		case 2:
		{
			puts( "\n\n\t> Registrati:\n");

			if( registraUtente( accounts ) == 1 ){//registrarsi

				puts("\n\t Utente registrato con successo! Ora effettua l'accesso per entrare nel tuo profilo");


			} else puts("\n Registrazione fallita!\n\n\n");

			getch();
			break;
		}
		case 3:
		{
			puts( "\n\n\t> Accedi come amministratore:");

			accediAdmin( accounts, apps);//accedere come admin

			getch();
			break;
		}
		case 4://fare il logout e salvare le modifiche fatte
		{
			printf("\n\t Uscendo... Salvare le modifiche?. (Y-N)->");
			if ( tolower( getche() ) == 'y' ){


				if ( applicaModifiche( fileAccount, accounts, fileApp, apps ) != -1 ) puts("\n\n\t Modifiche salvate correttamente.");
				else puts("\n\n\t Errore nel salvataggio delle modifiche");

			} else {

				if ( ripristinaCronologiaOriginale() == -1 ) puts("\n\n\t Errore nell'annullamento delle modifiche");
				else puts("\n\n\tModifiche annullate con successo");

			}

			return 0;
			break;
		}
	}
	return 1;
}


/*
 * ADMIN
 */

/* Menu relativo a tutte le funzioni che l'amministratore ha a disposizione per gli accounts.*/
int menuAdminApps( App apps[] )
{
	system("cls");

	puts(" Posizione: Menu di Accesso / Menu Admin / Apps\n");

	//visualizzo all'admin cose può fare con le apps

	puts( "\n\n\t Scegli la funzione che vuoi usare:\n" );

	puts( "\t [1] Aggiungi App" );
	puts( "\t --------------------" );
	puts( "\t [2] Apps attive" );
	puts( "\t --------------------" );
	puts( "\t [3] Indietro\n" );

	//in base alla scelta può:...
	switch( inputIntero(1, 3, "\t Input -> ") ){
		case 1:
		{

			puts( "\n\n\t> Aggiunta di una nuova App:\n");

			if ( aggiungiApp( apps ) == 0 )
				puts("\n\t Operazione fallita.");	//aggiungere un app allo store
			else
				puts("\n\t App aggiunta correttamente!");

			getch();
			break;
		}
		case 2:
		{
			if ( apps != NULL ){

				ElencoApp tutteLeApp = ricercaApp( apps, "", rNOME ); // Elenco di app contenente tutte le app

				while( menuAppTrovate( tutteLeApp, NULL , apps ) != 0 ); // Menu per quelle app

			} else {

				puts("\t Non ci sono apps nello store");
				getch();
			}
			break;
		}
		case 3:
		{
			return 0; //tornare indietro

			break;
		}
	}
	return 1;
}

/* Menu relativo a tutte le funzioni che l'amministratore ha a disposizione per gli accounts.*/
int menuAdminUtenti( Account accounts[], App apps[] )
{
	system("cls");

	puts(" Posizione: Menu di Accesso / Menu Admin / Utenti\n");

	//visualizzo all'admin cose può fare con gli utenti

	puts( "\n\n\t Scegli la funzione che vuoi usare:\n" );

	puts( "\t [1] Aggiungi Utente" );
	puts( "\t --------------------" );
	puts( "\t [2] Utenti attivi" );
	puts( "\t --------------------" );
	puts( "\t [3] Indietro\n" );

	//in base alla scelta può:...
	switch( inputIntero(1, 4, "\t Input -> ") ){
		case 1:
		{
			puts( "\n\n\t> Aggiunta di un Utente:\n");

			if( registraUtente( accounts ) == 1 ) {			//aggiungere un utente allo store

				puts("\n\t Utente registrato con successo!");

			} else puts("\n\t Aggiunta dell'utente fallita!\n\n\n");

			getch();
			break;
		}

		case 2:
		{

			if ( accounts != NULL ) menuTuttiGliAccount( accounts, apps ); //visualizzare tutti gli utenti attivi nello store
			else{
				puts("\t Non ci sono utenti iscritti");
				getch();
			}

			break;
		}

		case 3:
		{
			return 0;//tornare indietro
			break;
		}

	}
	return 1;
}

/* Menu in cui l'amministratore sceglie se lavorare sugli account oppure sulle app.*/
int menuAdmin( Account accounts[], App apps[] )
{
	system("cls");

	puts("\n\n ************HAI EFFETTUATO L'ACCESSO COME AMMINISTRATORE DEL SERVER************");

	//visualizzo all'admin quali dati gestire

	puts("\n\n\t Su cosa vuoi agire?:\n");

	puts( "\t [1] Apps" );
	puts( "\t --------------------" );
	puts( "\t [2] Utenti" );
	puts( "\t --------------------" );
	puts( "\t [3] LogOut\n" );

	//in base alla scelta può:...
	switch( inputIntero(1, 3, "\t Input -> ") ){
		case 1:
		{
			while( menuAdminApps( apps ) != 0); //aprire il menù admin delle apps
			break;
		}
		case 2:
		{
			while( menuAdminUtenti( accounts, apps ) != 0); //aprire il menù admin degli utenti
			break;
		}
		case 3:
		{
			printf("\n\n\t Conferma con Y, altro per annullare ->"); // eseguire il logout
			if ( tolower( getche() ) == 'y' ) return 0;
			break;
		}
	}
	return 1;
	
}





/*
 *  INTRO
 */

/* Visualizza la schermata iniziale di benvenuto nel programma.*/
void stampaIntro( char path[] )
{
	if ( isFileExists( path )){//se il file di intro esiste...

		FILE* logo = fopen( path, "r");//apro il file in modalità lettura
		rewind( logo );//riporto il puntatore ad inizio file
		char riga = fgetc( logo );//leggo un carattere dal file

		while( !feof( logo ) ){//fin quando non sono a fine file

			riga = fgetc( logo );//leggo un carattere da file
			printf("%c", riga );//e lo stampo
		}

	}
	printf("\n\n\n\n\n\t Developed By - Francesco Soprano & Michele Perilli");
	printf("\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t\t\t\t\t\t\t\t Premi un tasto per entrare...");

}
