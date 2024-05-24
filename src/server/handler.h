#ifndef handler_h
#define handler_h

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/times.h>
#include <cassert>
#include <cerrno>
#include <iostream>


#include "../service/redis/redisConnection.h"
#include "../service/database/dbConnection.h"
#include "../shared/standard.h"

/* Classes */
class Handler{
    public:
        //Costruttore Handler
        Handler(const char* redip, int redport, std::string client_requests[], int req_num);
        // Valori dati in input al costruttore
            //redip: ip server redis
            //redport: porta server redis
            // client_requests[]: sono l'insieme delle richieste effettuabili dal client
            // int req_num: e' il numero di queste funzioni

        bool sendToFunctions(int idClient, std::string msg);
        bool readFromFunctions(std::string* outstr, int* idClientpnt);

    private:
        redisContext *c2r;
        std::string* types;
        int num_types;
        
        void startStream();
};

#endif