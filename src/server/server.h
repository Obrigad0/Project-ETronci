// #ifndef server_h
#define server_h

#include <cstring> 
#include <iostream> 
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h> 
#include <cerrno>
#include <vector>
#include <sstream>
#include <signal.h>

#include "handler.h"
#include "../service/database/dbConnection.h"

#define MAX_CLIENT 100
#define TRUE 1 
#define FALSE 0 


#define POSTGRESQL_SERVER "localhost"
#define POSTGRESQL_PORT "5432"
#define POSTGRESQL_USER "handler"
#define POSTGRESQL_PSW "handler"
#define POSTGRESQL_DBNAME "logdb"

class Server {
    public:
        //Costruttore della classe Server
        Server(const char* id, int port, const char* redis_ip, int redis_port, std::string client_requests[], int req_num);

        // Valori dati in input al costruttore
            // const char* id: id del server creato (stringa!!! Es: serverCustomer)
            // int port: porta di ascolto del server
            // const char* redis_ip: ip server redis, che in questo caso e' locale quindi sara' "localhost"
            // int redis_port: porta di ascolto del server Redis inserito
            // client_requests[]: sono l'insieme delle richieste effettuabili dal client, es come in wasa lista dei nomi delle funzioni
            // int req_num: e' il numero di queste funzioni

        // run NON viene chiamato dal costruttore. Dopo aver istanziato un oggetto di tipo Server, va fatto server.run()
        // run avvia il server
        // E' l'unica funzione chiamabile da file che creano un proprio server (Customer, Courier e Seller)
        void run();
        
        //Distruttore della classe Server
        ~Server();

    private:

        void addNewClients(); // aggiunta di un nuovo client alla connessione  (ex add_new_clients() TO DO LEVARE DAL COMMENTO)
        void receiveClientData(int i);  // gestione ricezione dati dal client (i file descriptor) (ex receive TO DO LEVARE DAL COMMENTO)
        void sendClientResponse(int client_id, std::string out_str); //gestisce invio dati al client (ex send_response TO DO LEVARE DAL COMMENTO)
        void chiudiConnessione();

        DbConnection db = DbConnection("localhost", "5432", "handler", "handler", "logdb");
        PGresult *resp;
        const char* server; 
        int socket_server;
        int socket_port;
        fd_set current_set;
        int max_fd;
        Handler* handler;



};
