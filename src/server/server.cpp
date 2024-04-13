#include "server.h"
int stop_server = FALSE;

// Definizione costruttore
Server::Server(const char* id, int port, const char* redis_ip, int redis_port, std::string client_requests[], int req_num){

    // Inizializzazioni variabili globali alla classe server.h, cosi da non doversele sempre portare dietro
    socket_server = socket(AF_INET, SOCK_STREAM, 0); //creazione del socket
        //AF_INET indica utilizzo di ip iPv4, SOCK_STREAM indica l'utilizzo di TCP

    if (socket_server < 0) { 
        //Errore nella creazione del socket
        throw std::runtime_error(std::string("Errore nella creazione del Socket per il server: ") + id);
    }
 
    int on = 1;   
    // Riassegnazione veloce del'indirizzo e della porta precedentemente usati da un altro server
    // Cosi da poter riavviare in caso di errore il server in modo piu rapido
    if(setsockopt(socket_server, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on)) < 0){
        throw std::runtime_error(std::string("Errore in setsockopt() nella creazione del Socket per il server: ") + id);
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
    serverConf.sin_port = htons(port); //porta del server
    serverConf.sin_addr.s_addr = INADDR_ANY; // accetta connessioni da qualsiasi interfaccia di rete


    //Associazione del socket al server
    if(bind(socket_server, (struct sockaddr*) &serverConf, sizeof(serverConf)) == -1){
        throw std::runtime_error(std::string("Errore nel binding del socket per il server: ") + id);
    }
    

    //Avvio ascolto connessioni in entrata
    if(listen(socket_server,MAX_CLIENT) == -1){
        throw std::runtime_error(std::string("Errore nell'ascolto dal socket per il server: ") + id);
    }

    //TO DO Crezione oggetto handler per la gestione delle funzioni e l'interazione con redis

}

// distruttore server
Server::~Server(){  // TO DO
    // aggiungere cose db
    // chiusura connessione 
}

void Server::run(){ // TO DO

}

void Server::connectNewClient(){ // TO DO

}

void Server::sendClientResponse(int client_id, std::string out_str){ // TO DO

}

