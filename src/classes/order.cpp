#include "purchase.h"

Purchase::Purchase(
        char* purchase_instant_in,
        char* purchase_fare,
        char* purchase_customer, 
        char* purchase_card,
        char* purchase_zip_code, 
        char* purchase_street, 
        char* purchase_street_number){

    purchase_instant = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    fare = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    customer = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    card = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    zip_code = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    street = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    street_number = (char*) malloc(sizeof(char) * PARAMETERS_LEN);

    strcpy(purchase_instant, purchase_instant_in);
    strcpy(fare, purchase_fare);
    strcpy(customer, purchase_customer);
    strcpy(card, purchase_card);
    strcpy(zip_code, purchase_zip_code);
    strcpy(street, purchase_street);
    strcpy(street_number, purchase_street_number);
}

Purchase::Purchase(
        char* purchase_id,
        char* purchase_instant_in,
        char* purchase_fare,
        char* purchase_customer, 
        char* purchase_card,
        char* purchase_zip_code, 
        char* purchase_street, 
        char* purchase_street_number){

    id = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    purchase_instant = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    fare = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    customer = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    card = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    zip_code = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    street = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    street_number = (char*) malloc(sizeof(char) * PARAMETERS_LEN);

    strcpy(id, purchase_id);
    strcpy(purchase_instant, purchase_instant_in);
    strcpy(fare, purchase_fare);
    strcpy(customer, purchase_customer);
    strcpy(card, purchase_card);
    strcpy(zip_code, purchase_zip_code);
    strcpy(street, purchase_street);
    strcpy(street_number, purchase_street_number);
}

Purchase::Purchase(char* purchase_id, char* purchase_instant_in, char* purchase_fare, char* purchase_cancel_instant, char* purchase_customer, char* purchase_card, char* purchase_zip_code, char* purchase_street, char* purchase_street_number) {

    id = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    purchase_instant = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    fare = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    cancel_instant = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    customer = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    card = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    zip_code = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    street = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    street_number = (char*) malloc(sizeof(char) * PARAMETERS_LEN);

    strcpy(id, purchase_id);
    strcpy(purchase_instant, purchase_instant_in);
    strcpy(fare, purchase_fare);
    strcpy(cancel_instant, purchase_cancel_instant);
    strcpy(customer, purchase_customer);
    strcpy(card, purchase_card);
    strcpy(zip_code, purchase_zip_code);
    strcpy(street, purchase_street);
    strcpy(street_number, purchase_street_number);
}

Purchase::~Purchase(){
    free(id);
    free(purchase_instant);
    free(fare);
    free(cancel_instant);
    free(customer);
    free(card);
    free(zip_code);
    free(street);
    free(street_number);
}

Purchase* Purchase::from_stream(redisReply* reply, int stream_num, int msg_num){
    char key[PARAMETERS_LEN];
    char value[PARAMETERS_LEN];

    char* purchase_instant;
    char fare[PARAMETERS_LEN];
    char customer[PARAMETERS_LEN];
    char card[PARAMETERS_LEN];
    char zip_code[PARAMETERS_LEN];
    char street[PARAMETERS_LEN];
    char street_number[PARAMETERS_LEN];

    char read_fields = 0b000000;

    // prendo il timestamp corrente
    auto current_time = std::chrono::system_clock::now();
    std::time_t current_time_t = std::chrono::system_clock::to_time_t(current_time);
    purchase_instant = std::ctime(&current_time_t);

    for (int field_num = 2; field_num < ReadStreamMsgNumVal(reply, stream_num, msg_num) && read_fields != 0b111111; field_num += 2) {
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num, key);
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num + 1, value);
                    
        if (!strcmp(key, "fare")) {
            sprintf(fare, "%s", value);
            read_fields |=0b000001;

        }else if (!strcmp(key, "customer")) {
            sprintf(customer, "%s", value);
            read_fields |=0b000010;

        }else if (!strcmp(key, "card")) {
            sprintf(card, "%s", value);
            read_fields |=0b000100;

        }else if (!strcmp(key, "zip_code")) {
            sprintf(zip_code, "%s", value);
            read_fields |=0b001000;

        }else if (!strcmp(key, "street")) {
            sprintf(street, "%s", value);
            read_fields |=0b010000;

        }else if (!strcmp(key, "street_number")) {
            sprintf(street_number, "%s", value);
            read_fields |=0b100000;

        } else {
            throw std::invalid_argument("Stream error: invalid fields");
        }
    }

    if (read_fields != 0b111111){
        throw std::invalid_argument("Stream error: invalid fields");
    }

    return new Purchase(purchase_instant, fare, customer, card, zip_code, street, street_number);
}
    