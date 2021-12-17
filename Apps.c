/*
 * Apps.c
 *
 *
 *      Author: FRANCESCO SOPRANO and MICHELE PERILLI
 */

#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "Apps.h"
#include "Tools.h"
#include "Cronologia.h"
#include "Interfaccia.h"
#include "Account.h"



/*
 * FUNZIONI PER LA GESTIONE APPS
 */

/*Apre il file binario delle app, risolvendo eventuali problemi come l'assenza stessa del file.*/

FILE* apriAppsSP()
{
	FILE* fileApp = NULL; // Dichiaro e inizializzo il puntatore al file che restituiro'

	if ( isFileExists( APPS_SP ) )
		fileApp = fopen( APPS_SP, "rb+");		// Se il file Apps.sp ad accesso casuale esiste lo apro
	else
		fileApp = creaAppSP();					// altrimenti lo ricreo leggendo dal file testuale di backup

	return fileApp; // Ritorno il puntatore al file
}



/*Leggendo dal file di backup delle app, crea un file ad accesso casuale per le apps.*/

FILE* creaAppSP()
{
	App record; // Record temporaneo per contenere un'App
	azzeraApp( &record );


	FILE* fileApp_bak = fopen( APPS_BAK,"r" );	// poi apre in lettura il file testuale di backup
	FILE* fileApp_sp;

	if (fileApp_bak == NULL){
		/* Se il file testuale di backup non esiste, ritorno NULL */

		fileApp_sp = NULL;

		puts("App.bak non trovato.");

	} else {
		/* Altrimenti porto in cima il puntatore di riga e inizio a leggere */
		fileApp_sp = fopen( APPS_SP, "wb+" ); // Creo il file Apps.sp, se esiste lo azzera,

		rewind( fileApp_bak );
		rewind( fileApp_sp );

		while( !feof( fileApp_bak ) ){
			/* Finchè non siamo a fine file...
			 * - Leggo una riga (sapendo che la fgets si ferma al '\n' non andrò sicuramente a leggere oltre il record selezionato)
			 * - Uso la funzione strtok per spezzettare la mia riga letta in token, passando come parametro quale debba essere il separatore
			 * - Sapendo che strtok restituisce il puntatore al primo token creato, inizio a copiarlo nel campo nome, per poi riusare strtok con campo nullo
			 *   per passare al token successivo, che a sua volta verrà copiato in cognome con strcpy... e così via fino all'ultimo campo del record letto.
			 * - p.s. atoi converte da stringa a intero, atof da stringa a float*/

			char riga[DIMENSIONE_RIGA_DA_LEGGERE];
			fgets( riga, DIMENSIONE_RIGA_DA_LEGGERE, fileApp_bak);

			char* token  = strtok( riga, ";" );
			strcpy( record.nome, token );

			token = strtok(NULL, ";");
			record.categoria = ( Categoria ) atoi( token );

			token = strtok(NULL, ";");
			record.costo = atof( token );

			token = strtok(NULL, ";");
			record.dimensione = atof( token );

			token = strtok(NULL, "\n");
			strcpy( record.sviluppatore, token);

			record.isActive = true;

			record.totDownload = 0;
			record.totVoti = 0;
			record.sommaVoti = 0;

			/* Una volta ottenuto il record completo nella nostra struct, passiamo alla scrittura sul file binario: */
			fwrite( &record, sizeof( App ), 1, fileApp_sp );
		}
	}

	fclose( fileApp_bak ); // Finite le operazioni chiudo il file di backup
	return fileApp_sp; 	   // e restituisco il puntatore a App.sp
}



/* Questa funzione si occupa della popolazione del vettore di account*/

void inizializzaApps( FILE* file, App apps[] )
{
	/* Riporto in cima il puntatore di riga del file */
	rewind( file );

	/*pulisco tutto il vettore delle Apps*/
	for (unsigned int i = 0; i < MASSIMO_NUMERO_ACCOUNT; i++)
		azzeraApp(&apps[i]);

	/* Continua a legge la riga successiva nel file, la salva nel vettore dato nel paramentro
	 *  e incrementa l'indice fino a quando è vera la condizione del while*/
	for ( unsigned int i = 0; !feof( file ) && i < MASSIMO_NUMERO_APP; i++ ){
		fread( &apps[i], sizeof(App), 1, file );
	}
}



/* Conta quanti sono i record di app che sono effettivamente attive, cioe' escluso le
 * app cancellate dall'amministratore */

int contaApp ( const App apps[], char* chiave, TipoRicerca tipo )
{
	int app = 0;

	switch( tipo ){ // In base a che tipo di ricerca sceglie, proseguo diversamete:

		case rNOME: // Ricerca per nome: il parametro chiave sara' quindi un nome inserito dall'utente
		{
			for( unsigned int i = 0; i < MASSIMO_NUMERO_APP; i++ ){ // Per ogni app..

				// Se trovo un'app che ha il parametro chiave come nome ( completo o in parte ) e se l'app non e' stata eliminata dall'admin..
				if ( strncmp( toLower( chiave ), toLower ( apps[i].nome ), strlen( chiave ) ) == 0 && apps[i].isActive )
					app++;

			}
			break;
		}
		case rSVILUPPATORE: // Ricerca per sviluppatore: il parametro chiave sara' quindi uno sviluppatore inserito dall'utente
		{
			for( unsigned int i = 0; i < MASSIMO_NUMERO_APP; i++ ){ // Per ogni app..

				// Se trovo un'app che ha il parametro chiave come sviluppatore ( completo o in parte ) e se l'app non e' stata eliminata dall'admin..
				if ( strncmp( toLower( chiave ), toLower ( apps[i].sviluppatore ), strlen( chiave ) ) == 0 && apps[i].isActive )
					app++;

			}
			break;
		}
		case rGENERE: // Ricerca per genere: il parametro chiave sara' quindi un genere inserito dall'utente
		{
			for( unsigned int i = 0; i < MASSIMO_NUMERO_APP; i++ ){ // Per ogni app..

				// Se trovo un'app che ha il parametro chiave come genere ( completo o in parte ) e se l'app non e' stata eliminata dall'admin..
				if ( ( stringToCategoria( chiave ) == apps[i].categoria ) && apps[i].isActive )
					app++;

			}
			break;
		}
	}


	return app; // Ritorno il contatore in cui ho tenuto traccia di quante app ho trovato

}



/*Pulisce il record relativo ad una determinata app.*/

void azzeraApp ( App* app )
{
	app->isActive = false; // Rendo l'app inattiva

	// Pulisco le sue stringhe
	clearString(app->nome, LUNGHEZZA_NOME);
	clearString(app->sviluppatore, LUNGHEZZA_COGNOME);

	app->costo = 0.0f; 		//
	app->dimensione = 0.0f; //
	app->totVoti = 0; 		// Inizializzo i valori a 0;
	app->totDownload = 0; 	//
	app->sommaVoti = 0; 	//

	app->categoria = -1; // Setto la Categoria con un valore fuori dal range, lo 0 rappresenterebbe Azione.
}



/* Aggiunge una nuova app al vettore di app passato come parametro. Andra' a buon fine solo se l'input dei dati hanno esito positivo
 * e se non esiste un altra app con lo stesso nome. */

int aggiungiApp( App apps[] )
{
	App app; // Variabile App temporanea in cui salverò i dati dell'app da aggiungere
	App* recordLibero = cercaRecordLiberoinApps( apps ); // Creo un puntatore al primo record vuoto presente in apps, in cui andro' a mettere la nuova app che inserisce l'admin.

	azzeraApp( &app ); // Mi inizializzo la variabile che conterra' il record effettivo da scrivere in apps.

	bool esito = true; // Variabile in cui conterro' l'esito dell'operazione ( positivo o negativo ) che ritornero'

	if ( recordLibero == NULL ){
		// Se cercaRecordLiberoinApps restituisce NULL vuol dire che non c'e' piu' spazio nel vettore, quindi avviso l'utente dell'inconveniente e ritorno 0 (errore).

		puts("\n\n  Siamo spiacenti per l'inconveniente, ma non c'è più spazio libero per accogliere nuove app.");

		esito = false;

	} else {
		// Se ho trovato un record disponibile che accolga la nuova app, allora inizio con chiedere gli input.

		azzeraApp( recordLibero );  // Pulisco il record libero trovato.
		azzeraApp( &app );			// e il record di appoggio

		strcpy( app.nome, inputStringa( LUNGHEZZA_TITOLO, "\t Nome: " ) );// Inizio con gli input del nome dell'app


		strcpy( app.sviluppatore, inputStringa( LUNGHEZZA_NOME_SVILUPP, "\t Sviluppatore: " ) );	// e del nome dello sviluppatore.


		if ( app.nome[0] != '\0' && app.sviluppatore[0] != '\0' ){ // Nel caso in cui entrambi gli input siano stati effettuati..

			app.categoria = inputCategoria( "\t Categoria: " );													// Prendo l'input del genere
			printf("\t\t La categoria associata alla tua app e': %s\n", categoriaToString( app.categoria ) );		// dell'app che vuole caricare.


			app.dimensione = inputFloat( 0, DIMENSIONE_MASSIMA_APP, "\t Dimensione (MegaByte): " );	// Per poi chiedere anche
			app.costo = inputFloat( 0, COSTO_MASSIMO_APP, "\t Costo: " );							// dimensione e costo



			if ( ricercaAppIntegrale( apps, app.nome ) == NULL ){
				// Se il nome app non e' stato gia utilizzato, rendo l'app attiva e copio l'app nel record libero trovato in precedenza.
				app.isActive = true;

				*recordLibero = app;

			} else {
				// Se invece il nome e' gia stato usato in un'altra app, non posso proseguire e ritornero' un esito negativo avvisando l'utente.
				puts("Il nome di questa app e' gia' presente nel nostro database!");
				esito = false;
			}

		} else {
			// Se gli input del nome e dello sviluppatori non sono stati entrambi eseguiti ritornero' un esito negativo avvisando l'utente.
			puts("\n\t Assicurati che non ci siano campi vuoti.");
			 esito = false;
		}

	}

	return esito;
}



/* Permette all'utente dato come parametro di installare l'app data come parametro se isInstalled indica come non installata, altrimenti
* la disinstalla. Durante l'installazione viene presa in considerazione la memoria disponibile dell'utente e del credito. Se la memoria
* disponibile non e' abbastanza, mostro tutte le app che ha installato cosi, disinstallandone q.b. potra' continuare l'installazione.
* Se il credito non e' sufficente verra' effettuata una richiesta di ricarica dello stesso importo del prezzo dell'app.*/

void installaApp( const App* app, Account* utente, bool isInstalled, App apps[] )
{
	ElencoCronologia lista = getCronologiaUtente( utente ); // Estraggo la lista delle associazioni alle app di questo utente dal file cronologia.
	int riga = cercaAppInElencoCronologia( app, lista ); // e cerco l'app data dal parametro in quella lista, salvandomi la sua posizione nel file in riga.

	if ( isInstalled == false ){
		/* Se l'app non risulta essere attualmente installata: */

		bool continuare = true;
		if ( utente->memoriaDisponibile < app->dimensione && continuare == true){
			// Se l'utente non ha abbastanza memoria per installare l'app..

			// Chiedo se vuole effettuare una disinstallazione, che continuero' a chiedere fino a che l'utente non decide di smettere di disinstallare.
			printf("\n\t Memoria insufficiente. Ti servono %.2f MB, vuoi disinstallare qualcosa? (Y-N) ", ( app->dimensione - utente->memoriaDisponibile  ) );

			if ( tolower( getch() == 'y' ) ){

				while( menuLeMieApp( lista, utente, apps ) != 0 ); // Se vuole continuare a disinstallare, mostro le sue app.

			} else continuare = false; // Altrimenti non continuo e impedisco l'installazione.

		} else {
			// Invece se ha abbastanza memoria
			Cronologia record; 					// Dichiaro e inizializzo la variabile che conterra'
			clearRecordCronologia( &record );	// il record da scrivere sul file cronologia.

			if ( riga == -1 ) {
				/* Se l'utente non ha mai avuto quest' app: */

				if ( paga( utente, app->costo ) == 0 ) { // Se il pagamento va a buon fine ( se l'app e' gratis e' considerato come pagare 0 euro )..

					strcpy( record.email, utente->credenziali.email ); 	//
					strcpy( record.nomeApp, app->nome );				// Riempio la variabile di appoggio e aggiorno il file
					record.isInstalled = true;							// cronologia segnando l'app come installata.
					aggiungiCronologia( record );						//

					utente->memoriaDisponibile -= app->dimensione;		// Poi decremento la memoria disponibile

				} else {
					puts("\n\t Installazione fallita, credito non sufficente.");
					getch();
				}

			} else {
				/* altrimenti se ha gia avuto quest' app in passato, ma non ora: */

				record = leggiCronologia( lista.posizioni[riga] );	//
				record.isInstalled = true;							// Leggo il record, lo modifico e lo riscrivo sul file
				modificaCronologia( record, lista.posizioni[riga] );//

				utente->memoriaDisponibile -= app->dimensione;		// Poi decremento la memoria disponibile

			}
		}

	} else {
		/* invece se l'app risulta essere attualmente installata: */

		Cronologia record = leggiCronologia( lista.posizioni[riga] );	// La disinstallo leggendo il record,
		record.isInstalled = false;										// cambiando il campo giusto
		modificaCronologia( record, lista.posizioni[riga] );			// e riscrivendo sul file

		utente->memoriaDisponibile += app->dimensione;					// Poi incrementro la memoria disponibile

		printf("\n\t Hai liberato %.2f MB\n", app->dimensione );
		puts("\n\t Premi un tasto per tornare indietro.");
		getch();
	}
}



/* Scambia i due puntatori passati come parametro attraverso un passaggio per riferimento.*/

void scambiaApp ( App** campo1, App** campo2 )
{
	App* temp; // Puntatore di appoggio

	temp = *campo1;		//
	*campo1 = *campo2;	// Scambio i puntatori di app usando il puntatore di appoggio
	*campo2 = temp;		//
}



/* Dato un Elenco di app, lo ordina seguendo il criterio dato dal parametro TipoOrdinamento. La funzione utilizza il Selection Sort come algoritmo di base*/

void ordinaApp ( ElencoApp elenco, TipoOrdinamentoApp tipo )
{
	if ( elenco.lunghezza > 1){ // Proseguo con l'orinamento soltanto se c'e' piu di un elemento nel vettore.

		// Si e' utilizzato  il Selection Sort
		for( unsigned int i = 0; i < elenco.lunghezza - 1; i++){ // Scorrendo gli ementi
			unsigned int pos_min = i; // Mi salvo quale sia la posizione il cui andra' l'elemento piu' "piccolo"

			for( unsigned int j = i + 1; j < elenco.lunghezza ; j++){ // Scorro tutti gli elementi dalla posizione del minimo alla fine

				switch ( tipo ){
					// E in base a che tipo di ordinamento trovo il minimo e lo metto nella posizione minima.

					case oNOME:
					{
						if( strcmp( toLower( elenco.vettoreApp[j]->nome ), toLower( elenco.vettoreApp[pos_min]->nome ) ) < 0 )
							pos_min = j;
						break;
					}
					case oDOWNLOAD:
					{
						if ( elenco.vettoreApp[j]->totDownload > elenco.vettoreApp[pos_min]->totDownload )
							pos_min = j;
						break;
					}
					case oVOTO:
					{
						if ( mediaVoti( elenco.vettoreApp[j]->sommaVoti, elenco.vettoreApp[j]->totVoti ) > mediaVoti( elenco.vettoreApp[pos_min]->sommaVoti, elenco.vettoreApp[pos_min]->totVoti ) )
							pos_min = j;
						break;
					}
					case oSVILUPPATORE:
					{
						if( strcmp( toLower( elenco.vettoreApp[j]->sviluppatore ), toLower( elenco.vettoreApp[pos_min]->sviluppatore ) ) < 0  )
							pos_min = j;
						break;
					}
					case oGENERE:
					{
						if( strcmp( toLower( categoriaToString( elenco.vettoreApp[j]->categoria) ), toLower( categoriaToString( elenco.vettoreApp[pos_min]->categoria ) ) ) < 0  )
							pos_min = j;
						break;
					}
					case oPREZZO:
					{
						if ( elenco.vettoreApp[j]->costo < elenco.vettoreApp[pos_min]->costo )
							pos_min = j;
						break;
					}
					case oDIMENSIONE:
					{
						if ( elenco.vettoreApp[j]->dimensione < elenco.vettoreApp[pos_min]->dimensione )
							pos_min = j;
						break;
					}
				}

			}

			// Identificati gli indici della posizione in cui andra' il minimo e l'indice dell'elemento minimo, scambio gli elementi.
			scambiaApp( &elenco.vettoreApp[pos_min], &elenco.vettoreApp[i] );
		}
	}
}



/* Utilizzando il vettore di Apps, riscrivo il fileApp con quei dati, cosi da effettuare il salvataggio dei dati su disco.*/

int aggiornaFileApp( FILE* fileApp, App apps[] )
{
	if ( fileApp != NULL ){
		// Se il file esiste..

		rewind(fileApp);//riporto ad inizio file il puntatore

		for (unsigned int i = 0; i < MASSIMO_NUMERO_APP; i++ )
			// Scorro tutte le app e riscrivo il fileApp con i valori presenti nel vettore
			fwrite( &apps[i], sizeof( App ) , 1, fileApp );


	} else return 1; // Se il file non esiste ritorno 1 ( errore )

	return 0;// Aggiornamento del file avvenuto correttamente.
}






/*
 * FUNZIONI DI INPUT
 */

/* Chiede l'input di una categoria di app fino a che non trova una corrispondenza. */

Categoria inputCategoria( const char* testo )
{

	Categoria input = -1;

	char stringa[ LUNGHEZZA_CATEGORIA ];

	do
		// Chiedo in input una stringa e tento la conversione in genere. Se va a buon fine ritorno la categoria trovata,
		// Altrimenti ripeto l'operazione.

		input = stringToCategoria( strcpy( stringa, ( char* ) inputStringa( LUNGHEZZA_CATEGORIA, "\t Genere: " ) ) );

	while ( input == -1 || stringa[0] == '\0' || stringa[0] == '\n' ); // Ripeto se l'input non permette di associare nessuna categoria, oppure se l'input e' vuoto.

	return input;
}



/* Posizionandoci sul file Cronologia in posizione di riga, l'app in quella posizione ricevera' un voto che verra' chiesto in input*/

void lasciaUnVoto( int riga )
{
	// Leggo il record in posizione 'riga' da file cronologia.
	Cronologia temp = leggiCronologia( riga );

	// Modifico il campo relativo al voto chiedendone uno in input.
	temp.voto = (Voto) inputIntero(vNOPE, VERYGOOD, "\t Inserisci il tuo voto [1-5] -> ");

	// Riscrivo il record modificato sul file nella stessa posizione di prima.
	modificaCronologia( temp, riga );
}






/*
 * FUNZIONI DI OUTPUT
 */

/* Visualizza le informazioni dettagliate dell'app. */

void infoApp( const App* app )
{
	system("cls"); // Pulisco la schermata della console e stampo ogni campo dell'app passata come parametro

	puts(" Posizione: Menu di Accesso / Menu Utente / InfoApp");

	puts("\n\n ***** Dettagli app: *****\n\n");

	printf( "\t %s %s\n\n" , 		"Nome..................", app->nome );

	printf( "\t %s %.2f\n\n" , 		"Voto..................", mediaVoti( app->sommaVoti, app->totVoti) );

	printf( "\t %s %d\n\n" , 		"Downloads..............", app->totDownload);

	printf( "\t %s %s\n\n" , 		"Genere................", categoriaToString( app->categoria ) );

	printf( "\t %s %.2f MB\n\n" , 	"Dimensione............", app->dimensione );

	printf( "\t %s %.2f $\n\n" ,	"Costo.................", app->costo );

	printf( "\t %s %s\n\n" , 		"Sviluppatore..........", app->sviluppatore );

	puts("\nPremi un tasto per tornare indietro");
	getch();
}



/*Stampa in modo ordinato e formattato tutte le app presenti nel vettore di app, numerandole progressivamente.*/

void stampaApps( const App apps[] )
{
	printf("\n\n\t\t  ID | %-32s | %-15s | $ %-5s |  %-30s | %-6s | %-10s | %-8s", "NOME", "GENERE", "COSTO", "SVILUPPATORE", "DIMENSIONE","DOWNLOAD", "VOTO MEDIO");
	printf("\n\t\t --------------------------------------------------------------------------------------------------------------------------------------------");

	for (unsigned int i = 0; i < MASSIMO_NUMERO_APP; i++){ // Per ogni elemento del vettore di app..

		if ( apps[i].isActive ){ // Se l'app e' attiva, cioe' non eliminata dall'admin, la stampo.

			printf(" [%d]", i);
			printf(" %-35s | %-20s | $ %-7.2f |  %-20s ", apps[i].nome, categoriaToString( apps[i].categoria ), apps[i].costo, apps[i].sviluppatore);
			printf("\n -----------------------------------------------------------------------------------------\n");

		}

	}
}



/* Stampa in modo ordinato e formattato tutte le app presenti nell'elenco di app, numerandole progressivamente.*/

void stampaElencoApp( const ElencoApp elencoApp )
{

	printf("\n\n\t  ID | %-32s | %-15s | $ %-5s |  %-30s | %-6s | %-10s | %-8s", "NOME", "GENERE", "COSTO", "SVILUPPATORE", "DIMENSIONE","DOWNLOAD", "VOTO");
	printf("\n\t --------------------------------------------------------------------------------------------------------------------------------------");

	for ( int i = 0; i < elencoApp.lunghezza; i++){

		// Per avere un output formattato correttamente se l'indice dell'app
		switch ( contaCifre( i ) ){
			case 1:
				printf( "\n\t   [%d]", i ); // Se e' a una cifra metto tre spazi
				break;
			case 2:
				printf( "\n\t  [%d]", i ); // Se e' a due cifre metto due spazi
				break;
			case 3:
				printf( "\n\t [%d]", i ); // altrimenti uno spazio
				break;
		}

		//stampo l'elenco delle app
		printf(" %-32s | %-15s | $ %-5.2f |  %-30s |  %-6.2f MB |    %-7d | %-6.2f", elencoApp.vettoreApp[i]->nome,
																					categoriaToString( elencoApp.vettoreApp[i]->categoria ),
																					elencoApp.vettoreApp[i]->costo,
																					elencoApp.vettoreApp[i]->sviluppatore,
																					elencoApp.vettoreApp[i]->dimensione,
																					elencoApp.vettoreApp[i]->totDownload,
																					mediaVoti( elencoApp.vettoreApp[i]->sommaVoti, elencoApp.vettoreApp[i]->totVoti ) );
		printf("\n\t --------------------------------------------------------------------------------------------------------------------------------------");
	}
}






/*
 * FUNZIONI PER LA RICERCA
 */

ElencoApp ricercaAppTempoReale( const App apps[], TipoRicerca tipo )
{
	char chiave[LUNGHEZZA_CHIAVE_RICERCA + 1];			// La chiave di ricerca avra' dimensione
	clearString( chiave, LUNGHEZZA_CHIAVE_RICERCA + 1 );//

	system("cls");
	printf(" Dove ti trovi: Menu di Accesso / Menu Utente / Ricerca\n\n\tRicerca App in tempo reale: ");
	puts("\n\n\t(Invio per fermarti con la ricerca e interagire con i risultati)\n\n");
	printf("\n\t Chiave di ricerca ->");


	int i = 0;
	char input;

	while( ( input = getch() ) != '\r' ){
		// Continuo con l'inserimento fin quando non preme invio..


		if ( input != FUNC_BUTTONS_1 && input != FUNC_BUTTONS_2 ){
			// Se non ha inserito caratteri troppo particolari o tasti funzione, continuo..

			if ( input != ESC && input != TAB ){
				// Se l'input e' diverso dal tasto ESC e dal tasto TAB, in base all'input aggiorno la chiave di ricerca..

				if ( input == '\b' ) {
					// Se ha premuto backspace, dovrò cancellare il carattere precedentemte inserito, ma soltanto se ha gia inserito qualcosa.
					if ( i != 0 ) chiave[--i] = '\0';

				} else {
					// Se non ha premuto backspace, continuo..

					if ( i < LUNGHEZZA_CHIAVE_RICERCA ){
						// Se non sta andando oltre il limite massimo della chiave di ricerca..

						if ( i == 0 && input == ' ' ){
							// Impedisco che possa mettere uno spazio come primo carattere: invaliderebbe la ricerca
							// e sara' molto difficile che si accorga dello spazio, quindi evito che possa metterlo.

						} else {
							// Altrimenti, il carattere appena dato in input lo metto dentro la chiave di ricerca
							// in posizione giusta, per poi incrementare l'indice di posizione.
							chiave[i++] = input;
						}

					} //puts("\n\t\t Non puoi superare il limite massimo di caratteri per la ricerca di %d caratteri :(", LUNGHEZZA_CHIAVE_RICERCA );

				}

				system("cls");
				puts(" Dove ti trovi: Menu di Accesso / Menu Utente / Ricerca\n\n\t(Invio per fermarti con la ricerca e interagire con i risultati)\n\n");

				printf("\n\t Chiave di ricerca -> %s", chiave); // Stampiamo ogni volta la chiave completa dopo aver pulito lo schermo

				if ( i > 0 ) {
					// Visualizzo i risultati solo se ha inserito almeno qualcosa che ha prodotto dei risultati.

					ElencoApp elenco = ricercaApp( apps, chiave, tipo );
					if( elenco.lunghezza != 0)
						stampaElencoApp( elenco );

				}
			}

		} else input = getch();

	}

	// In questo punto arriviamo solo quando l'utente preme invio.

	ElencoApp elenco = { 0, NULL }; // Dichiaro e inizializzo un elenco app

	if ( i == 0 && input == '\r' ){

		// Se ha premuto direttamente invio senza inserire nulla, non faccio niente ritornando un elenco vuoto.

	} else elenco = ricercaApp( apps, chiave, tipo );

		// Altrimenti ritorno un elenco contenente quello che e' stato trovato usato la sua chiave di ricerca.
	return elenco;

}



/* Ricerca di un'app per nome preciso */

App* ricercaAppIntegrale( const App apps[], char* chiave )
{

	App* app = NULL;

	for( unsigned int i = 0; i < MASSIMO_NUMERO_APP; i++ ){
		// Scorro tutto il vettore delle app alla ricerca dell'app desiderata dall'admin

		if ( strcmp( toLower( chiave ), toLower( apps[i].nome ) ) == 0 && apps[i].isActive ){
			// Se la trovo riempio risultati con i dati dell'app cercata

			app = ( App* ) &apps[i]; // Assegno l'indirizzo al puntatore ad app da restituire

			i = MASSIMO_NUMERO_APP; // Manipolo l'indice cosi da fermare il ciclo

		}

	}

	return app;
}



/* Dato l'Elenco delle app, ricerco un app scegliendo il tipo di ricerca( titolo, sviluppatore, genere) */

ElencoApp ricercaApp( const App apps[], char* chiave, TipoRicerca tipo )
{

	int appTrovate = contaApp( apps, chiave, tipo ); // Conto le app in base alla ricerca fatta

	ElencoApp risultati;
	risultati.lunghezza = appTrovate;

	if ( appTrovate == 0 )
		// Se non sono state trovate apps il vettore ritornato sara' nullo
		risultati.vettoreApp = NULL;
	else {
		// Altrimenti procedo con l'allocazione e la ricerca
		risultati.vettoreApp = ( App** ) calloc( risultati.lunghezza, sizeof( App ) );

		switch( tipo ){
			// In base al tipo richiesto, si procede diversamente:

			case rNOME:// ricerca per nome anche per sottostringa
			{
				/*Scorro tra tutte le app attive nello store e con la strncmp comparo la chiave di ricerca con il titolo delle app attive
				 * p.s. toLower serve per matchare le apps anche se ricerco in maiuscolo*/
				for( unsigned int i = 0, j = 0; i < MASSIMO_NUMERO_APP; i++ ){
					if ( strncmp( toLower( chiave ), toLower( apps[i].nome ), strlen( chiave ) ) == 0 && apps[i].isActive ){
						risultati.vettoreApp[j] = ( App* ) &apps[i];//riempio il vettore con le app trovate
						j++;
					}
				}
				break;
			}
			case rSVILUPPATORE:// ricerca per sviluppatore anche per sottostringa
			{
				/*Scorro tra tutte le app attive nello store e con la strncmp comparo la chiave di ricerca con lo sviluppatore delle app attive
				 * p.s. toLower serve per matchare le apps anche se ricerco in maiuscolo*/
				for( unsigned int i = 0, j = 0; i < MASSIMO_NUMERO_APP; i++ ){
					if ( strncmp( toLower( chiave ), toLower( apps[i].sviluppatore ), strlen( chiave ) ) == 0 && apps[i].isActive ){
						risultati.vettoreApp[j] = ( App* ) &apps[i];//riempio il vettore con le app trovate
						j++;
					}
				}
				break;
			}
			case rGENERE:// ricerca per genere anche per sottostringa
			{
				/*Scorro tra tutte le app attive nello store e con la strncmp comparo la chiave di ricerca con i generi delle app attive
				 * p.s. stringToCategoria converte la ricerca in un valore Categoria che è un enum*/
				for( unsigned int i = 0, j = 0; i < MASSIMO_NUMERO_APP; i++ ){
					if ( ( stringToCategoria( chiave ) == apps[i].categoria ) && apps[i].isActive ){
						risultati.vettoreApp[j] = ( App* ) &apps[i];//riempio il vettore con le app trovate
						j++;
					}
				}
				break;
			}
		}
	}


	return risultati;
}



/* Estrae le applicazioni di un utente casuale avente la fascia di eta' data come parametro, e le usa per costruire un elenco
 * usato poi per consigliarle all'utente */

ElencoCronologia consigliaAppPerEta( const Account accounts[], const Maturita fasciaEta )
{

	unsigned int numeroUtentiSimili = 0;

	ElencoCronologia cronologiaTemp = { 0, NULL };

	// Conto gli utenti simili, cioe' con la stessa fascia di eta'
	for( unsigned int i = 0; i < MASSIMO_NUMERO_ACCOUNT; i++ ){
		if ( accounts[i].fasciaEta == fasciaEta ) numeroUtentiSimili++;
	}


	if ( numeroUtentiSimili != 0 ){
		// Se trova utenti simili, creo un vettore di interi per le posizioni degli utenti simili

		unsigned int utentiSimili[numeroUtentiSimili];
		unsigned int utentiTrovati = 0;

		for( unsigned int i = 0; i < MASSIMO_NUMERO_ACCOUNT; i++ ){
			// Per ogni account..

			if ( accounts[i].fasciaEta == fasciaEta ){
				utentiSimili[utentiTrovati] = i; // Li aggiungo al vettore salvandomi il loro indice se hanno la stessa fascia di eta'
				utentiTrovati++;
			}

		}

		do {
			// Poi, una volta avuto il vettore di interi contenente le posizioni degli utenti simili, ne sorteggio uno e estraggo la sua cronologia.

			int indiceUtenteSimileCasuale = utentiSimili[ genRandom( 0, numeroUtentiSimili - 1 ) ];

			cronologiaTemp = getCronologiaUtente( &accounts[ indiceUtenteSimileCasuale ] ); // Assegno la cronologia di utente random che ha la sua stessa maturita

		} while ( cronologiaTemp.lunghezza == 0 ); // Se non ha nessuna app installata, sorteggio un altro utente fin quando non ne trovo uno.
	}

	return cronologiaTemp; // Ritorno la cronologia dell'utente simile
}



/* Avendo il vettore di frequenze delle categorie relativamente ad un utente, costruisce un ElencoApp contenente al massimo 3 app con voti migliori
 *  per ogni categoria che appare almeno una volta tra le sue app, cosi da consigliargli app simili alle sue*/

ElencoApp consigliaAppPerCategoria( const int frequenzaCategorie[], const App apps[] )
{
	ElencoApp appTrovate = { 0, NULL };

	for ( unsigned int i = 0; i < NUMERO_CATEGORIE; i++ ){
		// Per ogni categoria..

		if ( frequenzaCategorie[i] > 0) {
			// Se la i-esima cella della frequenza è maggiore di 0 vuol dire che ha installato almeno un'app di quella categoria

			if ( ricercaApp( apps, categoriaToString(i), rGENERE ).lunghezza < NUMERO_APP_CONSIGLIATE_PER_CATEGORIA )
				// se ci sono meno del numero delle app da consigliare aumento la lunghezza per quante app ci sono
				appTrovate.lunghezza += ricercaApp( apps, categoriaToString(i), rGENERE ).lunghezza ;
			else
				// altrimenti aumento la lunghezza del massimo delle app da consigliare per categoria
				appTrovate.lunghezza += NUMERO_APP_CONSIGLIATE_PER_CATEGORIA;

		}
	}

	if( appTrovate.lunghezza != 0 ){
		// Se non ha 0 app installate..

		appTrovate.vettoreApp = ( App** ) calloc( appTrovate.lunghezza, sizeof( App ) );

		unsigned char k = 0; // Indice usato per tenere traccia di dove siamo arrivati nel salvare le app nell'ElencoApp

		for ( unsigned int l = 0; l < NUMERO_CATEGORIE; l++ ){
			// Per ogni categoria..

			ElencoApp ricerca = ricercaApp( apps, categoriaToString( l ), rGENERE ); // Effettuo la ricerca per genere usando la categoria in questione

			ordinaApp( ricerca, oVOTO ); // Le ordino per voto

			for ( unsigned int i = 0; i < NUMERO_APP_CONSIGLIATE_PER_CATEGORIA && i < ricerca.lunghezza; i++ ){
				// Poi ciclo fino al minimo tra il numero dei risultati e il numero massimo di app da consigliare

				if ( frequenzaCategorie[l] != 0 ){
					// se nel vettore frequenze risulta che ha almeno una app di quel genere

					appTrovate.vettoreApp[k++] = ricerca.vettoreApp[i]; // Copio in appTrovate le app di quel genere

				}

			} // end for i

		} // end for l

	} // Se ha 0 app installate ritornera' l'elenco vuoto.

	return appTrovate;
}



//Prende il vettore delle Apps e restituisce le ultime apps aggiunte

ElencoApp ultimeAppAggiunte(  const App apps[] )
{
	ElencoApp risultati = { 0 , NULL }; // Azzero risultati

	unsigned int numeroApps = contaApp( apps, "", rNOME ); // Conto tutte le apps presenti nello store


	if ( numeroApps == 0 ) {

		// Se non ci sono apps aggiunte nello store ritornera' l'elenco vuoto

	} else {

		risultati.lunghezza = ULTIME_APP_AGGIUNTE; // Assegno alla lunghezza di risultati il valore delle ultime app aggiunte che voglio ritornare

		risultati.vettoreApp = ( App** ) calloc( risultati.lunghezza, sizeof( App ) );

		for ( unsigned int i = numeroApps - 1, j = 0; i >= numeroApps - ULTIME_APP_AGGIUNTE; i-- ){
			// Partendo dalla fine delle apps torno indietro fino a quante app devo visualizzare nella top e assegno al vettore di risultati l'app trovata

			risultati.vettoreApp[j++] = ricercaApp( apps, "", rNOME ).vettoreApp[i];

		}

	}

	return risultati;

}



// Cerca il primo record libero nel vettore delle apps e ne restituisce il puntatore

App* cercaRecordLiberoinApps( App apps[] )
{
	int indiceRecordLibero = -1;

	for ( unsigned int i = 0; i < MASSIMO_NUMERO_APP; i++) {
		/*Scorro il vettore delle apps, cerco la prima app che ha il campo isActive settato a falso,
		 * se lo trovo esco dal ciclo e ritorno l'indirizzo del record libero */

		if ( apps[i].isActive == false ){

			indiceRecordLibero = i; // salvo l'indice

			i = MASSIMO_NUMERO_APP; // esco dal ciclo

		}
	}

	if ( indiceRecordLibero == -1 ) return NULL;// Se non ho trovato nessun app inattiva ritorno NULL
	else return &apps[indiceRecordLibero];		// Altrimenti ritorno l'indirizzo di quel record libero.

}






/*
 * FUNZIONI DI CONVERSIONE
 */

/* */

ElencoApp elencoCronologiaToElencoApp( ElencoCronologia elenco, const App apps[] )
{

	ElencoApp risultato;															//
	risultato.lunghezza = elenco.lunghezza;											// Dichiaro un ElencoApp di dimensione pari all'elencoCronologia
	risultato.vettoreApp = ( App** ) calloc( risultato.lunghezza, sizeof( App ) );	//


	for( unsigned int i = 0; i < risultato.lunghezza; i++ ){
		// Per ogni elemento dell'elenco cronologia..

		char nomeApp[LUNGHEZZA_TITOLO] = "";								// Estraggo il nome dell'app
		strcpy ( nomeApp, leggiCronologia( elenco.posizioni[i] ).nomeApp);	// copiandolo in una stringa di appoggio

		risultato.vettoreApp[i] = ( App* ) ricercaAppIntegrale( apps, nomeApp ); // e la uso per cercarla nel vettore di app per poi assegnare il suo puntatore al mio elencoApp

	}

	return risultato; // ritorno l'ElencoApp corrispondente all'elencoCronologia nel parametro.
}



//Prende un valore Categoria (enum) e lo trasforma in una stringa

char* categoriaToString( Categoria genere )
{
	if ( genere < ARCADE || genere > VIAGGI ) {
		//se il valore Categoria è fuori dal range dei genere delle apps ritorno NULL

		return NULL;

	} else {

		/* Matrice che contiene i nomi delle categorie delle apps corrispondenti al valore della loro enumerazione:
		 * l'indice di riga corrisponde alla stringa corrispondente alla Categoria(enum) delle apps */
		char nomi[ ][ LUNGHEZZA_CATEGORIA ] = { "Arcade", "Social", "Comunicazione", "Musica", "Strategia", "Produttivita'", "Sport", "Strumenti", "Corse",
												"Azione", "Intrattenimento", "Shopping", "Avventura", "Fitness", "Rompicapi", "Casual", "Video", "Viaggi" };

		char* stringa = ( char* ) calloc( LUNGHEZZA_CATEGORIA , sizeof( char ) );

		strcpy( stringa, nomi[ ( int ) genere ] ); // Altrimenti copio in una stringa il nome del genere corrispondente

		return stringa;

	}
}



//Prende il nome della Categoria e lo trasforma in una Categoria corrispondente( enum )

Categoria stringToCategoria ( char stringa[] )
{
	/* Matrice che contiene i nomi delle categorie delle apps corrispondenti al valore della loro enumerazione:
	 * l'indice di riga corrisponde alla stringa corrispondente alla Categoria(enum) delle apps */
	char nomi[ ][ LUNGHEZZA_CATEGORIA ] = { "Arcade", "Social", "Comunicazione", "Musica", "Strategia", "Produttivita' ", "Sport", "Strumenti", "Corse",
											"Azione", "Intrattenimento", "Shopping", "Avventura", "Fitness", "Rompicapi", "Casual", "Video", "Viaggi" };

	Categoria genere = -1;

	for ( unsigned int i = 0; i < NUMERO_CATEGORIE; i++ ){
		/* Scorro su tutte le categorie possibili e con la strncmp con la stringa da parametro, cerco la prima categoria corrispondente:
		 * se la trovo esco dal ciclo, setto con vero e ritorno la Categoria( enum ), altrimenti ritornera' -1 */
		if ( strncmp( toLower( nomi[i] ), toLower( stringa ), strlen( stringa ) ) == 0 )
		{
			genere = i;
			i = NUMERO_CATEGORIE;//esco dal ciclo
		}
	}

	return genere;
}



/* Elimina l'app passata come parametro, chiedendo una opt. Se tutto va bene ritorna 1, altrimenti ritorna 0*/

int eliminaApp( App* app )
{

	if( app != NULL && ( inputOTP() == 1 ) ){
		// Se l'app esiste e l'otp è corretto

		azzeraApp( app ); // Azzero l'app
		return 1;	// Ritorno esito positivo = eliminazione avvenuta correttamente

	} else return 0; // Ritorno esito negativo = eliminazione non avvenuta
}
