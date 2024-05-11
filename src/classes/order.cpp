#include "order.h"

Order::Order(char* order_date, char* product_id, char* product_quantity, char* order_customer, char* order_zip_code, char* order_address){

    date = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    product = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    quantity = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    customer = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    zip_code = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    address = (char*) malloc(sizeof(char) * PARAMETERS_LEN);

    strcpy(date, order_date);
    strcpy(product, product_id);
    strcpy(quantity, product_quantity);
    strcpy(customer, order_customer);
    strcpy(zip_code, order_zip_code);
    strcpy(address, order_address);
}

Order::Order(char* order_id, char* order_date, char* product_id, char* product_quantity, char* order_customer, char* order_zip_code, char* order_address){

    id = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    date = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    product = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    quantity = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    customer = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    zip_code = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    address = (char*) malloc(sizeof(char) * PARAMETERS_LEN);

    strcpy(id, order_id);
    strcpy(date, order_date);
    strcpy(product, product_id);
    strcpy(quantity, product_quantity);
    strcpy(customer, order_customer);
    strcpy(zip_code, order_zip_code);
    strcpy(address, order_address);
}

Order::~Order(){
    free(id);
    free(date);
    free(product);
    free(quantity);
    free(customer);
    free(zip_code);
    free(address);
}

Order* Order::from_stream(redisReply* reply, int stream_num, int msg_num){
    char key[KEY_LEN];
    char value[PARAMETERS_LEN];

    char product[PARAMETERS_LEN];
    char quantity[PARAMETERS_LEN];
    char customer[PARAMETERS_LEN];
    char zip_code[PARAMETERS_LEN];
    char address[PARAMETERS_LEN];

    // prendo il timestamp corrente
    char* date;
    auto current_timestamp = std::chrono::system_clock::now();
    std::time_t current_time = std::chrono::system_clock::to_time_t(current_timestamp);
    date = std::ctime(&current_time);

    for (int field_num = 2; field_num < ReadStreamMsgNumVal(reply, stream_num, msg_num); field_num += 2) {
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num, key);
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num + 1, value);
                    
        if (!strcmp(key, "product")) {
            sprintf(product, "%s", value);
        
        }else if (!strcmp(key, "quantity")) {
            sprintf(quantity, "%s", value);

        }else if (!strcmp(key, "customer")) {
            sprintf(customer, "%s", value);

        }else if (!strcmp(key, "zip_code")) {
            sprintf(zip_code, "%s", value);

        }else if (!strcmp(key, "address")) {
            sprintf(address, "%s", value);

        } else {
            throw std::invalid_argument("Stream error: invalid fields");
        }
    }

    return new Order(date, product, quantity, customer, zip_code, address);
}
    