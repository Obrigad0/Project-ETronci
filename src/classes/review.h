#ifndef card_h
#define card_h

#include <string.h>
#include <stdexcept>

// librerie locali

#include "../service/redis/redisConnection.h"
#include "../shared/standard.h"

// class

class Rating {
    public:
        char *product = NULL;
        char *customer = NULL;
        char *stars = NULL;
        std::string *comment = NULL;

        Rating(char* rating_product, char* rating_customer, char* rating_stars, std::string comment);

        ~Rating();

        static Rating* from_stream(redisReply* reply, int stream_num, int msg_num);
};

#endif