#ifndef review_h
#define review_h

#include <string.h>
#include <stdexcept>

// librerie locali

#include "../service/redis/redisConnection.h"
#include "../shared/standard.h"

// class

class Review {

    public:
        char *customer = NULL;
        char *product = NULL;
        char *stars = NULL;
        char *comment = NULL;

        Review(char* review_customer, char* review_product, char* review_stars, char* review_comment);

        ~Review();

        static Review* from_stream(redisReply* reply, int stream_num, int msg_num);
        
};

#endif