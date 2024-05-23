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

        Customer(char* customer_name, char* customer_mail);

        ~Customer();

        static Customer* from_stream(redisReply* reply, int stream_num, int msg_num);

};

#endif