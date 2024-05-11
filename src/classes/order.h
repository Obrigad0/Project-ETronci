#ifndef purchase_h
#define purchase_h

#include <string.h>
#include <stdexcept>
#include <chrono>
#include <ctime>

// librerie locali

#include "../service/redis/redisConnection.h"
#include "../shared/standard.h"


class Purchase{
    public:
        char *id = NULL;
        char *purchase_instant = NULL;
        char *fare = NULL;
        char *cancel_instant = NULL;
        char *customer = NULL;
        char *card = NULL;
        char *zip_code = NULL;
        char *street = NULL;
        char *street_number = NULL;

        Purchase(char* purchase_instant_in, char* purchase_fare, char* purchase_customer, char* purchase_card, char* purchase_zip_code, char* purchase_street, char* purchase_street_number);

        Purchase(char* purchase_id, char* purchase_instant_in, char* purchase_fare, char* purchase_cancel_instant, char* purchase_customer, char* purchase_card, char* purchase_zip_code, char* purchase_street, char* purchase_street_number);

        Purchase(char* purchase_id, char* purchase_instant_in, char* purchase_fare, char* purchase_customer, char* purchase_card, char* purchase_zip_code, char* purchase_street, char* purchase_street_number);

        ~Purchase();

        static Purchase* from_stream(redisReply* reply, int stream_num, int msg_num);
};

#endif