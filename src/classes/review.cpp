#include "review.h"

Review::Review(char* review_customer, char* review_product, char* review_stars, char* review_comment){

    customer = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    product = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    stars = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    comment = (char*) malloc(sizeof(char) * PARAMETERS_LEN);

    strcpy(customer, review_customer);
    strcpy(product, review_product);
    strcpy(stars, review_stars);
    strcpy(comment, review_comment);
}

Review::~Review(){
    free(customer);
    free(product);
    free(stars);
    free(comment);
}

Review* Review::from_stream(redisReply* reply, int stream_num, int msg_num){

    char key[KEY_LEN];
    char value[PARAMETERS_LEN];

    char customer[PARAMETERS_LEN];
    char product[PARAMETERS_LEN];
    char stars[PARAMETERS_LEN];
    char comment[PARAMETERS_LEN];

    for (int field_num = 2; field_num < ReadStreamMsgNumVal(reply, stream_num, msg_num); field_num +=  2) {
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num, key);
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num + 1, value);
                    
        if (!strcmp(key, "customer")) {
            sprintf(customer, "%s", value);
        
        }else if (!strcmp(key, "product")) {
            sprintf(product, "%s", value);

        }else if (!strcmp(key, "stars")) {
            sprintf(stars, "%s", value);
        
        }else if (!strcmp(key, "comment")) {
            sprintf(comment, "%s", value);

        } else {
            throw std::invalid_argument("Stream error: invalid fields");
        }
    }

    return new Review(customer, product, stars, comment);
}
    