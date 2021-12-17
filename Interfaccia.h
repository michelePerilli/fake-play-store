/*
 * Interfaccia.h
 *
 *
 *      Author: FRANCESCO SOPRANO and MICHELE PERILLI
 */

#ifndef INTERFACCIA_H_

/* Uso come identificativo di dichiarazione avvenuta della libreria INTERFACCIA_H_
 * - se questa libreria non è mai stata dichiarata, verrà definita la costante che impedirà
 * 	 la reinclusione di questa stessa libreria	*/

	#define INTERFACCIA_H_


	#include "Apps.h"
	#include "Account.h"
	#include "Cronologia.h"


	/*
	 * APPS
	 */

	/**
	 * Visualizza il menu relativo ad una singola app in cui ci saranno opzioni del tipo "installa" oppure "lascia un voto"
	 *
	 * @param app Puntatore all'app di cui visualizzare il menu'.
	 * @param utente Puntatore all'utente che ha aperto il menu'.
	 * @param apps Vettore di app utilizzato nel caso serva mostrare l'elenco delle app.
	 * @return 0 Se l'utente vuole uscire dal menu';
	 * @return 1 Se l'utente non ha ancora espresso la volonta' di uscire dal menu'.
	 */
	int menuApp( App* app , const Account* utente, App apps[] );



	/**
	 * Dopo una ricerca, viene generato un ElencoApp che passato a questo menu', si da la possibilita' all'utente passato come parametro di scegliere
	 * l'app che desidera e di entrare nel suo menu', per poi decidere che altra funzione usare.
	 *
	 * @param elencoApp Elenco di app da visualizzare nel menu'.
	 * @param utente Puntatore all'utente che ha aperto il menu'.
	 * @param apps Vettore di app utilizzato nel caso serva mostrare l'elenco delle app che non stanno nell'elenco.
	 */
	void menuAppTrovate( ElencoApp elencoApp, const Account* utente, App apps[] );



	/**
	 * Visualizza il menu' contenente le app che l'utente passato come parametro ha installato almeno una volta, potendo scegliere di reinstallare quelle
	 * gia disinstallate oppure disinstallare quelle attualmente installate.
	 *
	 * @param lista Elenco estratto dal file cronologia riguardante l'utente dato dal parametro.
	 * @param utente Puntatore all'utente che ha aperto il menu'.
	 * @param apps Vettore di app utilizzato nel caso serva mostrare l'elenco delle app che non stanno nell'elenco.
	 * @return 0 Se l'utente vuole uscire dal menu';
	 * @return 1 Se l'utente non ha ancora espresso la volonta' di uscire dal menu'.
	 */
	int menuLeMieApp( ElencoCronologia lista, Account* utente, App apps[] );



	/**
	 * Visualizza il menu' contenente tutte le app, su cui l'utente puo' interagire, scegliendo quella che vuole.
	 *
	 * @param utente Puntatore all'utente che ha aperto il menu'.
	 * @param apps Vettore di app da cui vengono presi dati per creare il menu' interattivo.
	 */
	void menuTutteLeApp( Account* utente, App apps[] );



	/**
	 * Visualizza il menu' per poter scegliere la ricerca app che l'utente preferisce: ricerca per Nome, Genere e Sviluppatore.
	 *
	 * @param utente Puntatore all'utente che ha aperto il menu'.
	 * @param apps Vettore di app su cui effettuare la ricerca.
	 */
	void menuCercaApp( Account* utente, App apps[] );



	/**
	 * Visualizza l'elenco delle migliori app, per maggior numero di download o per miglior voto.
	 *
	 * @param utente Puntatore all'utente che ha aperto il menu'.
	 * @param apps Vettore di app da cui vengono presi dati per creare il menu' interattivo.
	 * @param tipo Tipologia di classifica che si vuole visualizzare
	 */
	void menuTopApps( Account* utente, App apps[], TipoTop tipo );



	/**
	 * Visualizza l'elenco delle possibili app da consigliare, per utenti simili o per app simili.
	 *
	 * @param accounts Vettore di account da dove pescare un utente simile.
	 * @param utente Puntatore all'utente che ha aperto il menu'.
	 * @param apps Vettore di app da cui pescare le app simili.
	 */
	void menuAppSuggerite( Account accounts[], Account* utente, App apps[] );



	/**
	 * Menu in cui l'utente sceglie quale funzione usare sulle app: es. Ricerca, Migliori app, Suggerimento app...
	 *
	 * @param accounts Vettore di account usato per leggere i dati degli altri account cosi da poter suggerire le applicazione dell'utente che ha aperto il menu.
	 * @param utente Puntatore all'utente che ha aperto il menu'.
	 * @param apps Vettore di app su cui verranno applicate le funzioni che l'utente scegliera' di usare.
	 * @return
	 */
	int menuApps( Account accounts[], Account* utente, App apps[] );




	/*
	 * UTENTE
	 */

	/**
	 * Menu che racchiude le funzioni che l'utente puo' effettuare su se stesso, ad esempio vedere le suo info, reimpostare la password, vedere le sue app, ecc..
	 *
	 * @param utente Puntatore all'utente che ha aperto il menu'.
	 * @return 0 Se l'utente vuole uscire dal menu';
	 * @return 1 Se l'utente non ha ancora espresso la volonta' di uscire dal menu'.
	 */
	int menuProfilo( Account* utente );



	/**
	 * Menu in cui l'utente effettua la scelta se usare funzioni per le apps oppure su se stesso.
	 *
	 * @param accounts Vettore di account da cui raccogliere informazioni per eventuali suggerimenti di app che hanno anche altri utenti simili.
	 * @param utente Puntatore all'utente che ha aperto il menu'.
	 * @param apps Vettore di app su cui applicare le funzioni per le app, quando decide l'utente.
	 * @return 0 Se l'utente vuole uscire dal menu';
	 * @return 1 Se l'utente non ha ancora espresso la volonta' di uscire dal menu'.
	 */
	int menuUtente( Account accounts[], Account* utente, App apps[] );



	/**
	 * Visualizza il menu' contenente tutti gli account attivi
	 *
	 * @param apps Vettore di app usato per entrare in un profilo di un utente.
	 * @param accounts Vettore di account da cui prendere i dati degli utenti attivi.
	 */
	void menuTuttiGliAccount( App apps[], Account accounts[] );



	/**
	 * Visualizza il menu' contente gli utenti trovati da un'altra funzione, cioe' presente il ElencoAccount.
	 *
	 * @param elencoAccount ElencoAccount contenente gli account da mostrare nel menu.
	 * @param apps Vettore di apps usato per poter chiamare menuProfilo() appena si sceglie un utente.
	 * @return 0 Se l'utente vuole uscire da questo menu';
	 * @return 1 Se l'utente non ha ancora deciso di uscire da questo menu'.
	 */
	int menuUtentiTrovati( ElencoAccount elencoAccount, App apps[] );



	/**
	 * Menu in cui l'utente sceglie se accedere al suo profilo oppure se registrarsi nel caso non ne abbia uno. E' possibile scegliere anche l'accesso in amministratore
	 * se si e' a conoscenza delle credenziali che solo gli sviluppatori del programma conoscono.
	 *
	 * @param fileAccount File su cui effettuare il salvataggio delle modifiche sugli accounts se l'utente vuole.
	 * @param fileApp File su cui effettuare il salvataggio delle modifiche sulle apps se l'utente vuole.
	 * @param accounts Vettore di account su cui si lavorera' con tutte le funzioni, come l'iscrizione, l'eliminazione di un account, ecc..
	 * @param apps Vettore di app su cui si lavorera' con tutte le funzioni, come la ricerca, ordinamento, ecc..
	 * @return 0 Se l'utente vuole uscire dal menu';
	 * @return 1 Se l'utente non ha ancora espresso la volonta' di uscire dal menu'.
	 */
	int menuPrincipale( FILE* fileAccount, FILE* fileApp, Account accounts[], App apps[] );






	/*
	 * ADMIN
	 */

	/**
	 * Menu relativo a tutte le funzioni che l'amministratore ha a disposizione per gli accounts.
	 *
	 * @param apps Vettore di app su cui l'amministratore puo' applicare determinate funzioni se desidera.
	 * @return 0 Se l'utente vuole uscire dal menu';
	 * @return 1 Se l'utente non ha ancora espresso la volonta' di uscire dal menu'.
	 */
	int menuAdminApps( App apps[] );



	/**
	 * Menu relativo a tutte le funzioni che l'amministratore ha a disposizione per gli accounts.
	 *
	 * @param accounts Vettore di account su cui l'amministratore puo' applicare determinate funzioni se desidera.
	 * @return 0 Se l'utente vuole uscire dal menu';
	 * @return 1 Se l'utente non ha ancora espresso la volonta' di uscire dal menu'.
	 */
	int menuAdminUtenti( Account accounts[] );



	/**
	 * Menu in cui l'amministratore sceglie se lavorare sugli account oppure sulle app.
	 *
	 * @param accounts Vettore di account su cui l'amministratore puo' applicare determinate funzioni se desidera.
	 * @param apps Vettore di app su cui l'amministratore puo' applicare determinate funzioni se desidera.
	 * @return 0 Se l'utente vuole uscire dal menu';
	 * @return 1 Se l'utente non ha ancora espresso la volonta' di uscire dal menu'.
	 */
	int menuAdmin( Account accounts[], App apps[]);






	/*
	 * LOGO
	 */

	/**
	 * Visualizza la schermata iniziale di benvenuto nel programma.
	 *
	 * @param path Percorso del file contenente il logo/intro del programma.
	 */

	void stampaIntro( char path[] );

#endif /* INTERFACCIA_H_ */


