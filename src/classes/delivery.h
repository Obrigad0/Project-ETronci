#ifndef delivery_purchase_h
#define delivery_purchase_h

/* System libraries */

#include <string.h>
#include <stdexcept>

/* Local libraries */

#include "../../lib/con2redis/con2redis.h"
#include "../../utils/src/const.h"
#include "../../utils/src/utils.h"

/* Classes */

class DeliveryPurchase {
    public:
        char *delivery_code = NULL;
        char *time = NULL;
        char *courier = NULL;
        char *purchase = NULL;
        char *state = NULL;

        DeliveryPurchase(char* purchase_id, char* del_code, char* courier_id);
        DeliveryPurchase(char* del_code, char* timestamp, char* update_state, int);

        ~DeliveryPurchase();

        static DeliveryPurchase* from_stream(redisReply* reply, int stream_num, int msg_num);
        static DeliveryPurchase* update_from_stream(redisReply* reply, int stream_num, int msg_num);
        std::string to_insert_query();
        std::string to_update_query();
};

#endif