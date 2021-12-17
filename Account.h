/*
 * Account.h
 *
 *
 *      Author: FRANCESCO SOPRANO and MICHELE PERILLI
 */

#include <stdio.h>
#include <stdbool.h>
#include "AccountHead.h"

#ifndef ACCOUNT_H_
/** Se questa libreria e' gia stata inclusa, impedisci la ri-definizione
 *  delle costanti e dei tipi di dato di questa libreria */

	#define ACCOUNT_H_

	#include "Apps.h"


	/*
	 * FUNZIONI PER LA GESTIONE ACCOUNTS
	 */

	/**
	 * Apre il file binario degli account, risolvendo eventuali problemi come l'assenza stessa del file.
	 *
	 * @return Puntatore al file binario degli account.
	 * @warning Se il file non esiste verra' creato leggendo dal file di backup, il quale dovra' per forza essere
	 * 			esistente per poter proseguire con il programma
	 */
	FILE* apriAccountsSP();



	/**
	 * Leggendo dal file di backup degli account, crea un file ad accesso casuale per gli account.
	 *
	 * @return Puntatore al file binario degli account.
	 */
	FILE* creaAccountSP();



	/**
	 * Pulisce il vettore di account e lo popola con i dati presenti nel file binario.
	 *
	 * @param file File ad accesso casuale contenente i dati di tutti gli accounts iscritti.
	 * @param accounts Vettore di account in cui verranno caricati i dati presenti nel file.
	 */
	void inizializzaAccounts( FILE* file, Account accounts[] );



	/**
	 * Conta quanti sono i record di account che sono effettivamente degli utenti iscritti, cioe' escluso chi
	 * ha deciso di disiscriversi e qualsiasi altro campo vuoto.
	 *
	 * @param accounts Vettore di tutti gli account presenti nel database.
	 * @return Numero di utenti effetivamente con un account ancora attivo.
	 */
	int contaIscritti( const Account accounts[] );



	/**
	 * Disattiva l'account dell'utente passato come parametro.
	 *
	 * @param account Puntatore all'account da elimininare.
	 * @return 1 Se l'eliminazione dell'utente e' avvenuta con successo;
	 * @return 0 Se l'eliminazione dell'utente e' fallita.
	 */
	int eliminaUtente( Account* account );



	/**
	 * Pulisce il record relativo ad un determinato account.
	 *
	 * @param account Record su cui effettuare la pulitura.
	 */
	void azzeraUtente ( Account* account );



	/**
	 * Permette di effettuare l'accesso chiedendo le credenziali, che se saranno giuste, l'accesso sara' effettuato
	 * portando al menu dell'utente, altrimenti, per qualsiasi tipo di problema, l'accesso fallira'
	 *
	 * @param accounts Vettore di tutti gli account presenti nel programma, tra cui cercare l'utente che vuole effettuare l'accesso;
	 * @param apps Vettore di app, servira' per poter far effettuare ricerca, installazione o semplicemente visualizzare le info delle app all'utente.
	 * @warning L'accesso fallisce nel caso in cui le credenziali date in input siano sbagliate, oppure con almeno un campo vuoto.
	 */
	void accediUtente( Account accounts[], App apps[] );



	/**
	 * Permette di entrare in un profilo speciale e unico: Amministratore. L'accesso andra' a buon fine solo se le credenziali
	 * chieste siano giuste. Da amministratore sara' possibile utilizzare funzioni aggiuntive rispetto ad un utente normale.
	 *
	 * @param accounts Vettore di tutti gli utenti su cui poter effettuare modifiche.
	 * @param apps Vettore di tutti le app su cui poter effettuare modifiche.
	 */
	void accediAdmin( Account accounts[], App apps[]);



	/**
	 * Aggiunge un nuovo utente al vettore di account passato come parametro. Andra' a buon fine solo se l'input dei dati hanno esito positivo
	 * e se non esiste un altro utente con la stessa email data in input.
	 *
	 * @param accounts Vettore a cui verra' aggiunto il nuovo account.
	 * @return 0 Se l'iscrizione del nuovo account non e' andata a buon fine;
	 * @return 1 Se l'iscrizione e' avvenuta con successo.
	 * @warning L'iscrizione fallisce se non tutti gli input sono considerati corretti o se non ci sta un record disponibile in accounts.
	 */
	int registraUtente ( Account accounts[] );



	/**
	 * Dato come parametro un ElencoApp, restituisce un vettore di frequenze di categorie di app presenti nell'elenco. L'indice di posizione del vettore
	 * corrisponde al genere dell'applicazione, il contenuto rappresenta quante app di quel genere sono state installate dagli utenti.
	 *
	 * @param appInstallate ElencoApp da cui estrarre le frequenze di installazione
	 * @return Vettore di interi, dove l'indice di posizione corrisponde alla categoria e il contenuto rappresenta quante app di quel genere ci sono.
	 */
	int* getFrequenzeCategorie ( const ElencoApp appInstallate );



	/**
	 * Cerca nel vettore di account dato da parametro tutti gli utenti attivi e li mette tutti in un elencoAccount, che verra' restituito.
	 *
	 * @param accounts Vettore di account in cui cercare gli utenti attivi usati per assemblare l'elencoAccount.
	 * @return ElencoAccount contenente tutti gli utenti attivi al momento della chiamata di questa funzione.
	 * @warning Se non vengono trovati utenti attivi, la lunghezza verra' fissata a 0 e il vettore sara' NULL
	 */
	ElencoAccount getElencoUtentiAttivi ( const Account accounts[] );



	/**
	 * Riordina l'elencoAccount seguendo il criterio del tipo di ordinamento richiesto.
	 *
	 * @param elenco elencoAccount da riordinare.
	 * @param tipo Criterio di ordinamento.
	 */
	void ordinaAccount ( ElencoAccount elenco, TipoOrdinamentoAccount tipo );



	/**
	 * Scambia i due puntatori passati come parametro attraverso un passaggio per riferimento.
	 *
	 * @param campo1 Indirizzo del puntatore dell' account da scambiare con il secondo parametro.
	 * @param campo2 Indirizzo del puntatore dell' account da scambiare con il primo parametro.
	 */
	void scambiaAccount ( Account** campo1, Account** campo2 );



	/**
	 * Utilizzando il vettore di account, riscrivo il fileAccount con quei dati, cosi da effettuare il salvataggio dei dati su disco.
	 *
	 * @param fileAccount Puntatore al file su cui salvare il vettore di account.
	 * @param accounts Vettore di account da salvare su file.
	 * @warning Se il puntatore a file e' nullo, non verra' effettuata nessuna operazione.
	 */
	int aggiornaFileAccount( FILE* fileAccount, const Account accounts[] );






	/*
	 * FUNZIONI DI INPUT
	 */

	/**
	 * Effettua l'input della email e della password.
	 *
	 * @return Struct Credenziali, appunto composta della email e della password.
	 * @warning Non effettua controlli sulla correttezza.
	 */
	Credenziali inputCredenziali();



	/**
	 * Effettua l'input del nome e del cognome.
	 *
	 * @return Struct anagrafica, appunto composta dal nome e dal cognome inseriti.
	 * @warning Non effettua controlli sulla correttezza.
	 */
	Anagrafica inputDatiAnagrafici();



	/**
	 * Effettua l'input dell'eta' per poi associargli una Fascia di eta'
	 *
	 * @return Enum Maturita' rappresentante la fascia di eta' corrispondente agli anni dati in input.
	 */
	Maturita inputFasciaEta();



	/**
	 * Effettua il pagamento di un determinato importo.
	 *
	 * @param utente Puntatore all'utente che deve effettuare il pagamento.
	 * @param importo Di quanto verra' ridotto il credito dell'utente.
	 * @return 0 Se il pagamento va a buon fine;
	 * @return 1 Se il pagamento e' stato interrotto per qualsiasi tipo di errore.
	 * @warning Se l'utente non ha abbastanza credito, ritorna 1 se si rifiuta di effettuare una ricarica, invece torna 0 se effettua una ricarica adeguata ( che va a buon fine ).
	 */
	int paga( Account* utente, float importo );



	/**
	 * Effettua una ricarica del credito dell'utente passato come parametro, chiedendo in input l'importo.
	 *
	 * @param account Puntatore all'account da ricaricare.
	 * @return 0 Se la ricarica fallisce, cioe' se l'utente sbaglia l'inserimento della OTP;
	 * @return 1 Se la ricarica va a buon fine inserendo l'OTP giusta.
	 */
	int ricarica( Account* account );



	/**
	 * Modifica la password dell'utente dato come parametro, va a buon fine se la vecchia password inserita e' corretta e se la nuova password rispetta i vincoli previsti.
	 *
	 * @param utente Puntatore all'utente su cui effettuare la modifica della password di accesso.
	 * @return 0 Se la modifica fallisce;
	 * @return 1 Se la modifica va a buon fine.
	 */
	int reimpostaPassword( Account* utente );



	/**
	 * Effettua la richiesta all'utente di ricaricare il suo credito del valore dato da parametro.
	 *
	 * @param utente Puntatore all'utente a cui effettuare la richiesta.
	 * @param valore Importo da chiedere all'utente di ricaricare.
	 * @return true Se l'utente accetta ed effettua la ricarica;
	 * @return false Se l'utente rifiuta e non effetta la ricarica.
	 */
	bool richiestaRicarica( Account* utente, float valore );






	/*
	 * FUNZIONI DI OUTPUT
	 */

	/**
	 * Stampa tutti gli account presenti nel vettore in modo ordinato e ben formattato.
	 *
	 * @param accounts Vettore di utenti da stampare.
	 */
	void stampaAccounts( const Account accounts[] );



	/**
	 * Stampa tutti gli account presenti nell'elencoAccount in modo ordinato e ben formattato.
	 *
	 * @param elencoAccount Elenco di utenti da stampare.
	 */
	void stampaElencoAccount( const ElencoAccount elencoAccount );



	/**
	 * Stampa i dettagli dell'utente dato come parametro.
	 *
	 * @param utente Puntatore all'utente di cui stampare le info.
	 */
	void infoUtente( const Account* utente );



	/**
	 * Controlla il formato della email e spiega all'utente il perche' la email e' del formato non corretto ( nel caso fosse in un formato non corretto ).
	 *
	 * @param email Stringa su cui effettuare il controllo di correttezza email.
	 * @return 0 Se la email e' del formato corretto;
	 * @return 1 Se la email non e' del formato corretto.
	 */
	int outputTestEmail( const char email[] );



	/**
	 * Controlla il formato della password e spiega all'utente il perche' la password e' del formato non corretto ( nel caso fosse in un formato non corretto ).
	 *
	 * @param password Stringa su cui effettuare il controllo di correttezza password.
	 * @return 0 Se la password e' del formato corretto;
	 * @return 1 Se la password non e' del formato corretto.
	 */
	int outputTestPassword( const char password[] );






	/*
	 * FUNZIONI PER LA RICERCA
	 */

	/**
	 * Cerca l'utente nel vettore di accounts in base all'email data come parametro e se lo trova restituisce il puntatore, altrimenti restituisce NULL.
	 *
	 * @param accounts Vettore di account su cui effettuare la ricerca.
	 * @param email Campo usato per cercare l'account nel vettore.
	 * @return NULL Se non c'e' nessun utente nel vettore la cui email corrisponda a quella data da parametro;
	 * @return Puntatore all'account avente la stessa email di quella data da parametro.
	 */
	Account* cercaUtente( Account accounts[], const char email[] );



	/**
	 * Cerca un record vuoto nel vettore di account, quindi un record che puo' accogliere un nuovo iscritto e ne restituisce il puntatore, oppure NULL se non ce ne sono.
	 *
	 * @param accounts Vettore su cui effettuare la ricerca del record libero.
	 * @return NULL Se non ci sono record liberi in accounts
	 * @return Puntatore al primo record libero in accounts se c'e'.
	 */
	Account* cercaRecordLiberoinAccounts( Account accounts[] );






	/*
	 * FUNZIONI PER IL CONTROLLO CORRETTEZZA
	 */

	/**
	 * Effettua il test correttezza sulle credenziali date nel parametro, spiegando eventuali errori.
	 *
	 * @param record Credenziali su cui effettuare il controllo.
	 * @return 0 Se le credenziali sono del formato corretto;
	 * @return 1 Se o la email o la password sono del formato non corretto;
	 * @return 2 Se sia la email che la password sono del formato non corretto.
	 */
	int testCredenziali( Credenziali record );



	/*
	 * FUNZIONI PER LA CONVERSIONE
	 */

	/**
	 * Effettua la conversione da Fascia di eta' a stringa.
	 *
	 * @param valore Fascia d'eta' su cui effettuare l'operazione di conversione.
	 * @return Stringa corrispondente alla fascia d'eta' data da parametro.
	 */
	char* maturitaToString ( Maturita valore );



#endif /* ACCOUNT_H_ */
