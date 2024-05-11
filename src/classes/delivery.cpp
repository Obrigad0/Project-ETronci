#include "delivery.h"

Delivery::Delivery(char* order_id, char* delivery_code, char* courier_id) {
    id = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    order = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    courier = (char*) malloc(sizeof(char) * PARAMETERS_LEN);

    strcpy(id, delivery_code);
    strcpy(order, order_id);
    strcpy(courier, courier_id);
    status = "not shipped"
}

Delivery::Delivery(char* delivery_code, std::string update_status) {
    id = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    status = (char*) malloc(sizeof(char) * PARAMETERS_LEN);

    strcpy(id, delivery_code);
    strcpy(status, update_status);
}


Delivery::~Delivery(){
    free(id);
    free(order);
    free(courier);
    free(status);
}

// sostituito PARAMETERS_LEN con KEY_LEN per key

Delivery* Delivery::from_stream(redisReply* reply, int stream_num, int msg_num) {
    char key[KEY_LEN];
    char value[PARAMETERS_LEN];

    char delivery_code[PARAMETERS_LEN];
    char courier_id[PARAMETERS_LEN];
    char order_id[PARAMETERS_LEN];

    // itero attraverso i campi del messaggio nel flusso Redis. Ogni campo è rappresentato da una coppia chiave-valore.
    for (int field = 2; field < ReadStreamMsgNumVal(reply, stream_num, msg_num); field += 2) {
        ReadStreamMsgVal(reply, stream_num, msg_num, field, key);
        ReadStreamMsgVal(reply, stream_num, msg_num, field + 1, value);
                    
        if (!strcmp(key, "courier")) {
            sprintf(courier_id, "%s", value);

        } else if (!strcmp(key, "order")) {
            sprintf(order_id, "%s", value);

        } else if (!strcmp(key, "deliverycode")) {
            sprintf(delivery_code, "%s", value);

        } else {
            throw std::invalid_argument("Stream error: invalid fields");
        }
    }

    return new Delivery(order_id, delivery_code, courier_id);
}

// aggiorna lo stato della spedizione
Delivery* Delivery::update_from_stream(redisReply* reply, int stream_num, int msg_num) {
    char key[KEY_LEN];
    char value[PARAMETERS_LEN];

    char delivery_code[PARAMETERS_LEN];
    char timestamp[PARAMETERS_LEN];
    char update_status[PARAMETERS_LEN];

    for (int field = 2; field < ReadStreamMsgNumVal(reply, stream_num, msg_num); field += 2) {
        ReadStreamMsgVal(reply, stream_num, msg_num, field, key);
        ReadStreamMsgVal(reply, stream_num, msg_num, field + 1, value);
                    
        if (!strcmp(key, "start")) {
            sprintf(update_status, "shipped");

        } else if (!strcmp(key, "end")) {
            sprintf(update_status, "delivered");

        } else if (!strcmp(key, "deliverycode")) {
            sprintf(delivery_code, "%s", value);

        } else {
            throw std::invalid_argument("Stream error: invalid fields");
        }
    }

    return new Delivery(delivery_code, update_status);
}

////// query del database

std::string Delivery::to_insert_query() {
    std::string str_id = str(id);
    std::string str_courier = str(courier);
    std::string str_order = str(order);
    std::string current_date = get_current_timestamp_as_string();

    return "INSERT INTO delivery (id, order, courier, date) VALUES (\'" + str_id + "\', \'" + str_order + "\', \'" + str_courier + "\', \'" + current_date + "\')";
}


std::string Delivery::to_update_query() {
    std::string str_id = str(id);
    std::string str_status = str(status);

    return "UPDATE delivery SET statusOrder = \'" + str_status + "\', WHERE id = \'" + str_id + "\'";
}