#include "server.h"
int stop_server = FALSE;

// Definizione costruttore
Server::Server(const char* id, int port, const char* redis_ip, int redis_port, std::string client_requests[], int req_num){

    // Inizializzazioni variabili globali alla classe server.h, cosi da non doversele sempre portare dietro
    server = id;
    socket_server = socket(AF_INET, SOCK_STREAM, 0); //creazione del socket
        //AF_INET indica utilizzo di ip iPv4, SOCK_STREAM indica l'utilizzo di TCP
    socket_port = port;

    if (socket_server < 0) { 
        //Errore nella creazione del socket
        throw std::runtime_error(std::string("Errore nella creazione del Socket per il server: ") + server);
    }
 
    int on = 1;   
    // Riassegnazione veloce del'indirizzo e della porta precedentemente usati da un altro server
    // Cosi da poter riavviare in caso di errore il server in modo piu rapido
    if(setsockopt(socket_server, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on)) < 0){
        throw std::runtime_error(std::string("Errore in setsockopt() nella creazione del Socket per il server: ") + server);
        close(socket_server);
        exit(-1);
    }

    // Imposto il socket in modalita' non bloccante
    int flags = fcntl(socket_server,F_GETFL, 0);
    flags |= O_NONBLOCK;

    //controllo errore se il socket non e' in modalita' non bloccante? TO DO

    //configurazione server

    sockaddr_in serverConf;
    serverConf.sin_family = AF_INET;    //IPv4
    serverConf.sin_port = htons(socket_port); //porta del server
    serverConf.sin_addr.s_addr = INADDR_ANY; // accetta connessioni da qualsiasi interfaccia di rete


    //Associazione del socket al server
    if(bind(socket_server, (struct sockaddr*) &serverConf, sizeof(serverConf)) == -1){
        throw std::runtime_error(std::string("Errore nel binding del socket per il server: ") + server);
    }
    

    //Avvio ascolto connessioni in entrata
    if(listen(socket_server,MAX_CLIENT) == -1){
        throw std::runtime_error(std::string("Errore nell'ascolto dal socket per il server: ") + server);
    }

    //TO DO Crezione oggetto handler per la gestione delle funzioni e l'interazione con redis

}


// distruttore server
Server::~Server(){  // TO DO
    db.disconnectFromDatabase();
    // chiusura connessione

    int i;
    char query[QUERY_LEN];

// Itera su tutti i file descriptor fino a max_fd
    for (i=0; i <= max_fd; ++i){
        // Verifica se il file descriptor i è presente in current_set
        if (FD_ISSET(i, &current_set)) {
            // Costruisce una query SQL per aggiornare la tabella Client
            // Imposta disconnection_instant al timestamp corrente
            // per il client identificato da server_name e file_descriptor
            // solo se disconnection_instant è attualmente NULL
            sprintf(query, "UPDATE Client SET disconnection_instant = CURRENT_TIMESTAMP WHERE server_name = \'%s\' AND file_descriptor = %d AND disconnection_instant IS NULL", server, i);

            // Esegue la query sul database utilizzando il meTO DO RunQuery dell'oggetto db
            // Il secondo parametro false indica che la query non restituirà risultati (è un UPDATE, non un SELECT)
            resp = db.RunQuery(query, false);

            // Chiude il file descriptor i
            close(i);
        }
    }
}


void Server::run(){

    struct sigaction sigHandler; //struttura sigaction per gestire i segnali

    fd_set working_set; // set di file descriptor
    int returnSistem; // valore di ritorno delle chiamate di sistema
    int numreq; // numero di richieste pronte

    struct timeval timeout;// specificare un timeout
    int idClient; // Dichiara una variabile intera client_id per memorizzare l'ID del client
    bool response;
    char query[QUERY_LEN];
    std::string respTocli; // risposta da inviare al client

    sigHandler.sa_handler = handle_signals;
    sigemptyset(&sigHandler.sa_mask);
    sigHandler.sa_flags = 0;
    // Imposta i campi della struttura signalHandler:
    // - sa_handler: punta alla funzione handle_signals per gestire i segnali
    // - sa_mask: imposta la maschera dei segnali a vuoto con sigemptyset
    // - sa_flags: imposta i flag a 0

    sigaction(SIGINT, &sigHandler, NULL);
    sigaction(SIGTERM, &sigHandler, NULL);
    // Imposta la gestione dei segnali SIGINT e SIGTERM utilizzando sigaction e signalHandler

    numreq = 0;
    FD_ZERO(&current_set);
    max_fd = socket_server;
    FD_SET(socket_server, &current_set);
    // Inizializza variabili e set di file descriptor:
    // - ready_requests: inizializzato a 0
    // - current_set: inizializzato a vuoto con FD_ZERO
    // - max_fd: inizializzato al file descriptor del socket del server
    // - Aggiunge socket_server a current_set con FD_SET

    timeout.tv_sec  = 0;
    timeout.tv_usec = 5000;
    // Imposta il timeout a 5000 microsecondi (5 millisecondi)

    std::cout << "Running " << server << " server" << std::endl;

    while(!stop_server) {
        // Ciclo principale del server, continua finché stop_server è false

        memcpy(&working_set, &current_set, sizeof(current_set));
        // Copia current_set in working_set utilizzando memcpy

        returnSistem = select(max_fd + 1, &working_set, NULL, NULL, &timeout);
        // Chiama select per attendere l'attività sui file descriptor in working_set

        if (returnSistem < 0) {
            std::cout << "\nSocket selection error" << std::endl;
            break;
        }
        // Se select restituisce un valore negativo, stampa un messaggio di errore ed esce dal ciclo

        numreq = returnSistem;

        for (int i = 0; i <= max_fd && numreq > 0; ++i) {
            // Ciclo per scorrere i file descriptor pronti

            if ((i != socket_server) && FD_ISSET(i, &working_set)) {
                numreq -= 1;
                receiveClientData(i);
            }
            // Se il file descriptor i non è il socket del server ed è pronto,
            // decrementa numreq e chiama receiveClientData per ricevere i dati dal client
        }

        if (FD_ISSET(socket_server, &working_set)) {
            addNewClients();
        }
        // Se il socket del server è pronto, chiama addNewClients per accettare nuove connessioni client

        response = true;
        while(response){
            respTocli = "";
            idClient = -1;

            response = handler->read_from_managers(&respTocli, &idClient);

            if(response){
                sendClientResponse(idClient, respTocli);
            }
        }
        // Ciclo per gestire le risposte ai client:
        // - Inizializza response a una stringa vuota e idCLient a -1
        // - Chiama handler->read_from_managers per ottenere una risposta e l'ID del client
        // - Se c'è una risposta, chiama sendClientResponse per inviarla al client

        timeout.tv_sec  = 1;
        timeout.tv_usec = 0;
        // Imposta il timeout a 1 secondo
    }

    chiudiConnessione();

}

void Server::chiudiConnessione() {

    int i;
    char query[QUERY_LEN];

// Itera su tutti i file descriptor fino a max_fd
    for (i=0; i <= max_fd; ++i){
        // Verifica se il file descriptor i è presente in current_set
        if (FD_ISSET(i, &current_set)) {
            // Costruisce una query SQL per aggiornare la tabella Client
            // Imposta disconnection_instant al timestamp corrente
            // per il client identificato da server_name e file_descriptor
            // solo se disconnection_instant è attualmente NULL
            sprintf(query, "UPDATE Client SET disconnection_instant = CURRENT_TIMESTAMP WHERE server_name = \'%s\' AND file_descriptor = %d AND disconnection_instant IS NULL", server, i); //TO DO CAMBIARE QUESTTE QUERY PER ADATARLE AL DB

            // Esegue la query sul database utilizzando il meTO DO RunQuery dell'oggetto db
            // Il secondo parametro false indica che la query non restituirà risultati (è un UPDATE, non un SELECT)
            resp = db.RunQuery(query, false);

            // Chiude il file descriptor i
            close(i);
        }
    }
}

void Server::addNewClients(){
    // questo loop ascolta costantemente per nuove connessioni in entrata e le gestisce quando arrivano, 
    // registrando i nuovi client nel database e aggiungendo i loro file descriptor al set attivo del server. 
    // Una volta che non ci sono più connessioni in arrivo, il loop termina.
    int newClient;
    char query[QUERY_LEN]; //query len si trova in una cartella contente un file con tutte costanti. TO DO

    // nicolas ho rimesso il do while perche senno' non funzionava. Meglio ladri che poveri. TO DO LEVARE COMMENTO
    do{
        newClient = accept(socket_server, NULL, NULL); // accetta nuove connessioni in entrata
        
        if (newClient < 0) { // c'è stato un errore, stacca stacca

            if (errno != EWOULDBLOCK) {
                std::cout << "\nErrore, Client non accettato\n" << std::endl;  // ho messo \n alla fine invece che all'inizio, è piu carino? fla:Molto carino TO DO L.C.
                stop_server = TRUE;
            }
            break;
        }

        // accept ha avuto successo, bisogna registrare nel database l'accesso del client
        sprintf(query, "INSERT INTO Client(server_name, file_descriptor, connection_instant) VALUES (\'%s\', %d, CURRENT_TIMESTAMP)", server, newClient);//TO DO CAMBIARE QUESTTE QUERY PER ADATARLE AL DB
        resp = db.RunQuery(query, false);

        if (PQresultStatus(resp) != PGRES_COMMAND_OK && PQresultStatus(resp) != PGRES_TUPLES_OK) {
            send(newClient, "SERVER_ERROR", 12, 0);
            close(newClient);
            return;
        }

        FD_SET(newClient, &current_set); // aggiungo il file descriptor del nuovo client al set current_set. (per gestione \ monitoraggio)
        if (newClient > max_fd) { // il valore di max_fd, che rappresenta il file descriptor più grande attualmente gestito dal server.
            max_fd = newClient;
        }
        if (fcntl(newClient, F_SETFL, O_NONBLOCK) == -1) { // imposto il socket del client come non bloccante
            std::cout << "\nClient accept failed\n" << std::endl; // pure qui ho spostato \n :3
        }
    }while (newClient != -1);
}


void Server::sendClientResponse(int idClient, std::string msg){

    char query[QUERY_LEN];
    size_t index = msg.find('\n'); // Cerca la posizione del carattere di nuova riga '\n' all'interno della stringa msg
    // size_t è un tipo di dato intero senza segno utilizzato per rappresentare dimensioni e indici

    std::string primaRiga = msg.substr(0, index); // Estrae la sottostringa di msg dall'inizio fino all'indice trovato (escluso)
    // Questa sottostringa rappresenta la prima riga del messaggio

    sprintf(query, "WITH max_client_conn AS (SELECT max(connection_instant) AS instant FROM Client WHERE server_name = \'%s\' AND file_descriptor = %d), "
                   "     last_request AS (SELECT MAX(c.request_instant) AS instant FROM Communication AS c, max_client_conn AS m WHERE c.client_server_name = \'%s\' AND c.client_file_descriptor = %d AND c.client_connection_instant = m.instant) "
                   "UPDATE Communication SET response_status = \'%s\', response_instant = CURRENT_TIMESTAMP "
                   "WHERE client_server_name = \'%s\' AND client_file_descriptor = %d AND client_connection_instant = (SELECT instant FROM max_client_conn) AND request_instant = (SELECT instant FROM last_request)",
            server, idClient, server, idClient, primaRiga.c_str(), server, idClient);

    // TO DO: bisogna modificare questa query per adattarla al nostro db
    resp = db.RunQuery(query, false);

    // Verifica se il risultato della query indica un errore
    if (PQresultStatus(resp) != PGRES_COMMAND_OK && PQresultStatus(resp) != PGRES_TUPLES_OK) {
        send(idClient, "SERVER_ERROR", 12, 0); // Invia un messaggio di errore al client
        return;
    }

    std::cout << "\nClient id: " << idClient << " - Response: " << msg << std::endl; // Stampa l'ID del client e il messaggio di risposta
    send(idClient, msg.c_str(), msg.length(), 0); // Invia il messaggio di risposta al client
}

void Server::receiveClientData(int i){

    std::string msg; // Variabile di tipo stringa per memorizzare il messaggio
    char buffer[100]; // Array di caratteri per ricevere i dati dal socket
    int nb; // Variabile intera per memorizzare il numero di byte ricevuti
    int stacca = FALSE; // Flag booleano per indicare la disconnessione del client
    char query[QUERY_LEN]; // Array di caratteri per memorizzare la query SQL

    do {
        bzero(buffer, sizeof(buffer)); // Inizializza il buffer a zero
        nb = recv(i, buffer, sizeof(buffer) - 1, 0); // Riceve i dati dal socket 'i' e li memorizza nel buffer

        if (nb < 0) {
            if (errno != EWOULDBLOCK) {
                stacca = TRUE; // Imposta il flag 'stacca' a TRUE se si verifica un errore diverso da EWOULDBLOCK
            }
            break;
        }
        if (nb == 0) {
            stacca = TRUE; // Imposta il flag 'stacca' a TRUE se la connessione è stata chiusa
            break;
        }
        msg.append(buffer); // Aggiunge i dati ricevuti alla stringa 'msg'
    } while (TRUE);

    if (stacca) {
        // Aggiorna il timestamp di disconnessione del client nel database
        sprintf(query, "UPDATE Client SET disconnection_instant = CURRENT_TIMESTAMP WHERE server_name = \'%s\' AND file_descriptor = %d AND disconnection_instant IS NULL", server, i);
        // TO DO: modificare questa query per adattarla al nostro db
        resp = db.RunQuery(query, false); // Esegue la query sul database
        close(i); // Chiude il socket 'i'
        FD_CLR(i, &current_set); // Rimuove il descrittore di file 'i' dal set corrente
        if (i == max_fd) {
            while (FD_ISSET(max_fd, &current_set) == FALSE)
                max_fd -= 1; // Aggiorna il valore di 'max_fd' se 'i' era il descrittore di file massimo
        }
        return;
    }

// Inserisce una nuova comunicazione nel database
    sprintf(query, "INSERT INTO Communication(request, request_instant, client_server_name, client_file_descriptor, client_connection_instant)"
                   "VALUES (\'%s\', CURRENT_TIMESTAMP, \'%s\', %d, (SELECT connection_instant FROM Client WHERE disconnection_instant IS NULL and server_name = \'%s\' and file_descriptor = %d))", msg.c_str(), server, i, server, i);
// TO DO: modificare questa query per adattarla al nostro db
    resp = db.RunQuery(query, false); // Esegue la query sul database
    if (PQresultStatus(resp) != PGRES_COMMAND_OK && PQresultStatus(resp) != PGRES_TUPLES_OK) {
        send(i, "SERVER_ERROR", 12, 0); // Invia un messaggio di errore al client in caso di errore nella query
        return;
    }

    if (!handler->send_to_managers(i, msg)) {
        sendClientResponse(i, "BAD_REQUEST"); // Invia una risposta "BAD_REQUEST" al client se l'invio ai gestori fallisce
    }

}


