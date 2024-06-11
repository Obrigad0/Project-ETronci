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
- [/] esistono le categorie di prodotto? 
- [/] negli use-case, si devono dividere utenti registrati e non registrati?
- [/] far controllare use-case
- [ ] far controllare pseudocodice di handler, manager e server
# Punti della relazione
- [x] descrizione generale
- [x] requisiti utente
- [x] er diagram
- [x] use case diagram
- [ ] requisiti funzionali e non funzionali
- [x] architettura del sistema (grafico)
- [x] activity diagram
- [x] state diagram
- [x] message sequence chart
- [x] struttura del codice
- [x] pseudocodice :(
- [x] struttura delle connessioni redis
- [ ] monitor funzionali
- [ ] monitor non-funzionali
- [ ] risultati sperimentali
# Controllare se mancano
- [ ] monitor funzionale (3)
- [ ] monitor non funzionale  (2)?
- [ ] test generator ?
# Descrizione generale
Il il nostro progetto è il back-end di un sito di e-commerce, implementato in C++ e tramite l’utilizzo di un database Redis. Il sistema funge da triplice interfaccia per clienti, fornitori e trasportatori che vogliono accedere al sito.
La piattaforma e-commerce, che consente ai fornitore di vendere prodotti direttamente ai clienti privati.
I consumatori, (acquirenti) devono potersi registrare e fornire alcune informazioni personali come nome, indirizzo email, e password. Inoltre, il sistema deve consentire agli utenti di cercare e filtrare prodotti in base a ==diverse categorie e criteri==. I clienti possono visualizzare i dettagli di ogni prodotto, come nome e descrizione, e possono acquistarli in diverse quantità. Infine, gli acquirenti devono poter controllare lo stato dell'ordine, tracciare la consegna, e all'arrivo, lasciare recensioni e valutazioni sui prodotti.
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
		1. Ogni acquisto deve essere effettuato da un cliente
		2. Ogni acquisto deve essere spedito al''indirizzo indicato dal cliente che ha effettuato l'acquisto.
	2. Visualizzare ordini
		1. I clienti possono
> da finire
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
==Ognuno dei tre server è stato implementato tramite un'unica classe Server, che si interfaccia con client esterni e con il sotto-modulo (handler) associato.==
Ogni server (nel codice `main`) contiene la lista delle richieste che è in grado di gestire, che corrispondono alle funzioni associate al server. Se ad un server viene inviata una richiesta non prevista dal proprio gestore viene restituito un messaggio di "bad request". Infine, il database dei log contiene le risposte e le richieste relative a ogni server.
###### Pseudocodice dei server
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
					inviaAClient("BadRequest"); //?

		risposte = checkRisposte();

		se ci sono risposte: 
			per ogni risposta:
				inviaAClient(risposta);
```
###### Pseudocodice degli handler
```
Component Handler():
	richiesta = riceviRichiestaDalServer();
	tipo_richiesta getTipoRichiesta();
	
	se tipo_richiesta non è valido:
		return false
	altrimenti: 
		inviaAllaFunzione(richiessta); //?
		return true
```
###### Pseudocodice delle funzioni
```
Process Function():
	collegaDB();
	collegaRedis();

	ripeti finchè non si riceve sigterm
		richiesta = aspettaRichiesta();
		
		se richiesta non è valida:
			invia ("bad request");  //?
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
I gestori di ogni server creano due stream redis per ogni ==funzione== che controlla.  Per fare un esempio, per la funzione `MakeOrder`, che permette a un cliente di effettuare un ordine, il gestore di richieste di Customer istanzia uno stream per la comunicazione ==dal gestore verso la funzione e uno dalla funzione verso il gestore.==
Per tenere traccia dei diversi utenti che si interfacciano con il sistema, ==ad ogni client viene assegnato un id al momento della connessione==. Questi id, che vengono inviati insieme ad ogni messaggio scambiato tra le due componenti, permettono ai gestori di sapere a chi inoltrare la risposta.
#### Schema del database
![[ETronci ER.png]]
#### Monitor funzionali
Nel progetto si trovano diversi monitor funzionali, qui implementati tramite trigger e situati nel database dei dati. Questi trigger si comportano come monitor attivi e impediscono che i dati che si trovano all'interno del database possano diventare inconsistenti o che possano smettere di rispettare i vincoli esterni.

> esempio trigger con codice e spiegazione (3) e dove si trovano
#### Monitor non-funzionali
> codice monitor non funzionale
### Risultati sperimentali
I risultati riscontrati tramite il test generator mostrano che la piattaforma è stabile e funzionante. ==Inoltre è in grado di gestire correttamente eventuali sovraccarichi dovuti alla parallelizzazione di richieste e possibili input errati o inconsistenti.==