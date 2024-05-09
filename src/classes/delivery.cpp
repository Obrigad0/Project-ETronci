#include "delivery.h"

Delivery::Delivery(char* purchase_id, char* del_code, char* courier_id) {

    delivery_code = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    courier = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    purchase = (char*) malloc(sizeof(char) * PARAMETERS_LEN);

    strcpy(delivery_code, del_code);
    strcpy(courier, courier_id);
    strcpy(purchase, purchase_id);
}

Delivery::Delivery(char* del_code, char* timestamp, char* update_state, int useless) {

    delivery_code = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    time = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    state = (char*) malloc(sizeof(char) * PARAMETERS_LEN);

    strcpy(delivery_code, del_code);
    strcpy(state, update_state);
    strcpy(time, timestamp);
}


Delivery::~Delivery(){
    free(delivery_code);
    free(purchase);
    free(courier);
    free(time);
    free(state);
}

Delivery* Delivery::from_stream(redisReply* reply, int stream_num, int msg_num) {
    char key[PARAMETERS_LEN];
    char value[PARAMETERS_LEN];

    char code_dl[PARAMETERS_LEN];
    char courier_id[PARAMETERS_LEN];
    char purchase_id[PARAMETERS_LEN];

    char read_fields = 0b000;

    for (int field_num = 2; field_num < ReadStreamMsgNumVal(reply, stream_num, msg_num); field_num += 2) {
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num, key);
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num + 1, value);
                    
        if (!strcmp(key, "courier")) {
            sprintf(courier_id, "%s", value);
            read_fields |=0b001;

        } else if (!strcmp(key, "purchase")) {
            sprintf(purchase_id, "%s", value);
            read_fields |=0b010;

        } else if (!strcmp(key, "deliverycode")) {
            sprintf(code_dl, "%s", value);
            read_fields |=0b100;

        } else {
            throw std::invalid_argument("Stream error: invalid fields");
        }
    }

    if (read_fields != 0b111) {
        throw std::invalid_argument("Stream error: invalid fields");
    }

    return new DeliveryPurchase(purchase_id, code_dl, courier_id);
}


Delivery* Delivery::update_from_stream(redisReply* reply, int stream_num, int msg_num) {
    char key[PARAMETERS_LEN];
    char value[PARAMETERS_LEN];

    char code_dl[PARAMETERS_LEN];
    char timestamp[PARAMETERS_LEN];
    char update_state[PARAMETERS_LEN];

    char read_fields = 0b0000;

    for (int field_num = 2; field_num < ReadStreamMsgNumVal(reply, stream_num, msg_num); field_num += 2) {
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num, key);
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num + 1, value);
                    
        if (!strcmp(key, "start")) {
            sprintf(update_state, "OnGoing");
            sprintf(timestamp, "%s", value);
            read_fields |= 0b0001;

        } else if (!strcmp(key, "lost")) {
            sprintf(update_state, "Lost");
            sprintf(timestamp, "%s", value);
            read_fields |= 0b0010;

        } else if (!strcmp(key, "end")) {
            sprintf(update_state, "Delivered");
            sprintf(timestamp, "%s", value);
            read_fields |= 0b0100;

        } else if (!strcmp(key, "deliverycode")) {
            sprintf(code_dl, "%s", value);
            read_fields |= 0b1000;

        } else {
            throw std::invalid_argument("Stream error: invalid fields");
        }
    }

    if (read_fields != 0b1100 && read_fields != 0b1010 && read_fields != 0b1010 && read_fields != 0b1001) {
        throw std::invalid_argument("Stream error: invalid fields");
    }

    return new DeliveryPurchase(code_dl, timestamp, update_state, 0);
}

/////////////////////////////////////////////////////
// TO DO: cambiare roba del database

std::string Delivery::to_insert_query() {
    std::string str_code = delivery_code;
    std::string str_purchase = purchase;
    std::string str_courier = courier;
    std::string str_time = get_current_timestamp_as_string();

    return "INSERT INTO AssignedDelivery (delivery_code, delivery_assignment_instant, delivery_state, courier, purchase) VALUES (\'" + str_code + "\', \'" + str_time + "\', \'Assigned\', \'" + str_courier + "\', \'" + str_purchase + "\')";
}


std::string Delivery::to_update_query() {
    std::string str_code = delivery_code;
    std::string str_state = state;
    std::string str_time = replace_substring(time, SPACE_REDIS_STRING, SPACE);
    std::string str_column = (str_state == "Delivered" ? "delivery_end" : (str_state == "Lost" ? "lost_instant" : "delivery_start"));

    return "UPDATE AssignedDelivery SET delivery_state = \'" + str_state + "\', " + str_column + " = \'" + str_time + "\' WHERE delivery_code = \'" + str_code + "\'";
}