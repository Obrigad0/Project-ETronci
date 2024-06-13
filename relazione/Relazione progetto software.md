- [[#Requisiti utente|Requisiti utente]]
	- [[#Requisiti utente#Entity-Relationship diagram|Entity-Relationship diagram]]
	- [[#Requisiti utente#Use-case diagram|Use-case diagram]]
- [[#Requisiti di sistema|Requisiti di sistema]]
	- [[#Requisiti di sistema#Requisiti funzionali e non funzionali|Requisiti funzionali e non funzionali]]
	- [[#Requisiti di sistema#Architettura del sistema|Architettura del sistema]]
	- [[#Requisiti di sistema#Activity diagram|Activity diagram]]
	- [[#Requisiti di sistema#State diagram|State diagram]]
	- [[#Requisiti di sistema#Message sequence chart|Message sequence chart]]
- [[#Implementazione del software|Implementazione del software]]
	- [[#Implementazione del software#Struttura del codice|Struttura del codice]]
	- [[#Implementazione del software#Struttura delle connessioni redis|Struttura delle connessioni redis]]
	- [[#Implementazione del software#Schema del database|Schema del database]]
	- [[#Implementazione del software#Monitor funzionali|Monitor funzionali]]
	- [[#Implementazione del software#Monitor non-funzionali|Monitor non-funzionali]]
- [[#Risultati sperimentali|Risultati sperimentali]]


---
# Chiedere ai ragazzi:
- [x] gli utenti possono modificare il profilo?
- [x] esistono le categorie di prodotto? 
- [x] negli use-case, si devono dividere utenti registrati e non registrati? - chiedere a nicolas
- [x] far controllare use-case
- [x] far controllare pseudocodice di handler, manager e server
# Punti della relazione
- [x] descrizione generale
- [x] requisiti utente
- [x] er diagram
- [x] use case diagram
- [x] requisiti funzionali e non funzionali
- [x] architettura del sistema (grafico)
- [x] activity diagram
- [x] state diagram
- [x] message sequence chart
- [x] struttura del codice
- [x] pseudocodice :(
- [x] struttura delle connessioni redis
- [x] monitor funzionali
- [x] monitor non-funzionali
- [x] risultati sperimentali
# Controllare se mancano
- [x] monitor funzionale (3)
- [x] monitor non funzionale  (2)?
- [x] test generator ?
# Descrizione generale
Il il nostro progetto è il back-end di un sito di e-commerce, implementato in C++ e tramite l’utilizzo di un database Redis. Il sistema funge da triplice interfaccia per clienti, fornitori e trasportatori che vogliono accedere al sito.
La piattaforma e-commerce, che consente ai fornitore di vendere prodotti direttamente ai clienti privati.
I consumatori, (acquirenti) devono potersi registrare e fornire alcune informazioni personali come nome, indirizzo email, e password. Inoltre, il sistema deve consentire agli utenti di cercare i prodotti. I clienti possono visualizzare i dettagli di ogni prodotto, come nome e descrizione, e possono acquistarli in diverse quantità. Infine, gli acquirenti devono poter controllare lo stato dell'ordine, tracciare la consegna, e all'arrivo, lasciare recensioni e valutazioni sui prodotti.
I fornitori (venditori) possono creare account e accedere al sito, fornendo informazioni personali. Possono anche visualizzare, creare e modificare le schede dei prodotti.
Infine i trasportatori hanno accesso alla gestione delle spedizioni di ordini incaricate a loro. Possono visualizzarle e gestirne lo stato.

---
# Analisi del software
### Requisiti utente
1. Cliente
	1. Registrazione alla piattaforma
		1. id
		2. email (unique)
		3. password
	2. Effettuare ordini
		1. quantità
		2. indirizzo di consegna
	3. Controllare ordini
	4. Ricerca di prodotti
	5. Recensione di prodotti ordinati
2. Fornitore
	1. Registrazione alla piattaforma
		1. id
		2. nome
		3. email (unique)
		4. password
	2. Gestione prodotti (creazione e modifica)
		1. id
		2. nome
		3. descrizione
		4. price_tag
3. Trasportatore
	1. Registrazione alla piattaforma
		1. id
		2. nome
		3. email (unique)
		4. password
	2. Gestione spedizioni
		1. Accettare spedizione
		2. Aggiornare stato di spedizione
		3. Visualizzare ordini pendenti
#### Entity-Relationship diagram
![[01 - Università/relazione/immagini relazione/ETronci ER.png]]
#### Use-case diagram
![[01 - Università/relazione/immagini relazione/use-case diagram (draft).png]]

---
### Requisiti di sistema
#### Requisiti funzionali e non funzionali
1. Cliente
	1. Effettuare acquisti
		1. Ogni acquisto deve essere effettuato da un cliente.
		2. Ogni acquisto deve essere spedito al''indirizzo indicato dal cliente che ha effettuato l'acquisto.
	2. Recensire prodotti
		1. Ogni prodotto può essere recensito solo da un cliente che l'ha acquistato e ricevuto.
2. Venditori
	1. Gestire prodotti
		1. Ogni prodotto appartiene a un solo venditore e solo lui può modificarlo.
3. Trasportatori
	1. Gestione ordini
		1. Ogni corriere può accettare un ordine solo nel magazzino in cui lavora.
		2. Ogni corriere può cambiare lo stato di consegna solo in ordini a lui assegnati.
4. Requisiti non funzionali
	1. Il tempo medio di sessione di un client deve essere inferiore o uguale al tempo massimo di connessione.
	2. Il tempo medio di risposta del server ad una richiesta da parte di un client deve essere inferiore o uguale al tempo massimo di risposta.
#### Architettura del sistema
![[01 - Università/relazione/immagini relazione/architettura fisica del sistema.png]]
Nel nostro progetto, abbiamo usato un approccio che si basa sulla separazione delle richieste. Abbiamo creato tre diversi server, ognuno dedicato a un diverso tipo di utente, e che gestisce esclusivamente le richieste del corrispettivo tipo di utente: uno per i clienti, uno per i fornitori e uno per i trasportatori. I tre sever vengono eseguiti parallelamente.
I tre server sono suddivisi in due componenti, uno chiamato `Main`, che si occupa della gestione delle richieste tra i vari processi in esecuzione sul server e uno chiamato `_/function` che si occupa della gestione delle richieste inviate e ricevute da ogni utente (di quel tipo) del sistema.
![[Customer request handler.png]]![[Seller request handler.png]]![[Courier request handler.png]]
Ognuno di questi processi si trova nella directory `functions`. Rimane in attesa di ricevere una richiesta da parte del suo gestore e poi elabora la richiesta tramite interazione con il database, per poi restituire il risultato al gestore.
#### Activity diagram
Questo activity diagram riguarda l'invio di una richiesta da parte di un client, la sua elaborazione e l'invio della risposta da parte del server.
Questo processo è uguale per i tre server, indipendentemente dal tipo di richiesta del client.
È importante ricordare che nello scenario descritto dal diagramma, stiamo assumendo che il client abbia già stabilito la connessione con il server e che il server sia già attivo e pronto a ricevere e gestire la richiesta.
![[01 - Università/relazione/immagini relazione/Activity diagram.png]]
#### State diagram
Questa è lo state diagram che rappresenta il funzionamento dei server. Il server (sia customer che seller e courier) non aspetta infinitamente una richiesta entrante, se non ci sono richieste controlla le eventuali risposte ricevute dalle funzioni associate.
![[State Diagram.png]]
#### Message sequence chart
Questo è il message sequence chart che rappresenta il comportamento del sistema quando una richiesta viene inviata dal client, elaborata e la risposta viene inviata dal server. Questo schema è valido per tutti e tre i server (customer, seller e courier) e non dipende dal tipo di richiesta. Come sopra, ricordiamo che nello scenario descritto dal diagramma, stiamo assumendo che il client abbia già stabilito la connessione con il server e che il server sia già attivo e pronto a ricevere e gestire la richiesta.
![[01 - Università/relazione/immagini relazione/Message sequence chart.png]]
### Implementazione del software
#### Struttura del codice
La struttura del codice è sostanzialmente divisa in tre parti: `customer`, `seller` e `courier`. Ogni parte contiene il necessario codice relativo al proprio server specifico e alle sue funzioni.
Ognuno dei tre server è stato implementato tramite un'unica classe Server, che si interfaccia con client esterni e con la funzione associata.
Ogni server (nel codice `main`) contiene la lista delle richieste che è in grado di gestire, che corrispondono alle funzioni associate al server. Se ad un server viene inviata una richiesta non prevista dal proprio gestore viene restituito un messaggio di "bad request". Infine, il database dei log contiene le risposte e le richieste relative a ogni server.
###### Pseudocodice dei server:
```
Process Server ():
	setupMain();
	
	ripeti finchè non si riceve sigterm:
		richieste = checkRichieste();
		accettaConnessioniEntranti();

		se ci sono richieste:
			per ogni richiesta attività:
				logRichiesta(richiesta);
				isRichiestaValida = inviaRichiestaAMain(richiesta);
				se isRichiestaValida == falso:
					inviaAClient("BadRequest");

		risposte = checkRisposte();

		se ci sono risposte: 
			per ogni risposta:
				inviaAClient(risposta);
```
###### Pseudocodice dei gestori (main):
```
Component Main():
	richiesta = riceviRichiestaDalServer();
	tipo_richiesta getTipoRichiesta();
	
	se tipo_richiesta non è valido:
		return false
	altrimenti: 
		inviaAllaFunzione(richiessta);
		return true
```
###### Pseudocodice delle funzioni:
```
Process Function():
	collegaDB();
	collegaRedis();

	ripeti finchè non si riceve sigterm
		richiesta = aspettaRichiesta();
		
		se richiesta non è valida:
			invia ("bad request"); 
		altrimenti:
			query = convertiRichiesta();
			risultato = queryDB(query);
			
			se risultato non è valido:
				invia ("bad request");
			altrimenti:
				risposta = formattaRisposta(risultato);
				invia(risposta);
```

#### Struttura delle connessioni redis
I gestori di ogni server creano due stream redis per ogni funzione che controlla.  Per fare un esempio, per la funzione `MakeOrder`, che permette a un cliente di effettuare un ordine, il gestore di richieste di Customer istanzia uno stream per la comunicazione dal gestore verso la funzione e uno dalla funzione verso il gestore.
Per tenere traccia dei diversi utenti che si interfacciano con il sistema, ad ogni client viene assegnato un id al momento della connessione. Questi id, che vengono inviati insieme ad ogni messaggio scambiato tra le due componenti, permettono ai gestori di sapere a chi inoltrare la risposta.
#### Schema del database
![[ETronci ER.png]]
#### Monitor funzionali
Nel progetto si trovano diversi monitor funzionali, qui implementati tramite trigger e situati nel database dei dati. Questi trigger si comportano come monitor attivi e impediscono che i dati che si trovano all'interno del database possano diventare inconsistenti o che possano smettere di rispettare i vincoli esterni.
Di seguito riportiamo il codice di tre monitor funzionali:
- `src/service/logDatabase/logTrig.sql`
```sql
\c :log_dbname

-- drop all functions and triggers
DO $$
DECLARE
function_name TEXT;
BEGIN
FOR function_name IN
SELECT p.proname
FROM pg_proc p
         INNER JOIN pg_namespace n ON p.pronamespace = n.oid
WHERE n.nspname = 'public'
    LOOP
        EXECUTE 'DROP FUNCTION IF EXISTS ' || function_name || ' CASCADE';
END LOOP;
END $$;

CREATE OR REPLACE FUNCTION consistent_file_descriptor() RETURNS TRIGGER AS $$
BEGIN
    IF EXISTS (
        SELECT 1
        FROM Client
        WHERE NEW.server_name = Client.server_name
        AND NEW.file_descriptor = Client.file_descriptor
        AND (Client.disconnection_instant IS NULL OR NEW.connection_instant <= Client.disconnection_instant)
    ) IS TRUE THEN
        RAISE EXCEPTION 'file descriptor connection not closed';
END IF;

RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER consistent_file_descriptor_trg
    BEFORE INSERT ON Client FOR EACH ROW EXECUTE FUNCTION consistent_file_descriptor();
```
- `src/service/database/db-utils/databaseTrig.sql`
	```sql
\c :dbname

-- 1. trigger sulla tabella delivery

CREATE OR REPLACE FUNCTION check_courier_order_warehouse() RETURNS TRIGGER AS $$
BEGIN
    -- Verifica che il corriere stia accettando un ordine dalla stessa warehouse in cui lavora
    IF NOT EXISTS (
        SELECT 1
        FROM OrderedProduct op
        JOIN Product p ON op.product = p.id
        JOIN Courier c ON NEW.courier = c.id
        WHERE NEW.orderId = op.id
        AND p.warehouse = c.warehouse
    ) THEN
        RAISE EXCEPTION 'Courier and order must be from the same warehouse before accepting a delivery';
    END IF;
    
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER check_courier_order_warehouse_trg
BEFORE INSERT ON Delivery
FOR EACH ROW
EXECUTE FUNCTION check_courier_order_warehouse();

-- 2. trigger sulla tabella review

CREATE OR REPLACE FUNCTION check_review_validity() RETURNS TRIGGER AS $$
BEGIN
    -- Verifica se il cliente ha effettivamente ordinato (e ricevuto) il prodotto
    IF NOT EXISTS (
        SELECT 1
        FROM OrderedProduct op
        JOIN Delivery d ON op.id = d.orderId
        WHERE op.customer = NEW.customer
        AND op.product = NEW.product
        AND d.status = 'delivered'
    ) THEN
        RAISE EXCEPTION 'Customer can only review products they have ordered and received';
    END IF;

    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER check_review_validity_trg
BEFORE INSERT ON Review
FOR EACH ROW
EXECUTE FUNCTION check_review_validity();

```
#### Monitor non-funzionali
`src/service/logDatabase/main.cpp`
```cpp
// Connessione al database PostgreSQL
    DbConnection log_db = DbConnection(POSTGRESQL_SERVER, POSTGRESQL_PORT, POSTGRESQL_USER, POSTGRESQL_PSW, POSTGRESQL_DBNAME);
    PGresult *query_res; // Risultato della query

    char query[QUERYSIZE]; // Stringa per memorizzare la query

    while(1) {
        // Query per calcolare la media del tempo di disconnessione in millisecondi
        sprintf(query, "SELECT EXTRACT(EPOCH FROM AVG(disconnection_instant - connection_instant)) * 1000 as avg FROM Client WHERE disconnection_instant IS NOT NULL");

        // Esegue la query
        query_res = log_db.RunQuery(query, true);

        // Controlla se la query ha avuto successo e se ci sono risultati
        if ((PQresultStatus(query_res) != PGRES_COMMAND_OK && PQresultStatus(query_res) != PGRES_TUPLES_OK) || PQntuples(query_res) <= 0) {
            printf("DB_ERROR\n");
            continue;
        }

        // Ottiene il valore medio dalla query
        char* average = PQgetvalue(query_res, 0, PQfnumber(query_res, "avg"));

        // Se la media è vuota, imposta a "0"
        if(strlen(average)==0){
            sprintf(average, "0");
        }

        char response_status[8]; // Stato della risposta

        // Controlla se la media è inferiore o uguale al tempo massimo di connessione
        if (atof(average) <= MAX_CONNECTION_TIME_AVG) {
            sprintf(response_status, "SUCCESS");
        } else {
            sprintf(response_status, "ERROR");
        }

        // Inserisce le statistiche della sessione nel database
        sprintf(query, "INSERT INTO SessionStatistic(type, end_instant, value, response_status) VALUES ('Session', CURRENT_TIMESTAMP, %s, \'%s\')", average, response_status);

        // Esegue la query di inserimento
        query_res = log_db.RunQuery(query, false);

        // Controlla se la query di inserimento ha avuto successo
        if (PQresultStatus(query_res) != PGRES_COMMAND_OK && PQresultStatus(query_res) != PGRES_TUPLES_OK) {
            printf("DB_ERROR\n");
            continue;
        }

        // Query per calcolare la media del tempo di risposta in millisecondi
        sprintf(query, "SELECT EXTRACT(EPOCH FROM AVG(response_instant - request_instant)) * 1000 as avg FROM Communication WHERE response_instant IS NOT NULL");

        // Esegue la query
        query_res = log_db.RunQuery(query, true);

        // Controlla se la query ha avuto successo e se ci sono risultati
        if ((PQresultStatus(query_res) != PGRES_COMMAND_OK && PQresultStatus(query_res) != PGRES_TUPLES_OK) || PQntuples(query_res) <= 0) {
            printf("DB_ERROR\n");
            continue;
        }

        // Ottiene il valore medio dalla query
        average = PQgetvalue(query_res, 0, PQfnumber(query_res, "avg"));

        // Se la media è vuota, imposta a "0"
        if(strlen(average) == 0){
            sprintf(average, "0");
        }

        // Controlla se la media è inferiore o uguale al tempo massimo di risposta
        if (atof(average) <= MAX_RESPONSE_TIME_AVG) {
            sprintf(response_status, "SUCCESS");
        } else {
            sprintf(response_status, "ERROR");
        }

        // Inserisce le statistiche della risposta nel database
        sprintf(query, "INSERT INTO SessionStatistic(type, end_instant, value, response_status) VALUES ('Response', CURRENT_TIMESTAMP, %s, \'%s\')", average, response_status);

        // Esegue la query di inserimento
        query_res = log_db.RunQuery(query, false);

        // Controlla se la query di inserimento ha avuto successo
        if (PQresultStatus(query_res) != PGRES_COMMAND_OK && PQresultStatus(query_res) != PGRES_TUPLES_OK) {
            printf("DB_ERROR\n");
            continue;
        }

        // Sospende l'esecuzione per 60 secondi
        micro_sleep(60000000);
    }

    // Disconnette dal database
    log_db.disconnectFromDatabase();
```
### Risultati sperimentali
I risultati riscontrati tramite il test generator mostrano che la piattaforma è stabile e funzionante. Inoltre è in grado di gestire correttamente eventuali input errati o inconsistenti.