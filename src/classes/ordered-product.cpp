#include "ordered-product.h"

OrderedProduct::OrderedProduct(char* order_date, char* product_id, char* product_quantity, char* order_customer, char* order_zip_code, char* order_address){

    date = (char*) malloc(sizeof(char) * PRMTRSIZE);
    product = (char*) malloc(sizeof(char) * PRMTRSIZE);
    quantity = (char*) malloc(sizeof(char) * PRMTRSIZE);
    customer = (char*) malloc(sizeof(char) * PRMTRSIZE);
    zip_code = (char*) malloc(sizeof(char) * PRMTRSIZE);
    address = (char*) malloc(sizeof(char) * PRMTRSIZE);

    strcpy(date, order_date);
    strcpy(product, product_id);
    strcpy(quantity, product_quantity);
    strcpy(customer, order_customer);
    strcpy(zip_code, order_zip_code);
    strcpy(address, order_address);
}

OrderedProduct::OrderedProduct(char* order_id, char* order_date, char* product_id, char* product_quantity, char* order_customer, char* order_zip_code, char* order_address){

    id = (char*) malloc(sizeof(char) * PRMTRSIZE);
    date = (char*) malloc(sizeof(char) * PRMTRSIZE);
    product = (char*) malloc(sizeof(char) * PRMTRSIZE);
    quantity = (char*) malloc(sizeof(char) * PRMTRSIZE);
    customer = (char*) malloc(sizeof(char) * PRMTRSIZE);
    zip_code = (char*) malloc(sizeof(char) * PRMTRSIZE);
    address = (char*) malloc(sizeof(char) * PRMTRSIZE);

    strcpy(id, order_id);
    strcpy(date, order_date);
    strcpy(product, product_id);
    strcpy(quantity, product_quantity);
    strcpy(customer, order_customer);
    strcpy(zip_code, order_zip_code);
    strcpy(address, order_address);
}

OrderedProduct::~OrderedProduct(){
    free(id);
    free(date);
    free(product);
    free(quantity);
    free(customer);
    free(zip_code);
    free(address);
}

OrderedProduct* OrderedProduct::from_stream(redisReply* reply, int stream_num, int msg_num){
    char key[KEYSIZE];
    char value[PRMTRSIZE];

    char product[PRMTRSIZE];
    char quantity[PRMTRSIZE];
    char customer[PRMTRSIZE];
    char zip_code[PRMTRSIZE];
    char address[PRMTRSIZE];

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

    return new OrderedProduct(date, product, quantity, customer, zip_code, address);
}
    