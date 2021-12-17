/*
 * AppHead.h
 *
 *
 *      Author: FRANCESCO SOPRANO and MICHELE PERILLI
 */

#include <stdbool.h>

#ifndef APPHEAD_H_

	#define APPHEAD_H_

	/// Percorso relativo del file binario di Apps
	#define APPS_SP "Database\\Apps.sp"

	/// Percorso relativo del file testuale di backup di Apps
	#define APPS_BAK "Database\\Apps.bak"

	/// Dimensione della stringa che la fgets leggera' nel file da backup per poter creare poi il record per il file binario.
	#define DIMENSIONE_RIGA_DA_LEGGERE 200

	/// Numero massimo di applicazioni che possono essere inserite nel vettore di App.
	#define MASSIMO_NUMERO_APP 200

	/// Lunghezza massima che il nome di una App puo' raggiungere.
	#define LUNGHEZZA_TITOLO 51

	/// Costo massimo che una App puo' richiedere per il download.
	#define COSTO_MASSIMO_APP 999.99

	/// Dimensioni in MB massima che una App puo' raggiungere, ovviamente il minimo e' 0.
	#define DIMENSIONE_MASSIMA_APP 2000

	/// Lunghezza massima del nome di uno sviluppatore.
	#define LUNGHEZZA_NOME_SVILUPP 51

	/// Lunghezza massima che puo' raggiungere una categoria sotto forma di stringa.
	#define LUNGHEZZA_CATEGORIA 20

	/// Lunghezza massima che puo' raggiungere la chiave di ricerca.
	#define LUNGHEZZA_CHIAVE_RICERCA 51


	/// Quante categorie di App sono state prese in considerazione.
	#define NUMERO_CATEGORIE 18

	/// Quante app estrarre di una stessa Categoria con la chiamata di consigliaAppPerGenere()
	#define NUMERO_APP_CONSIGLIATE_PER_CATEGORIA 3

	/// Quante app verranno visualizzate quando verra' chiamato un menu riguardande un classifica delle migliori app.
	#define NUMERO_APP_NELLA_TOP 10

	/// Quante app visualizzare quando e' richiesta l'estrazione delle ultime app aggiunte
    #define ULTIME_APP_AGGIUNTE 5


	/// Codice ascii del tasto ESC
	#define ESC 27

	/// Codice ascii del tasto TAB
	#define TAB 9

	/// Ascii di partenza dei tasti funzione da F10 a F12
	#define FUNC_BUTTONS_1 0

	/// Ascii di arrivo dei tasti funzione da F1 a F9
	#define FUNC_BUTTONS_2 -32


	/// Enumerazione usata per poter indicare che tipo di top App, se per maggiori download o se per miglior voto.
	typedef enum { TOPDOWNLOAD, TOPVOTO } TipoTop;

	/// Enumerazione sul genere a cui un'app fa parte.
	typedef enum { ARCADE, SOCIAL, COMUNICAZIONE, MUSICA, STRATEGIA, PRODUTTIVITA, SPORT, STRUMENTI, CORSE, AZIONE, INTRATTENIMENTO, SHOPPING, AVVENTURA, FITNESS, ROMPICAPI, CASUAL, VIDEO, VIAGGI } Categoria;

	/// Enumerazione sul voto che un utente puo' dare ad una app: vNOPE sta per nessun voto, gli altri vanno da una stella a 5.
	typedef enum{ vNOPE, VERYLOW, LOW, MEDIUM, GOOD, VERYGOOD } Voto;

	/// Enumerazione sul tipo di ordinamento da effettuare, se per nome o per genere, ecc..
	typedef enum { oNOME, oDOWNLOAD, oVOTO, oSVILUPPATORE, oGENERE, oPREZZO, oDIMENSIONE } TipoOrdinamentoApp;

	/// Enumerazione su che tipo di ricerca effettuare, se per nome, per sviluppatore o per genere.
	typedef enum { rNOME, rSVILUPPATORE, rGENERE } TipoRicerca;





	/// Struct contenente i dati di una app
	typedef struct
	{
		/// Nome dell'applicazione, e' usato come identificativo.
		char nome[LUNGHEZZA_TITOLO];

		/// Categoria dell'app a cui appartiene.
		Categoria categoria;

		/// Costo dell'applicazione per poterla scaricare.
		float costo;

		/// Dimensione in MB dell'app in questione.
		float dimensione;

		/// Sviluppatore dell'applicazione.
		char sviluppatore[LUNGHEZZA_NOME_SVILUPP];

		/// Numero di volte che l'applicazione e' stata scaricata.
		int totDownload;

		/// Somma dei voti che l'app ha ricevuto.
		int sommaVoti;

		/// Quantita' di voti che l'app ha ricevuto.
		int totVoti;

		/// Variabile booleana che indica lo stato dell'app: attiva o inattiva.
		bool isActive;

	} App;







	/// Struttura usata per contenere un vettori di puntatori ad app e la sua relativa lunghezza.
	typedef struct
	{
		/// Lunghezza del vettore di puntatori ad App.
		int lunghezza;

		/// Vettore di puntatori ad App
		App** vettoreApp;

	} ElencoApp;




#endif /* APPHEAD_H_ */
