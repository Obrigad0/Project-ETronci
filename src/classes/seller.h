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
        char *id = NULL;
        char *name = NULL;
        char *piva = NULL;
        char *descrpition = NULL;
        char *mail = NULL;
        char *password = NULL

        // TO DO non so se posso togliere l'id dal costruttore? customer non cel'ha, forse dovrei metterlo lì? uff

        Seller(char* seller_id, char* seller_name, char* seller_piva, char* seller_description, char* seller_mail, char* seller_password);

        ~Seller();

        static Seller* from_stream(redisReply* reply, int stream_num, int msg_num);

};

#endif