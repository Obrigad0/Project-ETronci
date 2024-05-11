#ifndef order_h
#define order_h

#include <string.h>
#include <stdexcept>
#include <chrono>
#include <ctime>

// librerie locali

#include "../service/redis/redisConnection.h"
#include "../shared/standard.h"


class Order{
    public:
        // eliminato street e street_number in favore di address
        // eliminato fare e card, aggiunto product e quantity
        char *id = NULL;
        char *date = NULL;
        char *product = NULL;
        char *quantity = NULL;
        char *customer = NULL;
        char *zip_code = NULL;
        char *address = NULL;

        Order(char* order_date, char* product_id, char* product_quantity, char* order_customer, char* order_zip_code, char* order_address);

        Order(char* order_id, char* order_date, char* product_id, char* product_quantity, char* order_customer, char* order_zip_code, char* order_address);

        ~Order();

        static Order* from_stream(redisReply* reply, int stream_num, int msg_num);
};

#endif