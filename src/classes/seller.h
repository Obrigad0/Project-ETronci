#ifndef seller_h
#define seller_h

#include <string.h>
#include <stdexcept>

// librerie locali

#include "../service/redis/redisConnection.h"
#include "../shared/standard.h"

// class

class Seller{

    public:
        char *name = NULL;
        char *piva = NULL;
        char *descrpition = NULL;
        char *mail = NULL;

        Seller(char* seller_name, char* seller_piva, char* seller_description, char* seller_mail);

        ~Seller();

        static Seller* from_stream(redisReply* reply, int stream_num, int msg_num);

};

#endif