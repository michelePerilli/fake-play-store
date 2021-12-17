/*
 * CronologiaHead.h
 *
 *
 *      Author: FRANCESCO SOPRANO and MICHELE PERILLI
 */

#include "AccountHead.h"

#include "AppHead.h"

#ifndef CRONOLOGIAHEAD_H_

	#define CRONOLOGIAHEAD_H_


	/// Dimensione della stringa che la fgets leggera' nel file da backup per poter creare poi il record per il file binario.
	#define DIMENSIONE_RIGA_DA_LEGGERE 200

	/// Percorso relativo del file binario di Cronologia
	#define CRONOLOGIA_SP "Database\\Cronologia.sp"

	/// Percorso relativo del file binario di Cronologia usato per annullare le modifiche effettuare.
	#define CRONOLOGIA_SP_COPIA "Database\\Cronologia.sp_"

	/// Percorso relativo del file testuale di backup di Cronologia
	#define CRONOLOGIA_BAK "Database\\Cronologia.bak"





	/// Struct capace di racchiudere l'identificativo di utente e app, e quindi usata per conservare le loro associazioni, cioe' installazione di app e voto.
	typedef struct
	{
		/// Identificativo dell'utente, cioe' la sua email.
		char email[LUNGHEZZA_EMAIL];

		/// Identificativo dell'app, cioe' il suo nome.
		char nomeApp[LUNGHEZZA_TITOLO];

		/// Flag di installazione, se e' false e' stata disinstallata, altrimenti e' attualmente installata.
		bool isInstalled;

		/// Voto dato all'app in questione dall'utente.
		Voto voto;

	} Cronologia;





	/// Una struct contente un vettore variabile di interi, quindi la struct contiene il vettore in se' e la sua dimensione.
	typedef struct
	{
		/// Dimensione del vettore di interi.
		int lunghezza;

		/// Vettore di interi che conterra' in ogni cella, il numero di riga riferito al file cronologia.
		int* posizioni;

	} ElencoCronologia;





#endif /* CRONOLOGIAHEAD_H_ */
