#ifndef delivery_h
#define delivery_h

#include <string.h>
#include <stdexcept>

// librerie locali

#include "../service/redis/redisConnection.h"
#include "../shared/standard.h"

class Delivery {
    public:
        // eliminata la variabile time
        char *id = NULL; // cambiato nome delivery_code a id
        char *courier = NULL;
        char *order = NULL;
        std::string *status = NULL; // cambiato nome da state a status

        Delivery(char* order_id, char* delivery_code, char* courier_id);
        Delivery(char* delivery_code, std::string update_status); // costruttore per aggiornare lo stato dell'ordine

        ~Delivery();

        static Delivery* from_stream(redisReply* reply, int stream_num, int msg_num);
        static Delivery* update_from_stream(redisReply* reply, int stream_num, int msg_num);

        std::string to_insert_query();
        std::string to_update_query();
};

#endif