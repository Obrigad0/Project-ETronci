#include "delivery.h"

Delivery::Delivery(char* delivery_id, char* order_id, char* courier_id) {
    id = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    orderid = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    courier = (char*) malloc(sizeof(char) * PARAMETERS_LEN);

    strcpy(id, delivery_id);
    strcpy(orderid, order_id);
    strcpy(courier, courier_id);
    status = "not shipped";
}

Delivery::Delivery(char* delivery_id, char* update_status) {
    id = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    status = (char*) malloc(sizeof(char) * PARAMETERS_LEN);

    strcpy(id, delivery_id);
    strcpy(status, update_status);
}


Delivery::~Delivery(){
    free(id);
    free(orderid);
    free(courier);
    free(status);
}

// sostituito PARAMETERS_LEN con KEY_LEN per key

Delivery* Delivery::from_stream(redisReply* reply, int stream_num, int msg_num) {
    char key[KEY_LEN];
    char value[PARAMETERS_LEN];

    char id[PARAMETERS_LEN];
    char courier[PARAMETERS_LEN];
    char orderid[PARAMETERS_LEN];

    // itero attraverso i campi del messaggio nel flusso Redis. Ogni campo è rappresentato da una coppia chiave-valore.
    for (int field = 2; field < ReadStreamMsgNumVal(reply, stream_num, msg_num); field += 2) {
        ReadStreamMsgVal(reply, stream_num, msg_num, field, key);
        ReadStreamMsgVal(reply, stream_num, msg_num, field + 1, value);
                    
        if (!strcmp(key, "deliveryid")) {
            sprintf(id, "%s", value);

        } else if (!strcmp(key, "orderid")) {
            sprintf(orderid, "%s", value);

        }else if (!strcmp(key, "courier")) {
            sprintf(courier, "%s", value);

        } else {
            throw std::invalid_argument("Stream error: invalid fields");
        }
    }

    return new Delivery(id, orderid, courier);
}

// aggiorna lo stato della spedizione
Delivery* Delivery::update_from_stream(redisReply* reply, int stream_num, int msg_num) {
    char key[KEY_LEN];
    char value[PARAMETERS_LEN];

    char id[PARAMETERS_LEN];
    char update_status[PARAMETERS_LEN];

    for (int field = 2; field < ReadStreamMsgNumVal(reply, stream_num, msg_num); field += 2) {
        ReadStreamMsgVal(reply, stream_num, msg_num, field, key);
        ReadStreamMsgVal(reply, stream_num, msg_num, field + 1, value);
                    
        if (!strcmp(key, "start")) {
            sprintf(update_status, "shipped");

        } else if (!strcmp(key, "end")) {
            sprintf(update_status, "delivered");

        } else if (!strcmp(key, "deliveryid")) {
            sprintf(id, "%s", value);

        } else {
            throw std::invalid_argument("Stream error: invalid fields");
        }
    }

    return new Delivery(id, update_status);
}

////// query del database

std::string Delivery::to_insert_query() {
    std::string str_id = str(id);
    std::string str_courier = str(courier);
    std::string str_order = str(orderid);

    auto current_timestamp = std::chrono::system_clock::now();
    std::time_t current_time = std::chrono::system_clock::to_time_t(current_timestamp);
    std::string current_date = std::ctime(&current_time);

    //std::string current_date = get_current_timestamp_as_string();

    return "INSERT INTO Delivery (id, orderid, courier, date) VALUES (\'" + str_id + "\', \'" + str_order + "\', \'" + str_courier + "\', \'" + current_date + "\')";
}


std::string Delivery::to_update_query() {
    std::string str_id = str(id);
    std::string str_status = str(status);

    return "UPDATE Delivery SET statusOrder = \'" + str_status + "\', WHERE id = \'" + str_id + "\'";
}