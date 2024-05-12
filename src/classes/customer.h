#ifndef customer_h
#define customer_h

#include <string.h>
#include <stdexcept>

// librerie locali

#include "../service/redis/redisConnection.h"
#include "../shared/standard.h"

// class

class Customer{

    public:
        char* name = NULL;
        char* mail = NULL;
        char* password = NULL;
        char* address = NULL;

        Customer(char* customer_name, char* customer_email, char* customer_password, char* customer_address);

        ~Customer();

        static Customer* from_stream(redisReply* reply, int stream_num, int msg_num);

};

#endif