- [[#Requisiti utente|Requisiti utente]]
- [[#Requisiti di sistema|Requisiti di sistema]]
	- [[#Requisiti di sistema#Requisiti funzionali e non funzionali|Requisiti funzionali e non funzionali]]
	- [[#Requisiti di sistema#Architettura del sistema|Architettura del sistema]]

---
# Chiedere ai ragazzi:
- [/] gli utenti possono modificare il profilo?
- [/] esistono le categorie di prodotto? 
- [/] i fornitori possono modificare le schede dei prodotti?
- [/] negli use-case, si devono dividere utenti registrati e non registrati?
- [/] far controllare use-case
- [/] fornitori, clienti e trasportatori hanno server diversi o ce n'è solo uno?
# Descrizione generale
Il il nostro progetto è il back-end di un sito di e-commerce, implementato in C++ e tramite l’utilizzo di un database Redis. Il sistema funge da triplice interfaccia per clienti, fornitori e trasportatori che vogliono accedere al sito.
La piattaforma e-commerce, che consente ai fornitore di vendere prodotti direttamente ai clienti privati.
I consumatori, (acquirenti) devono potersi registrare e fornire alcune informazioni personali come nome, indirizzo email, e password, e ==modificarle in un secondo momento==. Inoltre, il sistema deve consentire agli utenti di cercare e filtrare prodotti in base a ==diverse categorie e criteri==. I clienti possono visualizzare i dettagli di ogni prodotto, come nome e descrizione, e possono acquistarli in diverse quantità. Infine, gli acquirenti devono poter controllare lo stato dell'ordine, tracciare la consegna, e all'arrivo, lasciare recensioni e valutazioni sui prodotti.
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