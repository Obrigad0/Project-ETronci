#ifndef address_h
#define address_h

#include <string.h>
#include <stdexcept>

// librerie locali

#include "../service/redis/redisConnection.h"
#include "../shared/standard.h"

// class

class Address{

    public:
        char *street = NULL;
        char *street_number = NULL;
        char *zip_code = NULL;
        char *city = NULL;

        Address(char* address_street, char* address_steet_number, char* address_zip_code, char* address_city);

        ~Address();

        static Address* from_stream(redisReply* reply, int stream_num, int msg_num);

};

#endif