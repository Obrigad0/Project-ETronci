#ifndef product_h
#define product_h

#include <string.h>
#include <stdexcept>

// librerie locali

#include "../service/redis/redisConnection.h"
#include "../shared/standard.h"

// class

class Product {

    public:
        char *id = NULL;
        char *name = NULL;
        char *description = NULL;
        char *price_tag = NULL;

        Product(char* product_id, char* product_name, char* product_description, char* product_price);
        Product(char* product_id, char* product_description);

        ~Product();

        static Product* from_stream(redisReply* reply, int stream_num, int msg_num);
        static Product* update_from_stream(redisReply* reply, int stream_num, int msg_num);
        
        std::string to_insert_query();

};

#endif