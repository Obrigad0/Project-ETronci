#ifndef ordered_product_h
#define ordered_product_h

#include <string.h>
#include <stdexcept>
#include <chrono>
#include <ctime>

// librerie locali

#include "../service/redis/redisConnection.h"
#include "../shared/standard.h"


class OrderedProduct {

    public:
        char *id = NULL;
        char *date = NULL;
        char *product = NULL;
        char *quantity = NULL;
        char *customer = NULL;
        char *zip_code = NULL;
        char *address = NULL;

        OrderedProduct(char* order_date, char* product_id, char* product_quantity, char* order_customer, char* order_zip_code, char* order_address);

        OrderedProduct(char* order_id, char* order_date, char* product_id, char* product_quantity, char* order_customer, char* order_zip_code, char* order_address);

        ~OrderedProduct();

        static OrderedProduct* from_stream(redisReply* reply, int stream_num, int msg_num);
};


#endif