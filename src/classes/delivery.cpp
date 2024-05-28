#include "delivery.h"
#include <iostream>
Delivery::Delivery(char* order_id, char* courier_id, char* order_status) {
    orderid = (char*) malloc(sizeof(char) * PRMTRSIZE);
    courier = (char*) malloc(sizeof(char) * PRMTRSIZE);
    status = (char*) malloc(sizeof(char) * PRMTRSIZE);

    strcpy(orderid, order_id);
    strcpy(courier, courier_id);
    // lo status a questo punto è "not shipped" oppure "shipped"
    strcpy(status, order_status);
}

Delivery::Delivery(char* delivery_id, char* update_status) {
    id = (char*) malloc(sizeof(char) * PRMTRSIZE);
    status = (char*) malloc(sizeof(char) * PRMTRSIZE);

    strcpy(id, delivery_id);
    strcpy(status, update_status);
}

Delivery::Delivery(char* delivery_id, char* order_id, char* courier_id, char* delivery_date, char* order_status) {
    id = (char*) malloc(sizeof(char) * PRMTRSIZE);
    date = (char*) malloc(sizeof(char) * PRMTRSIZE);
    orderid = (char*) malloc(sizeof(char) * PRMTRSIZE);
    courier = (char*) malloc(sizeof(char) * PRMTRSIZE);
    status = (char*) malloc(sizeof(char) * PRMTRSIZE);

    strcpy(date, delivery_date);
    strcpy(orderid, order_id);
    strcpy(courier, courier_id);
    strcpy(id, delivery_id);
    strcpy(status, order_status);
}



Delivery::~Delivery(){
    free(id);
    free(orderid);
    free(courier);
    free(status);
}


Delivery* Delivery::from_stream(redisReply* reply, int stream_num, int msg_num) {
    char key[KEYSIZE];
    char value[PRMTRSIZE];

    char courier[PRMTRSIZE];
    char orderid[PRMTRSIZE];
    char status[PRMTRSIZE];

    // itero attraverso i campi del messaggio nel flusso Redis. Ogni campo è rappresentato da una coppia chiave-valore.
    for (int field = 2; field < ReadStreamMsgNumVal(reply, stream_num, msg_num); field += 2) {
        ReadStreamMsgVal(reply, stream_num, msg_num, field, key);
        ReadStreamMsgVal(reply, stream_num, msg_num, field + 1, value);
                    
        if (!strcmp(key, "status")) {
            sprintf(status, "%s", value); //cambiato id con status

        } else if (!strcmp(key, "orderid")) {
            sprintf(orderid, "%s", value);

        }else if (!strcmp(key, "courier")) {
            sprintf(courier, "%s", value);

        } else {
            throw std::invalid_argument("Stream error: invalid fields");
        }
    }

    return new Delivery(orderid, courier, status);
}

// aggiorna lo stato della spedizione
Delivery* Delivery::update_from_stream(redisReply* reply, int stream_num, int msg_num) {
    char key[KEYSIZE];
    char value[PRMTRSIZE];

    char id[PRMTRSIZE];
    char update_status[PRMTRSIZE];

    for (int field = 2; field < ReadStreamMsgNumVal(reply, stream_num, msg_num); field += 2) {
        ReadStreamMsgVal(reply, stream_num, msg_num, field, key);
        ReadStreamMsgVal(reply, stream_num, msg_num, field + 1, value);
                    
        if (!strcmp(key, "status")) {
            sprintf(update_status, value);

        } else if (!strcmp(key, "id")) {
            sprintf(id, "%s", value);

        } else {
            throw std::invalid_argument("Stream error: invalid fields");
        }
    }

    return new Delivery(id, update_status);
}

////// query del database

std::string Delivery::to_insert_query() {
    //std::string str_id = id;
    std::string str_courier = courier;
    std::string str_order = orderid;

    //auto current_timestamp = std::chrono::system_clock::now();
    //std::time_t current_time = std::chrono::system_clock::to_time_t(current_timestamp);
    //std::string current_date = std::ctime(&current_time); // solo ctime(&current_time);???? TO DO

    std::string current_date = get_current_timestamp_as_string();

    return "INSERT INTO Delivery (orderId, courier, date) VALUES (\'" + str_order + "\', \'" + str_courier + "\', \'" + current_date + "\')";
}


std::string Delivery::to_update_query() {
    std::string str_id = id;
    std::string str_status = status;

    return "UPDATE Delivery SET status = \'" + str_status + "\' WHERE id = \'" + str_id + "\'";
}