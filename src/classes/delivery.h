#ifndef delivery_h
#define delivery_h

#include <string.h>
#include <stdexcept>

// librerie locali

#include "../service/redis/redisConnection.h"
#include "../shared/standard.h"

class Delivery {

    public:
        char *id = NULL;
        char *orderid = NULL;
        char *courier = NULL;
        char *date = NULL;
        char *status = NULL;

        Delivery(char* order_id, char* courier_id, char* order_status);
        Delivery(char* delivery_id, char* update_status); // costruttore per aggiornare lo stato dell'ordine
        Delivery(char* delivery_id, char* order_id, char* courier_id, char* delivery_date, char* order_status);

        ~Delivery();

        static Delivery* from_stream(redisReply* reply, int stream_num, int msg_num);
        static Delivery* update_from_stream(redisReply* reply, int stream_num, int msg_num);

        std::string to_insert_query();
        std::string to_update_query();
        
};

#endif