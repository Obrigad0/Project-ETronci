#ifndef customer_h
#define customer_h

#include <string.h>
#include <stdexcept>

// librerie locali

#include "../service/redis/redisConnection.h"
#include "../shared/standard.h"

// class

class Courier{

    public:
        char* name = NULL;
        char* mail = NULL;
        char* password = NULL;
        char* warehouse = NULL;

        Courier(char* customer_name, char* customer_email, char* customer_password, char* courier_warehouse);

        ~Courier();

        static Courier* from_stream(redisReply* reply, int stream_num, int msg_num);

};

#endif