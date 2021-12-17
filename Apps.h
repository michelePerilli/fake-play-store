/*
 * Apps.h
 *
 *
 *      Author: FRANCESCO SOPRANO and MICHELE PERILLI
 */

#include <stdio.h>
#include "AppHead.h"
#include "AccountHead.h"
#include "Cronologia.h"

#ifndef APP_H_

/** Uso come identificativo di dichiarazione avvenuta della libreria INTERFACCIA_H_
 * - se questa libreria non e' mai stata dichiarata, verra' definita la costante che impedira'
 * 	 la reinclusione di questa stessa libreria	*/

	#define APP_H_



	/*
	 * FUNZIONI PER LA GESTIONE APPS
	 */

	/**
	 * Apre il file binario delle app, risolvendo eventuali problemi come l'assenza stessa del file.
	 *
	 * @return Puntatore al file binario delle app.
	 * @warning Se il file non esiste verra' creato leggendo dal file di backup, il quale dovra' per forza essere
	 * 			esistente per poter proseguire con il programma, altrimenti ritornera' NULL
	 */
	FILE* apriAppsSP();



	/**
	 * Leggendo dal file di backup delle app, crea un file ad accesso casuale per le apps.
	 *
	 * @return Puntatore al file binario delle app.
	 */
	FILE* creaAppSP();



	/**
	 * Pulisce il vettore di app e lo popola con i dati presenti nel file binario.
	 *
	 * @param file File ad accesso casuale contenente i dati di tutti delle apps attive.
	 * @param apps Vettore di app in cui verranno caricati i dati presenti nel file.
	 */
	void inizializzaApps( FILE* file, App apps[] );



	/**
	 * Conta quanti sono i record di app che sono effettivamente attive, cioe' escluso le
	 * app cancellate dall'amministratore
	 *
	 * @param apps Vettore di tutti gli account presenti nel database.
	 * @param chiave Stringa in base al quale contare le app.
	 * @param tipo Tipo di riferimento della chiave, se e' un nome o genere o sviluppatore.
	 * @return Numero di app effetivamente ancora attive.
	 */
	int contaApp ( const App apps[], char* chiave, TipoRicerca tipo );



	/**
	 * Pulisce il record relativo ad una determinata app.
	 *
	 * @param app Record su cui effettuare la pulitura.
	 */
	void azzeraApp ( App* app );



	/**
	 * Elimina l'app se l'inserimento dell'OTP va a buon fine, altrimenti l'app non verra' toccata.
	 *
	 * @param app Puntatore all'app da eliminare.
	 * @return 0 Se l'eliminazione fallisce a causa di un errato input dell'OTP richiesta;
	 * @return 1 Se L'app e' stata eliminata correttamente.
	 */
	int eliminaApp( App* app );


	/**
	 * Aggiunge una nuova app al vettore di app passato come parametro. Andra' a buon fine solo se l'input dei dati hanno esito positivo
	 * e se non esiste un altra app con lo stesso nome.
	 *
	 * @param apps Vettore a cui verra' aggiunto la nuova app.
	 * @return 0 Se l'aggiunta della nuova app non e' andata a buon fine;
	 * @return 1 Se l'aggiunta della nuova app e' avvenuta con successo.
	 * @warning L'iscrizione fallisce se non tutti gli input sono considerati corretti o se non ci sta un record disponibile in apps.
	 */
	int aggiungiApp( App apps[] );



	/**
	 * Permette all'utente dato come parametro di installare l'app data come parametro se isInstalled indica come non installata, altrimenti
	 * la disinstalla. Durante l'installazione viene presa in considerazione la memoria disponibile dell'utente e del credito. Se la memoria
	 * disponibile non e' abbastanza, mostro tutte le app che ha installato cosi, disinstallandone q.b. potra' continuare l'installazione.
	 * Se il credito non e' sufficente verra' effettuata una richiesta di ricarica dello stesso importo del prezzo dell'app.
	 *
	 * @param app Puntatore all'app da installare/disinstallare.
	 * @param utente Puntatore all'utente che vuole installare l'app.
	 * @param isInstalled Valore booleano che indica se l'app e' gia installata o meno.
	 * @param apps Vettore di apps da cui attingere se bisogna mostrare tutte le app installate.
	 */
	void installaApp( const App* app, Account* utente, bool isInstalled , App apps[] );



	/**
	 * Scambia i due puntatori passati come parametro attraverso un passaggio per riferimento.
	 *
	 * @param campo1 Indirizzo del puntatore della app da scambiare con il secondo parametro.
	 * @param campo2 Indirizzo del puntatore della app da scambiare con il primo parametro.
	 */
	void scambiaApp ( App** campo1, App** campo2 );



	/**
	 * Dato un Elenco di app, lo ordina seguendo il criterio dato dal parametro TipoOrdinamento.
	 *
	 * @param elenco Elenco app da ordinare secondo il criterio dato da tipo.
	 * @param tipo Tipo di ordinamento da effettuare sull'elenco app.
	 */
	void ordinaApp ( ElencoApp elenco, TipoOrdinamentoApp tipo );



	/**
	 * Utilizzando il vettore di app, riscrivo il fileApp con quei dati, cosi da effettuare il salvataggio dei dati su disco.
	 *
	 * @param fileApp Puntatore al file su cui salvare il vettore di account.
	 * @param apps Vettore di account da salvare su file.
	 * @warning Se il puntatore a file e' nullo, non verra' effettuata nessuna operazione.
	 */
	int aggiornaFileApp( FILE* fileApp, App apps[] );



	/*
	 * FUNZIONI DI INPUT
	 */

	/**
	 * Chiede l'input di una categoria di app fino a che non trova una corrispondenza.
	 *
	 * @param testo Testo che viene stampato per incitare l'utente a eseguire l'input.
	 * @return Categoria presa in input.
	 */
	Categoria inputCategoria( const char* testo );



	/**
	 * Posizionandoci sul file Cronologia in posizione di riga, l'app in quella posizione ricevera' un voto che verra' chiesto in input
	 *
	 * @param riga Posizione della associazione tra app e account sul file cronologia.
	 */
	void lasciaUnVoto( int riga );






	/*
	 * FUNZIONI DI OUTPUT
	 */

	/**
	 * Visualizza le informazioni dettagliate dell'app.
	 *
	 * @param app Puntatore all'app di cui visualizzare le informazioni.
	 */
	void infoApp( const const App* app );



	/**
	 * Stampa in modo ordinato e formattato tutte le app presenti nel vettore di app, numerandole progressivamente.
	 *
	 * @param apps Vettore di app da cui prendere i dati da stampare.
	 */
	void stampaApps( const App apps[] );



	/**
	 * Stampa in modo ordinato e formattato tutte le app presenti nell'elenco di app, numerandole progressivamente.
	 *
	 * @param elencoApp Elenco di app da cui prendere i dati da stampare
	 */
	void stampaElencoApp( const ElencoApp elencoApp );






	/*
	 * FUNZIONI PER LA RICERCA
	 */

	/**
	 * Effettua la ricerca del tipo dato da parametro, nel vettore di app dato come parametro. Non attende l'invio per iniziare la ricerca,
	 * ma viene effettuata ad ogni pressione di un tasto, fermandosi quando viene premuto invio.
	 *
	 * @param apps Vettore di app su cui effettuare la ricerca.
	 * @param tipo Tipo di ricerca da effettuare.
	 * @return Elenco delle app trovate.
	 * @warning Se non trova nemmeno un'app, ritornera' NULL.
	 */
	ElencoApp ricercaAppTempoReale( const App apps[], TipoRicerca tipo );


	/**
	 * Effettua la ricerca del tipo dato da parametro, nel vettore di app dato come parametro, usando la chiave di ricerca passata da parametro.
	 *
	 * @param apps Vettore di app su cui effettuare la ricerca.
	 * @param chiave Stringa da utilizzare come chiave di ricerca.
	 * @param tipo Tipo di ricerca da effettuare.
	 * @return Elenco delle app trovate.
	 * @warning Se non trova nemmeno un'app, la lunghezza dell'ElencoApp sara' 0 e il vettore NULL.
	 */
	ElencoApp ricercaApp ( const App apps[], char* chiave, TipoRicerca tipo );



	/**
	 * Effettua la ricerca per nome, nel vettore di app dato come parametro, usando la chiave di ricerca passata da parametro.
	 *
	 * @param apps  Vettore di app su cui effettuare la ricerca.
	 * @param chiave Stringa da utilizzare come chiave di ricerca.
	 * @return Puntatore all'app trovata;
	 * @return NULL Se non trova l'app.
	 */
	App* ricercaAppIntegrale( const App apps[], char* chiave );



	/**
	 * Consiglia le app che ha installato un utente random appartenente alla fascia di eta' passata come parametro.
	 *
	 * @param accounts Vettore di account da cui prendere un utente a caso con la fascia di eta' giusta.
	 * @param fasciaEta Fascia d'eta' a cui un utente deve appartenere per poter essere sorteggiato come 'consigliere di app'.
	 * @return ElencoCronologia contenente tutte le app con cui l'utente sorteggiato ha interagito.
	 * @warning Se non trova utenti con quella fascia di eta', l'ElencoCronologia avra' lunghezza zero e vettore NULL.
	 */
	ElencoCronologia consigliaAppPerEta( const Account accounts[], const Maturita fasciaEta );



	/**
	 * Passando come parametro la frequenza di installazioni di generi di app installate dall'utente che chiama la funzione,
	 * costruisce un ElencoApp formato da massimo 3 app per ogni genere che ha installato.
	 *
	 * @param frequenzaCategorie Vettore di interi dove l'indice rappresenta la categoria e il contenuto rappresenta quante app di quel genere l'utente ha installato.
	 * @param apps Vettore di app da cui prendere le app da consigliare.
	 * @return ElencoApp contenente le app suggerite.
	 * @warning Se non trova app da suggerire la sua dimensione sara' zero e il vettore NULL.
	 */
	ElencoApp consigliaAppPerCategoria( const int frequenzaCategorie[], const App apps[] );



	/**
	 * Cerca le ultime app aggiunte.
	 *
	 * @param apps Vettore di tutte le app su cui effettuare la ricerca.
	 * @return Elenco app delle ultime app aggiunte.
	 */
	ElencoApp ultimeAppAggiunte( const App apps[] );



	/**
	 * Cerca il primo record non occupato da una app attiva nel vettore di app e ne restituisce il puntatore.
	 *
	 * @param apps Vettore di app in cui effettuare la ricerca.
	 * @return Puntatore al primo record non occupato da una app attiva;
	 * @return NULL Se non ci sono record liberi.
	 */
	App* cercaRecordLiberoinApps( App apps[] );



	/*
	 * FUNZIONI DI CONVERSIONE
	 */

	/**
	 * Effettua una conversione di tipo da ElencoCronologia ad ElencoApp, sfruttando le informazioni presenti nel vettore di app.
	 *
	 * @param elenco Elenco di cronologia da convertire
	 * @param apps Vettore di app da dove attingere informazioni per costruire l'elenco app
	 * @return Elenco App corrispondente all'elenco cronologia.
	 */
	ElencoApp elencoCronologiaToElencoApp( ElencoCronologia elenco, const App apps[] );



	/**
	 * Effettua una conversione di tipo da Categoria a stringa, prendendo appunto come parametro la categoria.
	 *
	 * @param genere Categoria app da convertire in stringa.
	 * @return Stringa corrispondente alla categoria data come parametro.
	 */
	char* categoriaToString ( Categoria genere );



	/**
	 * Effettua una conversione di tipo da stringa a Categoria, prendendo la stringa come parametro.
	 *
	 * @param genere Stringa da convertire in Categoria.
	 * @return Categoria corrispondente alla stringa data come parametro.
	 */
	Categoria stringToCategoria ( char* genere );





#endif /* APP_H_ */





