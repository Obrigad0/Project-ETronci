#ifndef courier_h
#define courier_h

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
        char* warehouse = NULL;

        Courier(char* courier_name, char* courier_email, char* courier_warehouse);

        ~Courier();

        static Courier* from_stream(redisReply* reply, int stream_num, int msg_num);

};

#endif