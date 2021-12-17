/*
 *
 * AccountHead.h
 *
 *
 *      Author: FRANCESCO SOPRANO and MICHELE PERILLI
 */


#include "stdbool.h"

#ifndef ACCOUNTHEAD_H_

	#define ACCOUNTHEAD_H_

	/// Percorso relativo del file binario di Accounts
	#define ACCOUNTS_SP "Database\\Accounts.sp"

	/// Percorso relativo del file testuale di backup di Accounts
	#define ACCOUNTS_BAK "Database\\Accounts.bak"

	/// Dimensione della stringa che la fgets leggera' nel file da backup per poter creare poi il record per il file binario.
    #define DIMENSIONE_RIGA_DA_LEGGERE 200


	/// Lunghezza massima che il nome dell'utente puo' raggiungere
	#define LUNGHEZZA_NOME 21

	/// Lunghezza massima che il cognome dell'utente puo' raggiungere
	#define LUNGHEZZA_COGNOME 21

	/// Lunghezza massima che l'email dell'utente puo' raggiungere
	#define LUNGHEZZA_EMAIL 31

	/// Lunghezza massima che la password dell'utente puo' raggiungere
	#define LUNGHEZZA_PASSWORD 21


	/// Eta' massima che un utente puo' avere per essere considerato NEONATO
	#define SOGLIA_NEONATO 1

	/// Eta' massima che un utente puo' avere per essere considerato BAMBINO
	#define SOGLIA_BAMBINO 10

	/// Eta' massima che un utente puo' avere per essere considerato ADOLESCENTE
	#define SOGLIA_ADOLESCENTE 18

	/// Eta' massima che un utente puo' avere per essere considerato GIOVANE_ADULTO
	#define SOGLIA_GIOVANE_ADULTO 30

	/// Eta' massima che un utente puo' avere per essere considerato ADULTO
	#define SOGLIA_ADULTO 50


	/// Eta' minima che un utente puo' avere.
	#define ANNI_MINIMI 1

	/// Eta' massima che un utente puo' avere.
	#define ANNI_MASSIMI 100


	/// Memoria minima generabile randomicamente all'iscrizione del nuovo utente
	#define RANGE_MINIMO_MEMORIA_RANDOM 1000

	/// Memoria massima generabile randomicamente all'iscrizione del nuovo utente
	#define RANGE_MASSIMO_MEMORIA_RANDOM 3000


	/// Definisce il numero massimo di account che possiamo contenere
	#define MASSIMO_NUMERO_ACCOUNT 200


	/// Lunghezza massima della fascia d'eta' sotto forma di stringa.
	#define LUNGHEZZA_MASSIMA_MATURITA 15



	/// Email per accedere all'account Amministratore
	#define EMAIL_ADMIN "playstore@admin.sp"

	/// Password per accedere all'account Amministratore
	#define PASSWORD_ADMIN "Fake!1!"



	/// Enumerazione sul tipo di ordinamento degli account, se per nome, cognome o email (alfabetico)
	typedef enum { uNOME, uCOGNOME, uEMAIL } TipoOrdinamentoAccount;


	/// Enumerazione sulla fascia di eta' a cui un utente appartiene, l'eta' minima per entrare nella fascia successiva sara' definita da costanti
	typedef enum { NEONATO, BAMBINO, ADOLESCENTE, GIOVANE_ADULTO, ADULTO, ANZIANO } Maturita;





	/// Struct che riunisce in un unico tipo di dato le credenziali dell'utente: email e password.
	typedef struct
	{
		/// Stringa che conterra' una email
		char email[LUNGHEZZA_EMAIL];

		/// Stringa che conterra' una password
		char password[LUNGHEZZA_PASSWORD];

	} Credenziali;





	/// Struct che riunisce in un unico tipo di dato la parte anagrafica dell'utente: nome e cognome
	typedef struct
	{
		/// Nome dell'utente
		char nome[LUNGHEZZA_NOME];

		/// Cognome dell'utente
		char cognome[LUNGHEZZA_COGNOME];

	} Anagrafica;





	/** Tipo di dato capace di contenere un Account, con dati anagrafici, credenziali, fascia d'età a cui appartiene
	 *  e in più c'è il saldo, lo stato della sua memoria di archiviazione e lo stato ( attivo o inattivo ) dell'account stesso */
	typedef struct
	{
		/// Nome e cognome dell'utente
		Anagrafica datiAnagrafici;

		/// Email e password dell'utente
		Credenziali credenziali;

		/// Fascia di eta' a cui appartiene in base alla sua eta'
		Maturita fasciaEta;

		/// Saldo per eventuali app a pagamento
		float saldo;

		/// Memoria in cui l'utente potra' installare app
		float memoriaDisponibile;

		/// Stato dell'account, attivo o inattivo
		bool isActive;

	} Account;




	/// Struttura usata per contenere un vettori di puntatori ad app e la sua relativa lunghezza
	typedef struct
	{
		/// Lunghezza del vettore di puntatori ad utenti
		int lunghezza;

		/// Vettore di puntatori di utenti
		Account** vettoreUtenti;

	} ElencoAccount;


#endif /* ACCOUNTHEAD_H_ */
