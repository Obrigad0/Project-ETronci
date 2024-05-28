- [[#Requisiti utente|Requisiti utente]]
- [[#Requisiti di sistema|Requisiti di sistema]]
	- [[#Requisiti di sistema#Requisiti funzionali e non funzionali|Requisiti funzionali e non funzionali]]
	- [[#Requisiti di sistema#Architettura del sistema|Architettura del sistema]]

---
# Chiedere ai ragazzi:
- [x] gli utenti possono modificare il profilo?
- [/] esistono le categorie di prodotto? 
- [x] i fornitori possono modificare le schede dei prodotti?
- [/] negli use-case, si devono dividere utenti registrati e non registrati?
- [/] far controllare use-case
- [/] fornitori, clienti e trasportatori hanno server diversi o ce n'è solo uno?
- [/] farsi dire cosa mettere nell'activity diagram
- [/] farsi dire cosa mettere nello state diagram
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
		3. email
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
		3. email
		4. password
	2. Gestione spedizioni
		1. Accettare spedizione
		2. Aggiornare stato di spedizione
		3. Visualizzare ordini pendenti
#### Entity-Relationship diagram
![[ETronci ER.png]]
#### Use-case diagram
![[use-case diagram (draft).png]]

---
### Requisiti di sistema
#### Requisiti funzionali e non funzionali
1. Cliente
	1. Effettuare acquisti
		1. Ogni acquisto deve essere effettuato da un cliente
		2. Ogni acquisto deve essere spedito al''indirizzo indicato dal cliente che ha effettuato l'acquisto.
	2. Visualizzare ordini
		1. I clienti possono 
#### Architettura del sistema
![[architettura fisica del sistema.png]]
Nel nostro progetto, abbiamo usato un approccio che si basa sulla separazione delle richieste. Abbiamo creato tre diversi server, ognuno dedicato a un diverso tipo di utente, e che gestisce esclusivamente le richieste del corrispettivo tipo di utente: uno per i clienti, uno per i fornitori e uno per i trasportatori. I tre sever vengono eseguiti parallelamente.
I tre server sono suddivisi in due componenti, uno chiamato `Main`, che si occupa della gestione delle richieste tra i vari processi in esecuzione sul server e uno chiamato `_/function` che si occupa della gestione delle richieste inviate e ricevute da ogni utente (di quel tipo) del sistema.
![[Customer request handler.png]]![[Seller request handler.png]]![[Courier request handler.png]]
> Ciascuno di tali processi viene denominato Manager. Ogni Manager è una
macchina a stati finiti (vedere sezioni successive) che rimane in attesa di
ricevere una richiesta da parte dell’Handler associato, per poi processare tale
richiesta interagendo con il database ed infine restituire l’esito all’Handler.

#### Activity diagram
...
Questo activity diagram riguarda l'invio di una richiesta da parte di un client, la sua elaborazione e l'invio della risposta da parte del server.
Questo processo è uguale per i tre server, indipendentemente dal tipo di richiesta del client.
È importante ricordare che nello scenario descritto dal diagramma, stiamo assumendo che il client abbia già stabilito la connessione con il server e che il server sia già attivo e pronto a ricevere e gestire la richiesta.
![[activitydiagram]]
#### State diagram
> Come precedentemente accennato, ogni Manager in esecuzione su uno dei
tre server corrisponde ad una macchina a stati finiti, la quale alterna tra
uno stato di ricezione, stato di elaborazione e stato di invio della risposta.
Tale comportamento risulta comune per tutti i Manager, senza alcuna azione
aggiuntiva rispetto agli altri. In particolare, è opportuno notare che ogni Manager rimanga in esecuzione permanente fino al momento in cui venga ricevuto un segnale di terminazione da parte del server. Una volta ricevuto tale segnale, ogni richiesta attualmente in elaborazione verrà scartata, senza comunicare alcun riscontro all’Handler associato.

???
#### Message sequence chart
...
![[messagesequenceshart]]
### Implementazione del software
### Struttura del codice
...
_pseudocodice_
### Struttura delle connessioni redis
...
### Schema del database
![[ETronci ER 1.png]]
### Monitor funzionali
...
esempio trigger con codice e spiegazione (3) e dove si trovano
### Monitor non-funzionali
codice monitor non funzionale
### Risultati sperimentali
I risultati riscontrati tramite il test generator mostrano che la piattaforma è in grado di gestire 