#include "address.h"

Address::Address(char* address_street, char* address_steet_number, char* address_zip_code, char* address_city){

    street = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    street_number = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    zip_code = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    city = (char*) malloc(sizeof(char) * PARAMETERS_LEN);

    strcpy(street, address_street);
    strcpy(street_number, address_steet_number);
    strcpy(zip_code, address_zip_code);
    strcpy(city, address_city);
}

Address::~Address(){
    free(street);
    free(street_number);
    free(zip_code);
    free(city);
}

Address* Address::from_stream(redisReply* reply, int stream_num, int msg_num){

    char key[KEY_LEN];
    char value[PARAMETERS_LEN];

    char street[PARAMETERS_LEN];
    char street_number[PARAMETERS_LEN];
    char zip_code[PARAMETERS_LEN];
    char city[PARAMETERS_LEN];

    for (int field_num = 2; field_num < ReadStreamMsgNumVal(reply, stream_num, msg_num); field_num +=  2) {
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num, key);
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num + 1, value);
            
        if (!strcmp(key, "street")) {
            sprintf(street, "%s", value);

        }else if (!strcmp(key, "zip_code")) {
            sprintf(zip_code, "%s", value);

        } else if (!strcmp(key, "street_number")) {
            sprintf(street_number, "%s", value);

        } else if (!strcmp(key, "city")) {
            sprintf(city, "%s", value);

        } else {
            throw std::invalid_argument("Stream error: invalid fields");
        }
    }

    return new Address(street, street_number, zip_code, city);
}
    