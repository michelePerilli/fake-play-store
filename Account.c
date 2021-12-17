/*
 * Account.c
 *
 *
 *      Author: FRANCESCO SOPRANO and MICHELE PERILLI
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <stdbool.h>

#include "Tools.h"
#include "Interfaccia.h"
#include "Cronologia.h"
#include "AccountHead.h"

/*
 * FUNZIONI PER LA GESTIONE ACCOUNTS
 */

/* Apre il file binario degli account, risolvendo eventuali problemi come l'assenza stessa del file.*/

FILE* apriAccountsSP()
{
	FILE* fileAccount = NULL;

	if ( isFileExists( ACCOUNTS_SP ) )
		fileAccount = fopen( ACCOUNTS_SP, "rb+");		// Se il file Accounts.sp ad accesso casuale esiste lo apro
	else
		fileAccount = creaAccountSP();					// altrimenti lo ricreo leggendo dal file testuale di backup

	return fileAccount;
}



/* Leggendo dal file di backup degli account, crea un file ad accesso casuale per gli account.*/

FILE* creaAccountSP()
{
	Account record; // Record temporaneo per contenere un Account


	FILE* fileAccount_bak = fopen( ACCOUNTS_BAK,"r" );	// poi apre in lettura il file testuale di backup
	FILE* fileAccount_sp;

	if ( fileAccount_bak == NULL ){
		/* Se il file testuale di backup non esiste, ritorno NULL */
		fileAccount_sp = NULL;

		puts("Accounts.bak non trovato.");

	} else {
		/* Altrimenti porto in cima i puntatori di riga e inizio a leggere */
		fileAccount_sp = fopen( ACCOUNTS_SP, "wb+" ); // Creo il file Accounts.sp, se esiste lo azzera,
		rewind( fileAccount_bak );
		rewind( fileAccount_sp );

		while( !feof( fileAccount_bak ) ){
			/* Finchè non siamo a fine file...
			 * - Leggo una riga (sapendo che la fgets si ferma al '\n' non andrò sicuramente a leggere oltre il record selezionato)
			 * - Uso la funzione strtok per spezzettare la mia riga letta in token, passando come parametro quale debba essere il separatore
			 * - Sapendo che strtok restituisce il puntatore al primo token creato, inizio a copiarlo nel campo nome, per poi riusare strtok con campo nullo
			 *   per passare al token successivo, che a sua volta verrà copiato in cognome con strcpy... e così via fino all'ultimo campo del record letto.
			 * - p.s. atoi converte da stringa a intero, atof da stringa a float*/

			char riga[DIMENSIONE_RIGA_DA_LEGGERE];
			fgets( riga, DIMENSIONE_RIGA_DA_LEGGERE, fileAccount_bak);

			char* token  = strtok( riga, ";" );
			strcpy( record.datiAnagrafici.nome, token);

			token = strtok(NULL, ";");
			strcpy( record.datiAnagrafici.cognome, token);

			token = strtok(NULL, ";");
			strcpy( record.credenziali.email, token);

			token = strtok(NULL, ";");
			strcpy( record.credenziali.password, token);

			token = strtok(NULL, ";");
			record.fasciaEta = (Maturita) atoi(token);

			token = strtok(NULL, ";");
			record.saldo = atof(token);

			token = strtok(NULL, "\n");
			record.memoriaDisponibile = atof(token);

			record.isActive = true;

			/* Una volta ottenuto il record completo nella nostra struct, passiamo alla scrittura sul file binario: */
			fwrite(&record, sizeof(Account), 1, fileAccount_sp );
		}

	}

	fclose( fileAccount_bak ); /*Finite le operazioni chiudo il file di backup */
	return fileAccount_sp; 	   /* e restituisco il puntatore a Account.sp */
}



/* Questa funzione si occupa della popolazione del vettore di account,
 * cosi da evitare di lavorare direttamente sul file e dare la possibilità
 * di annullare le modifiche effettuate e di facilitare il lavoro sui dati */

void inizializzaAccounts( FILE* file, Account accounts[] )
{
	/* Riporto in cima il puntatore di riga del file */
	rewind( file );

	for (unsigned int i = 0; i < MASSIMO_NUMERO_ACCOUNT; i++)
		azzeraUtente(&accounts[i]);

	/* Continua a legge la riga successiva nel file, la salva nel vettore dato nel paramentro
	 *  e incrementa l'indice fino a quando è vera la condizione del while*/
	for (unsigned int i = 0; !feof( file ) && i < MASSIMO_NUMERO_ACCOUNT; i++){
		fread( &accounts[i], sizeof(Account), 1, file );
	}
}



/* Preso come parametro il vettore di account, restituisce quanti account sono
 *  ancora effettivamente iscritti*/

int contaIscritti( const Account accounts[] )
{
	int utenti = 0;//azzero il contatore degli iscritti

	//ciclo sul vettore di utenti e e incremento il contatore se ci sono utenti iscritti
	for (unsigned int i = 0; i < MASSIMO_NUMERO_ACCOUNT; i++){
		if(accounts[i].isActive == true) utenti++;
	}
	return utenti;//ritorno il contatore di utenti iscritti
}



/* Disattiva l'account dell'utente passato come parametro.*/

int eliminaUtente( Account* account )
{

	if( account != NULL && ( inputOTP() == 1) ){//se l'utente esiste e l'otp è corretto

		azzeraUtente( account );//azzero l'utente
		return 1;//eliminazione avvenuta correttamente

	} else return 0;//eliminazione non avvenuta
}



/* Inizializza ogni singolo campo dell'account passato come parametro */

void azzeraUtente (Account* account)
{
		//pulisco le stringhe
		clearString(account->datiAnagrafici.nome, LUNGHEZZA_NOME);
		clearString(account->datiAnagrafici.cognome, LUNGHEZZA_COGNOME);
		clearString(account->credenziali.email, LUNGHEZZA_EMAIL);
		clearString(account->credenziali.password, LUNGHEZZA_PASSWORD);


		account->fasciaEta = -1;//inizializzo la sua Maturita con un valore fuori dal range
		account->saldo = 0.0f;//azzero il saldo
		account->memoriaDisponibile = 0.0f;//azzero la memoria disponibile
		account->isActive = false;//rendo inattivo l'utente

}



/* Permette di effettuare l'accesso chiedendo le credenziali, che se saranno giuste, l'accesso sara' effettuato
* portando al menu dell'utente, altrimenti, per qualsiasi tipo di problema, l'accesso fallira'*/

void accediUtente( Account accounts[] , App apps[])
{


	Credenziali temp = inputCredenziali();  /* Account temporaneo in cui salvare gli input dell'utente */

	if ( temp.email[0] == '\0' || temp.password[0] == '\0' ){
		puts("\t Assicurati di non lasciare vuoto nessun campo e riprova.");
		getch();
	}
	else {

		Account* utente = cercaUtente( accounts, temp.email );

		if ( utente != NULL ) { //se esiste allora chiedi la password

			if ( strcmp( temp.password, utente->credenziali.password ) == 0 ){ //se la password esiste accede al menuUtente


				/* Entra nel menu dell'utente dopo aver effettuato l'accesso e ci resta fin quando
				 *  non si effettua il logOut cosi da far ritornare 0*/
				while ( menuUtente( accounts, utente , apps ) != 0 && utente->isActive == true );



			} else{
				puts("\n\t La password e' errata.");
				getch();
			}

		} else{
			puts("\n\t Spiacenti, utente non trovato, se non sei ancora iscritto prova a registrarti.");
			getch();
		}

	}
}



/* Permette di entrare in un profilo speciale e unico: Amministratore. L'accesso andra' a buon fine solo se le credenziali
* chieste siano giuste. Da amministratore sara' possibile utilizzare funzioni aggiuntive rispetto ad un utente normale.*/

void accediAdmin( Account accounts[], App apps[])
{
	Credenziali temp  = inputCredenziali();/* Account temporaneo in cui salvare gli input dell'utente */

	if ( testCredenziali( temp ) == 0 ) {

		if ( strcmp( toLower( temp.email ), EMAIL_ADMIN ) == 0 && strcmp( temp.password, PASSWORD_ADMIN ) == 0)//se le credenziali corrispondono a quelle dell'admin
			while ( menuAdmin( accounts , apps ) != 0 );//entra nel menuAdmin
		else puts("\n\t Credenziali admin errati");

	}

}



/* Aggiunge un nuovo utente al vettore di account passato come parametro. Andra' a buon fine solo se l'input dei dati hanno esito positivo
* e se non esiste un altro utente con la stessa email data in input.*/

int registraUtente ( Account accounts[] )
{
	Account utente;/* Account temporaneo in cui salvare gli input dell'utente */
	Account* recordLibero = cercaRecordLiberoinAccounts( accounts );// cerco un record libero nel vettore di Accounts e mi salvo il suo indirizzo


	azzeraUtente( &utente );//azzero l'utente per precauzione


	if ( recordLibero == NULL ){// se non trovo un record libero nel vettore di Accounts vuol dire che non c'è più spazio per potersi registrare
		puts("\n\t Siamo spiacenti per l'inconveniente, ma non c'e' piu' spazio libero per accogliere nuovi iscritti.");

		return 0;
	} else {

		azzeraUtente( recordLibero );//azzero il record libero per precazuzione
		utente.datiAnagrafici = inputDatiAnagrafici();//input dei dati anagrafici dell'utente che intende registrarsi
		utente.credenziali = inputCredenziali();////input delle credenziali dell'utente che intende registrarsi

		//se i suoi dati angrafici e le credenziali sono corretti
		if ( testAnagrafica( utente.datiAnagrafici.nome ) == 0 && testAnagrafica( utente.datiAnagrafici.cognome ) == 0 && testCredenziali( utente.credenziali ) == 0 ) {


			if ( cercaUtente( accounts, utente.credenziali.email ) == NULL ){//se non esiste nessun'altro utente con la stessa email

				utente.isActive = true;//attivo l'utente
				utente.fasciaEta = inputFasciaEta();//input dell'età dell'utente che intende registrarsi
				utente.memoriaDisponibile = genRandom(RANGE_MINIMO_MEMORIA_RANDOM, RANGE_MASSIMO_MEMORIA_RANDOM);//genero una memoria disponibile

				*recordLibero = utente;//assegno al record libero l'utente registrato


			} else {
				puts("\n\t La tua email e' gia' presente nel nostro database!");
				return 0;//l'utente non può registrarsi poichè esiste un altro utente con la stessa email
			}

			return 1;//l'utente è registrato correttamente

		}else return 0;//dati anagrafici o credenziali non corretti
	}

}



/* Dato come parametro un ElencoApp, restituisce un vettore di frequenze di categorie di app presenti nell'elenco. L'indice di posizione del vettore
* corrisponde al genere dell'applicazione, il contenuto rappresenta quante app di quel genere sono state installate dagli utenti.*/

int* getFrequenzeCategorie ( const ElencoApp appInstallate )
{
	int* listaCategorie = ( int* ) calloc ( NUMERO_CATEGORIE, sizeof( int ) );

	for( int i = 0; i < appInstallate.lunghezza; i++)//ciclo di azzeramento del vettore di frequenza di categorie
		listaCategorie[i] = 0;

	//scorro il vettore e incremento l'indice che corrisponde alla Categoria installata
	for ( unsigned int i = 0; i < appInstallate.lunghezza; i++ )
		listaCategorie[ appInstallate.vettoreApp[i]->categoria ]++;


	return listaCategorie;//restituisco un vettore di frequenze di categorie di app presenti nell'elenco
}



/* Cerca nel vettore di account dato da parametro tutti gli utenti attivi e li mette tutti in un elencoAccount, che verra' restituito.*/

ElencoAccount getElencoUtentiAttivi ( const Account accounts[] )
{
	int utentiAttivi = contaIscritti( accounts);

	ElencoAccount elenco;
	elenco.lunghezza = utentiAttivi;

		if ( utentiAttivi == 0 )//se non sono stati trovati utenti attivi ritorno il vettore nullo
			elenco.vettoreUtenti = NULL;
		else {

			elenco.vettoreUtenti = ( Account** ) calloc( elenco.lunghezza, sizeof( Account ) );

			for( unsigned int i = 0, j = 0; i < MASSIMO_NUMERO_ACCOUNT; i++ ){
						if ( accounts[i].isActive ){
							elenco.vettoreUtenti[j] = ( Account* ) &accounts[i];//riempio il vettore con gli utenti attivi
							j++;
						}

			}

		}
		return elenco;//ritorno l'elenco degli utenti attivi
}



/* Riordina l'elencoAccount seguendo il criterio del tipo di ordinamento richiesto.*/

void ordinaAccount ( ElencoAccount elenco, TipoOrdinamentoAccount tipo )
{
	if ( elenco.lunghezza > 1){ // Proseguo con l'orinamento soltanto se c'e' piu di un elemento nel vettore.

			// Si e' utilizzato  il Selection Sort
			for( unsigned int i = 0; i < elenco.lunghezza - 1; i++){ // Scorrendo gli ementi
				unsigned int pos_min = i; // Mi salvo quale sia la posizione il cui andra' l'elemento piu' "piccolo"

				for( unsigned int j = i + 1; j < elenco.lunghezza ; j++){ // Scorro tutti gli elementi dalla posizione del minimo alla fine

					switch ( tipo ){ // E in base a che tipo di ordinamento trovo il minimo e lo metto nella posizione minima.

						case uNOME:
						{
							if( strcmp( toLower( elenco.vettoreUtenti[j]->datiAnagrafici.nome), toLower( elenco.vettoreUtenti[pos_min]->datiAnagrafici.nome ) ) < 0 )
								pos_min = j;
							break;
						}
						case uCOGNOME:
						{
							if ( strcmp( toLower( elenco.vettoreUtenti[j]->datiAnagrafici.cognome ), toLower( elenco.vettoreUtenti[pos_min]->datiAnagrafici.cognome ) ) < 0 )
								pos_min = j;
							break;
						}
						case uEMAIL:
						{
							if ( strcmp( toLower( elenco.vettoreUtenti[j]->credenziali.email ), toLower( elenco.vettoreUtenti[pos_min]->credenziali.email ) ) < 0 )
								pos_min = j;
							break;

					}

				}
			}
			// Identificati gli indici della posizione in cui andra' il minimo e l'indice dell'elemento minimo, scambio gli elementi.
			scambiaAccount( &elenco.vettoreUtenti[pos_min], &elenco.vettoreUtenti[i] );
		}
	}
}



/* Scambia i due puntatori passati come parametro attraverso un passaggio per riferimento.*/

void scambiaAccount ( Account** campo1, Account** campo2 )
{
	Account* temp; // Puntatore di appoggio

	temp = *campo1;		//
	*campo1 = *campo2;	// Scambio i puntatori di app usando il puntatore di appoggio
	*campo2 = temp;		//
}



/* Utilizzando il vettore di account, riscrivo il fileAccount con quei dati, cosi da effettuare il salvataggio dei dati su disco.*/

int aggiornaFileAccount(FILE* fileAccount, const Account accounts[])
{
	if ( fileAccount != NULL ){
		// Se il file esiste

		rewind(fileAccount); //riporto ad inizio file il puntatore

		//scorro su tutti gli utenti e riscrivo il fileAccount con i nuovi valori di essi
		for (unsigned int i = 0; i < MASSIMO_NUMERO_ACCOUNT; i++)
			fwrite(&accounts[i], sizeof(Account) , 1, fileAccount);

	} else return 1; //Esito negativo, il file non esiste

	return 0;// Esito positivo, file aggiornato correttamente.
}






/*
 * FUNZIONI DI INPUT
 */

/*Effettua l'input della email e della password.*/

Credenziali inputCredenziali( )
{
	Credenziali credenziali;// variabile Credenziali temporanea per salvare l'input delle credenziali dell'utente

	clearString(credenziali.email, LUNGHEZZA_EMAIL );//pulisco la stringa

	clearString(credenziali.password, LUNGHEZZA_PASSWORD );//pulisco la stringa


	strcpy(credenziali.email, inputStringa( LUNGHEZZA_EMAIL, "\t  Email: "));//copio la email inserita

	strcpy(credenziali.email, toLower( credenziali.email ) );//la rendo minuscola

	strcpy(credenziali.password, inputStringa( LUNGHEZZA_PASSWORD, "\t  Password: "));//copio la password inserita

	return credenziali;//ritorno le credenziali dell'utente
}



/* Effettua l'input del nome e del cognome.*/

Anagrafica inputDatiAnagrafici()
{
	Anagrafica dati;// variabile Anagrafica temporanea per salvare l'input dei dati anagrafici dell'utente

	clearString(dati.nome, strlen(dati.nome));//pulisco la stringa
	clearString(dati.cognome, strlen(dati.cognome));//pulisca la stringa

	strcpy(dati.nome, inputStringa( LUNGHEZZA_NOME, "\t  Nome: "));//copio il nome inserito

	strcpy(dati.cognome, inputStringa( LUNGHEZZA_COGNOME, "\t  Cognome: "));//copio il cognome inserito

	return dati;//ritorno i dati anagrafici dell'utente
}



/*Effettua l'input dell'eta' per poi associargli una Fascia di eta'.*/

Maturita inputFasciaEta()
{
	int anni = inputIntero(ANNI_MINIMI, ANNI_MASSIMI, "\t  Anni: ");//mi salvo l'età dell'utente

	//ritorno la sua Maturita in base alla sua età
	if (anni < SOGLIA_NEONATO) return NEONATO;
	else if (anni < SOGLIA_BAMBINO) return BAMBINO;
	else if (anni < SOGLIA_ADOLESCENTE) return ADOLESCENTE;
	else if (anni < SOGLIA_GIOVANE_ADULTO) return GIOVANE_ADULTO;
	else if (anni < SOGLIA_ADULTO) return ADULTO;
	else return ANZIANO;

}



/*Effettua il pagamento di un determinato importo.*/

int paga( Account* utente, float importo )
{
	if ( utente->saldo < importo ){//se il credito dell'utente non è sufficiente chiedo una richiesta di ricarica

		puts("\n\t Saldo non sufficente: ");
		if ( richiestaRicarica( utente, importo ) == 0 ){
			if( inputOTP() == 1 )
				utente->saldo += importo;//incremento il conto dell'utente con il costo dell'app
		}
		else return 1;
		utente->saldo -= importo;//se è sufficiente decremento il suo conto del costo dell'app acquistata


	} else {
		utente->saldo -= importo;//se è sufficiente decremento il suo conto del costo dell'app acquistata

	}
	
	return 0;
}



/*Effettua una ricarica del credito dell'utente passato come parametro, chiedendo in input l'importo.*/

int ricarica( Account* utente )
{
	printf("\n\t -- Ricarica il tuo conto --\n\n");
	puts("\t Ricarica minima: 5$\n");//visualizzazione del minimo di ricarica
	puts("\t Ricarica massima: 100$\n");//visualizzazione del massimo di ricarica

	float importo = inputFloat( 5, 100, "\t Importo: ");
	if( inputOTP() ){// se l'otp è corretto
		utente->saldo += importo;//aggiungo l'importo che vuole ricaricare
		return 1;
	} else return 0;
}



/*Modifica la password dell'utente dato come parametro, va a buon fine se la vecchia password inserita e' corretta e se la nuova password rispetta i vincoli previsti.*/

int reimpostaPassword( Account* utente )
{
	printf("\n\t -- Reimposta la tua password --\n\n");
	bool flagErrore = 1;

	//se inserisci correttamente la sua vecchia password..
	if ( strcmp( utente->credenziali.password, inputStringa( LUNGHEZZA_PASSWORD, "\t Vecchia password: ") ) == 0 ){

		//inserisce una nuova
		char* password = (char*) inputStringa( LUNGHEZZA_PASSWORD, "\t Nuova password: ");

		Credenziali temp;
		strcpy( temp.email, utente->credenziali.email );//copio in una variabile Credenziali temporanea le nuove credenziali
		strcpy( temp.password, password );//

		if ( testCredenziali( temp ) == 1 ){//se non sono corrette "sintatticamente" setto il flag con false

			flagErrore = 0;

		} else {//altrimenti se sono corrette


			if ( inputOTP() ){// se l'otp è corretto

				strcpy( utente->credenziali.password, password );//copio la nuova password
				flagErrore = 1; //setto con true

			} else flagErrore = 0;//setto con falso
		}
	}

	return flagErrore;//ritorno il tipo di errore
}



/*Effettua la richiesta all'utente di ricaricare il suo credito del valore dato da parametro.*/

bool richiestaRicarica( Account* utente, float valore )
{

	puts( "\t>Vuoi effettauare una ricarica? " );
	printf( "\n\t [0] Effettua adesso una ricarica di %.2f $.", valore );
	puts( "\n\t [1] Non voglio effettuare una ricarica, rinuncio a questa fantastica applicazione." );
	return inputIntero( 0, 1, "\n\t Input ->");

}






/*
 * FUNZIONI DI OUTPUT
 */

/*Stampa in modo ordinato e formattato tutti gli utenti attivi presenti nel vettore di utenti, numerandoli progressivamente.*/

void stampaAccounts( const Account accounts[] )
{
	system ("cls");

	printf("\n\n%-15s|%-15s|%-32s|%-20s|%-16s|%-8s|%-8s\n", "NOME", "COGNOME", "EMAIL", "PASSOWRD", "FASCIA", "SALDO", "MEMORIA");

	for (unsigned int i = 0; i < MASSIMO_NUMERO_ACCOUNT; i++)// Per ogni elemento del vettore di accounts..
		if (accounts[i].isActive == true)// Se l'utente e' attivo, cioe' non eliminato dall'admin, lo stampo.
			printf("\n%-15s|%-15s|%-32s|%-20s|%-16s|%-8.2f|%-8.2f", accounts[i].datiAnagrafici.nome,
																			  accounts[i].datiAnagrafici.cognome,
																			  accounts[i].credenziali.email,
																			  accounts[i].credenziali.password,
																			  maturitaToString(accounts[i].fasciaEta),
																			  accounts[i].saldo,
																			  accounts[i].memoriaDisponibile );


	puts( "\n\nPremi un tasto per tornare indietro" );
	getch();

}



/* Stampa tutti gli account presenti nell'elencoAccount in modo ordinato e ben formattato.*/

void stampaElencoAccount( const ElencoAccount elencoAccount )
{
		printf("\n\n\t   ID | %-20s | %-20s | %-30s | %-16s | %-8s", "NOME", "COGNOME", "EMAIL", "FASCIA ETA'", "SALDO");
		printf("\n\t   -------------------------------------------------------------------------------------------------------------");

		for ( int i = 0; i < elencoAccount.lunghezza; i++){

			// Per avere un output formattato correttamente se l'indice dell'utente
			switch ( contaCifre( i ) ){
				case 1:
					printf( "\n\t  [%d] |", i ); // Se e' a una cifra metto due spazi
					break;
				case 2:
					printf( "\n\t [%d] |", i ); // Se e' a due cifre metto uno spazio
					break;
				case 3:
					printf( "\n\t[%d] |", i ); // altrimenti non metto spazi
					break;
			}

			// Stampo l'elenco degli utenti
			printf(" %-20s | %-20s | %-30s | %-16s | %-8.2f", elencoAccount.vettoreUtenti[i]->datiAnagrafici.nome,
															  elencoAccount.vettoreUtenti[i]->datiAnagrafici.cognome,
															  elencoAccount.vettoreUtenti[i]->credenziali.email,
															  maturitaToString(elencoAccount.vettoreUtenti[i]->fasciaEta),
															  elencoAccount.vettoreUtenti[i]->saldo);
			printf("\n\t   -------------------------------------------------------------------------------------------------------------");
		}

}



/*Stampa i dettagli dell'utente dato come parametro.*/

void infoUtente( const Account* utente )
{
	system("cls");// Pulisco la schermata della console e stampo ogni campo dell'untente passato come parametro

	puts(" Dove ti trovi: Menu di Accesso / Menu Utente / Profilo / Info");

	puts("\n\n ***** Dettagli sul tuo profilo: *****\n\n");

	printf( "\t %s %s\n\n" , 		"Nome.................", utente->datiAnagrafici.nome );

	printf( "\t %s %s\n\n" , 		"Cognome..............", utente->datiAnagrafici.cognome );

	printf( "\t %s %s\n\n" , 		"Email................", utente->credenziali.email );

	printf( "\t %s %s\n\n" , 		"Password.............", oscuraPassword( utente->credenziali.password ));

	printf( "\t %s $ %.2f\n\n" , 	"Credito Residuo......", utente->saldo );

	printf( "\t %s %.2f MB\n\n" ,	"Memoria Disponibile..", utente->memoriaDisponibile );

	puts("\n\t Premi un tasto per tornare indietro.");
	getch();
}



/*Controlla il formato della email e spiega all'utente il perche' la email e' del formato non corretto ( nel caso fosse in un formato non corretto ).*/

int outputTestEmail( const char email[] )
{
	int flagErrore = 0;

	switch ( controllaEmail( email ) ){//in base all'errore che restituisce controllaEmail visualizza cosa ha sbagliato
		case 1:
			puts("\t Email nel formato non corretto, serve una e una sola @");
			flagErrore = 1;
			break;
		case 2:
			puts("\t Email nel formato non corretto, manca il punto al dominio");
			flagErrore = 1;
			break;
		case 3:
			puts("\t Email nel formato non corretto");
			flagErrore = 1;
			break;
		case 4:
			puts("\t Ci sono caratteri non validi!");
			flagErrore = 1;
			break;
	}
	return flagErrore;
}



/*Controlla il formato della password e spiega all'utente il perche' la password e' del formato non corretto ( nel caso fosse in un formato non corretto ).*/

int outputTestPassword( const char password[] )
{
	int flagErrore = 0;

	switch ( controllaPassword( password ) ){//in base all'errore che restituisce controllaPassword visualizza cosa ha sbagliato
		case 1:
			puts("\t Non possono esserci ';' nella password!");
			flagErrore = 1;
			break;
		case 2:
			puts("\t Caratteri speciali non presenti nella password");
			flagErrore = 1;
			break;
		case 3:
			puts("\t Numeri non presenti nella password");
			flagErrore = 1;
			break;
		case 4:
			puts("\t Inserisci almeno una lettera maiuscola nella password");
			flagErrore = 1;
			break;
		case 5:
			puts("\t Inserisci almeno una lettera minuscola nella password");
			flagErrore = 1;
			break;
	}
	return flagErrore;
}






/*
 * FUNZIONI PER LA RICERCA
 */

/* La funzione cercaUtente(..) scorre tutti gli accounts e confronta la email dell'utente dato come paramentro con l'utente corrente
 *  se corrispondono vuol dire che l'utente esiste quindi ne restituisce il puntatore, se non esiste nell'elenco, ritorna NULL*/

Account* cercaUtente( Account accounts[], const char email[])
{
	Account* record = NULL;//inizializzo il record

	//ciclo sul vettore di Accounts
	for ( unsigned char i = 0; i < MASSIMO_NUMERO_ACCOUNT; i++ ){

		if( strcmp( toLower( email ), toLower( accounts[i].credenziali.email ) ) == 0 ){//se l'utente esiste
			record = &accounts[i];//riempio il record con i suoi dati
			i = MASSIMO_NUMERO_ACCOUNT;//esco dal ciclo
		}
	}
	return record;//ritorno il puntatore al record
}



/*Cerca un record vuoto nel vettore di account, quindi un record che puo' accogliere un nuovo iscritto e ne restituisce il puntatore, oppure NULL se non ce ne sono.*/

Account* cercaRecordLiberoinAccounts( Account accounts[] )
{
	int indiceRecordLibero = -1;//setto l'indice del record libero con valore non corretto

	//scorro il vettore degli utenti cerco il primo primo utente che ha il campo isActive settato a falso, se lo trovo esco dal ciclo
	for ( unsigned int i = 0; i < MASSIMO_NUMERO_ACCOUNT; i++)
		if ( accounts[i].isActive == false){

			indiceRecordLibero = i;//salvo l'indice
			i = MASSIMO_NUMERO_ACCOUNT;//esco dal ciclo

		}

	if ( indiceRecordLibero == -1 ) return NULL;//altrimenti se non ho trovato nessun utente inattivo ritorno indirizzo nullo
	else return &accounts[indiceRecordLibero];//se trovo un record libero ritorno l'indirizzo

}






/*
 * FUNZIONI PER IL CONTROLLO CORRETTEZZA
 */

/*Effettua il test correttezza sulle credenziali date nel parametro, spiegando eventuali errori.*/

int testCredenziali( Credenziali record )
{
	return outputTestPassword( record.password ) + outputTestEmail( record.email );
}






/*
 * FUNZIONI PER LA CONVERSIONE
 */

/*Effettua la conversione da Fascia di eta' a stringa.*/

char* maturitaToString ( Maturita valore )
{
	if ( valore < 0 || valore > ANZIANO ){// se il valore di Maturita ( enum ) è fuori dal range ritorno NULL

		return NULL;

	} else {

		//matrice che contiene i nomi delle corrispondenti alle Maturita degli utenti(enum)
		//l'indice di riga corrisponde a una Maturita(enum) degli utenti
		char nomi[][LUNGHEZZA_MASSIMA_MATURITA] = { "Neonato", "Bambino", "Adolescente", "Giovane Adulto", "Adulto", "Anziano" };

		char* stringa = ( char* ) calloc( LUNGHEZZA_MASSIMA_MATURITA, sizeof( char ) );

		strcpy( stringa, nomi[ ( int ) valore ] );//altrimenti copio in una stringa il nome della Maturita corrispondente

		return stringa;

	}
}


