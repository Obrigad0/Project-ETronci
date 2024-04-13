#include "server.h"
int stop_server = FALSE;

// Definizione costruttore
Server::Server(const char* id, int port, const char* redis_ip, int redis_port, std::string client_requests[], int req_num){

    // Inizializzazioni variabili globali alla classe server.h, cosi da non doversele sempre portare dietro
    server = id
    socket_server = socket(AF_INET, SOCK_STREAM, 0); //creazione del socket
        //AF_INET indica utilizzo di ip iPv4, SOCK_STREAM indica l'utilizzo di TCP
    socket_port = port

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
    // aggiungere cose db
    // chiusura connessione 
}


void Server::run(){ // TO DO

}


void Server::addNewClients(){
    // questo loop ascolta costantemente per nuove connessioni in entrata e le gestisce quando arrivano, 
    // registrando i nuovi client nel database e aggiungendo i loro file descriptor al set attivo del server. 
    // Una volta che non ci sono più connessioni in arrivo, il loop termina.
    int new_client;
    char query[QUERY_LEN];

    // ho cambiato il do-while in un semplice while, così sembriamo più poveri
    while (new_client != -1) { 
        new_client = accept(sockfd, NULL, NULL); // accetta nuove connessioni in entrata
        
        if (new_client < 0) { // c'è stato un errore, stacca stacca

            if (errno != EWOULDBLOCK) {
                std::cout << "Client accept failed\n" << std::endl;  // ho messo \n alla fine invece che all'inizio, è piu carino?
                kill_server = TRUE;
            }
            break;
        }

        // accept ha avuto successo, bisogna registrare nel database l'accesso del client
        sprintf(query, "INSERT INTO Client(server_name, file_descriptor, connection_instant) VALUES (\'%s\', %d, CURRENT_TIMESTAMP)", server, new_client);
        res = log_db.RunQuery(query, false);

        if (PQresultStatus(res) != PGRES_COMMAND_OK && PQresultStatus(res) != PGRES_TUPLES_OK) {
            send(new_client, "SERVER_ERROR", 12, 0);
            close(new_client);
            return;
        }

        FD_SET(new_client, &current_set); // aggiungo il file descriptor del nuovo client al set current_set. (per gestione \ monitoraggio)
        if (new_client > max_fd) { // il valore di max_fd, che rappresenta il file descriptor più grande attualmente gestito dal server.
            max_fd = new_client;
        }
        if (fcntl(new_client, F_SETFL, O_NONBLOCK) == -1) { // imposto il socket del client come non bloccante
            std::cout << "Client accept failed\n" << std::endl; // pure qui ho spostato \n :3
        }
    }
}


void Server::sendClientResponse(int client_id, std::string out_str){ // TO DO

}

