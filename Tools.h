/*
 * Tools.h
 *
 *
 *      Author: FRANCESCO SOPRANO and MICHELE PERILLI
 */

#ifndef TOOLS_H_

/* Uso TOOLS_H_ come identificativo di avvenuta inclusione della libreria
 * - se questa libreria non e' mai stata dichiarata, verra' definita la costante che impedira'
 *   la reinclusione di questa stessa libreria	*/


	#define TOOLS_H

    #include<stdbool.h>


	/// Partenza del range di ascii che verranno generati per costruire una OTP
	#define ASCII_MINIMO_CARATTERE_OTP 65

	/// Partenza del range di ascii che verranno generati per costruire una OTP
	#define ASCII_MASSIMO_CARATTERE_OTP 90

	/// Numero di caratteri della OTP
	#define DIMENSIONE_OTP 5



	/*
	 * FUNZIONI MATEMATICHE
	 */

	/**
	 * Conta quante cifre compongono il valore dato nel parametro.
	 *
	 * @param n	Un intero di cui vogliamo sapere il numero di cifre
	 * @return Numero di cifre del numero dato dal parametro
	 */
	int contaCifre( int n );



	/**
	 * Restituisce la media aritmetrica avente la somma dei numeri e la loro quantita'
	 *
	 * @param somma La somma dei valori di cui vogliamo sapere la media
	 * @param quantita Quanti numeri sono di cui stiamo calcolando la media
	 * @return Un float rappresentante la media aritmetrica
	 * @warning Se quantita e' un valore nullo, la funzione ritornera' 0.0f
	 */
	float mediaVoti ( float somma, float quantita );






	/*
	 * FUNZIONI PER LA MANIPOLAZIONE STRINGHE
	 */

	/**
	 * Inizializza la stringa data come parametro, pulendola da eventuali dati indesiderati
	 *
	 * @param vet Stringa su cui effettuare la pulizia
	 * @param len Quantita' di caratteri della stringa da pulire
	 */
	void clearString(char vet[], int len);



	/**
	 *  Data una stringa come parametro, toglie gli spazi iniziali e finali
	 *
	 * @param stringa Stringa su cui effettuare il lavoro
	 */
	void togliSpaziDagliEstremi( char stringa[] );



	/**
	 * Restitusce la stringa data in input, offuscata da asterischi, eccezione fatta per il primo e l'ultimo carattere.
	 *
	 * @param password Stringa con qualificatore const, quindi non modificabile. Verra' usata per creare la stringa offuscata.
	 * @return Stringa offuscata da asterischi, lasciando intatti primo e ultimo carattere.
	 */
	const char* oscuraPassword( const char* password );



	/**
	 * Controlla se la stringa data come parametro e' considerabile un numero, cioe' se e' convertibile in numero senza avere caratteri di scarto.
	 *
	 * @param vet Stringa da controllare.
	 * @param isFloat Flag per indicare se stiamo controllando un numero intero o un numero float.
	 * @return true Se la stringa e' considerabile un numero;
	 * @return false Se la stringa non e' considerabile un numero.
	 */
	bool areDigits ( const char vet[], bool isFloat );






	/*
	 * FUNZIONI PER LA CONVERSIONE
	 */

	/**
	 * Restituisce la stringa data in input, ma con ogni lettera resa minuscola.
	 *
	 * @param vet Stringa con qualificatore const, quindi non modificabile. Usata per poter restituire la stringa 'abbassata'.
	 * @return Una stringa uguale a quella data da parametro, ma con tutte le lettere rese minuscole.
	 */
	const char* toLower( const char vet[] );






	/*
	 * FUNZIONI PER L'INPUT
	 */

	/**
	 * Chiede un input testuale di dimensione massima data nel parametro, stampando il testo dato come parametro
	 *
	 * @param len Lunghezza massima che la stringa da prendere in input puo' avere
	 * @param testo Testo stampato a schermo per incitare l'utente a effettuare l'input
	 * @return Stringa presa in input
	 * @warning Verra' richiesto l'input all'utente finche' non inserisce una stringa di dimensione minore o uguale di len
	 */
	const char* inputStringa(int len, const char* testo);



	/**
	 * Chiede un input numerico compreso nell'intervallo dato nel parametro, stampando il testo dato come parametro
	 *
	 * @param min Valore minimo che l'utente deve inserire
	 * @param max Valore massimo che l'utente
	 * @param testo Testo stampato a schermo per incitare l'utente a effettuare l'input
	 * @return Intero preso in input
	 * @warning Verra' richiesto l'input all'utente finche' non inserisce valori corretti
	 */
	int inputIntero(int min, int max, const char* testo);



	/**
	 * Chiede un input numerico compreso nell'intervallo dato nel parametro, stampando il testo dato come parametro
	 *
	 * @param min Valore minimo che l'utente deve inserire
	 * @param max Valore massimo che l'utente
	 * @param testo Testo stampato a schermo per incitare l'utente a effettuare l'input
	 * @return Float preso in input
	 * @warning Verra' richiesto l'input all'utente finche' non inserisce valori corretti
	 */
	float inputFloat(float min, float max, const char* testo);



	/**
	 * Genera una One Time Password e la chiede in input, una sorta di "Dimostra di non essere un robot".
	 *
	 * @return 0 Se l'esito dell'input e' negativo;
	 * @return 1 Se l'input e' stato effettuato correttamente.
	 */
	int inputOTP();






	/*
	 * FUNZIONI PER IL CONTROLLO CORRETTEZZA STRINGHE
	 */

	/**
	 * Effettua il test di correttezza email sulla stringa data in input, ritorna 0 se supera il test, altrimenti ritorna un non-zero.
	 * Una email e' corretta se ha almeno un carattere prima della @ seguita dal dominio, il dominio e' corretto se ha almeno un carattere prima e dopo del punto
	 *
	 * @param email Stringa su cui effettuare il controllo
	 * @return 0 Se la stringa in parametro e' effettivamente una email
	 * @return 1 Se non c'e' una sola @
	 * @return 2 Se manca il punto identificatore del dominio
	 * @return 3 Se non ci sono caratteri prima o dopo della @ o dopo il punto del dominio
	 * @return 4 Se ci sono caratteri non ammessi nella email, cioe' il punto-e-virgola e lo spazio
	 */
	int controllaEmail( const char* email );



	/**
	 * Effettua il test di correttezza password sulla stringa data in input, ritorna 0 se supera il test, altrimenti ritorna un non-zero in base al tipo di errore.
	 * Una password e' corretta se contiene almeno un numero, almeno una lettera minuscola, almeno una lettera maiuscola e almeno un carattere speciale che non sia il punto-e-virgola
	 *
	 * @param password Stringa su cui effettuare il controllo
	 * @return 0 Se la stringa rispetta i requisiti di sicurezza per essere usata come password
	 * @return 1 Se ci sono caratteri speciali non concessi
	 * @return 2 Se non ci sono caratteri speciali
	 * @return 3 Se non ci sono numeri
	 * @return 4 Se non ci sono lettere maiuscole
	 * @return 5 Se non ci sono lettere minuscole
	 */
	int controllaPassword( const char* password );



	/**
	 * Effettua un test di correttezza del parametro per poter essere usato come nome o cognome, ritorna 0 se supera il test, altrimenti ritorna un non-zero in base al tipo di errore.
	 * Il test viene superato solo se nella stringa ci sono soltanto lettere maiuscole o minuscole.
	 *
	 * @param stringa Stringa su cui effettuare il controllo
	 * @return 0 Se la stringa rispetta i requisiti
	 * @return 1 Se ci sono numeri
	 * @return 2 Se ci sono caratteri speciali
	 */
	int testAnagrafica( char* stringa );






	/*
	 * FUNZIONI PER LA GENERAZIONE RANDOM
	 */

	/** Genera random un numero compreso tra min e max e lo restituisce
	 *
	 * @param min Valore minimo generabile
	 * @param max Valore massimo generabile
	 * @return Valore intero casuale compreso tra min e max
	 */
	int genRandom( int min, int max );



	/** Genera e restituisce una OTP composta da sole lettere maiuscole, di dimensione pari al valore del parametro
	 *
	 * @param dim Lunghezza della OPT da generare
	 * @return Stringa composta da una combinazione casuale di lettere maiuscole casuali
	 */
	char* genOTP( unsigned int dim );






	/*
	 * FUNZIONI PER FILES
	 */

	/**
	 * Controlla se un file esiste, avendo il suo percorso in parametro.
	 *
	 * @param nomeFile Percorso del file da controllare.
	 * @return true Se il file e' realmente esistente su disco;
	 * @return false Se il file non esiste.
	 */
	bool isFileExists( char nomeFile[] );





#endif /* TOOLS_H_*/
