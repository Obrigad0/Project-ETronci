#include "product.h"

Product::Product(char* product_id, char* product_name, char* product_description, char* product_price, char* product_seller, char* product_warehouse){

    id = (char*) malloc(sizeof(char) * PRMTRSIZE);
    name = (char*) malloc(sizeof(char) * PRMTRSIZE);
    description = (char*) malloc(sizeof(char) * PRMTRSIZE);
    price_tag = (char*) malloc(sizeof(char) * PRMTRSIZE);
    seller = (char*) malloc(sizeof(char) * PRMTRSIZE);
    warehouse = (char*) malloc(sizeof(char) * PRMTRSIZE);

    strcpy(id, product_id);
    strcpy(name, product_name);
    strcpy(description, product_description);
    strcpy(price_tag, product_price);
    strcpy(seller, product_seller);
    strcpy(warehouse, product_warehouse);
}

Product::Product(char* product_id, char* product_description){

    id = (char*) malloc(sizeof(char) * PRMTRSIZE);
    name = (char*) malloc(sizeof(char) * PRMTRSIZE);
    description = (char*) malloc(sizeof(char) * PRMTRSIZE);
    price_tag = (char*) malloc(sizeof(char) * PRMTRSIZE);
    seller = (char*) malloc(sizeof(char) * PRMTRSIZE);
    warehouse = (char*) malloc(sizeof(char) * PRMTRSIZE);

    strcpy(id, product_id);
    strcpy(description, product_description);
}


Product::~Product(){
    free(id);
    free(name);
    free(description);
    free(price_tag);
    free(seller);
    free(warehouse);
}

Product* Product::from_stream(redisReply* reply, int stream_num, int msg_num) {

    char key[KEYSIZE];
    char value[PRMTRSIZE];

    char id[PRMTRSIZE];
    char name[PRMTRSIZE];
    char description[PRMTRSIZE];
    char price_tag[PRMTRSIZE];
    char seller[PRMTRSIZE];
    char warehouse[PRMTRSIZE];

    for (int field_num = 2; field_num < ReadStreamMsgNumVal(reply, stream_num, msg_num); field_num += 2) {
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num, key);
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num + 1, value);
                    
        if (!strcmp(key, "id")) {
            sprintf(id, "%s", value);

        } else if (!strcmp(key, "name")) {
            sprintf(name, "%s", value);

        } else if (!strcmp(key, "description")) {
            sprintf(description, "%s", value);

        } else if (!strcmp(key, "price_tag")) {
            sprintf(price_tag, "%s", value);
        
        } else if (!strcmp(key, "seller")) {
            sprintf(seller, "%s", value);
        
        } else if (!strcmp(key, "warehouse")) {
            sprintf(warehouse, "%s", value);

        } else {
            throw std::invalid_argument("Stream error: invalid fields");
        }
    }

    return new Product(id, name, description, price_tag, seller, warehouse);
}


Product* Product::update_from_stream(redisReply* reply, int stream_num, int msg_num) {

    char key[KEYSIZE];
    char value[PRMTRSIZE];

    char id[PRMTRSIZE];
    char description[PRMTRSIZE];

    for (int field_num = 2; field_num < ReadStreamMsgNumVal(reply, stream_num, msg_num); field_num += 2) {
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num, key);
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num + 1, value);
                    
        if (!strcmp(key, "id")) {
            sprintf(id, "%s", value);

        } else if (!strcmp(key, "description")) {
            sprintf(description, "%s", value);

        }else {
            throw std::invalid_argument("Stream error: invalid fields");
        }
    }

    std::string str_description = description;

    str_description = replace_substring(str_description, REDISSPACE, BLANKSPACE);

    return new Product(id, (char*)str_description.c_str());
}

std::string Product::to_insert_query() {
    std::string str_id = id;
    std::string str_name = name;
    std::string str_description = description;
    std::string str_price = price_tag;
    std::string str_seller = seller;
    std::string str_warehouse = warehouse;

    str_name = replace_substring(str_name, REDISSPACE, BLANKSPACE);
    str_description = replace_substring(str_description, REDISSPACE, BLANKSPACE);

    return "INSERT INTO Product (id, name, description, price_tag, seller, warehouse) VALUES (\'" + str_id + "\', \'" + str_name + "\', \'" + str_description + "\', \'" + str_price + "\', \'" + str_seller + "\', \'" + str_warehouse + "\')";
}