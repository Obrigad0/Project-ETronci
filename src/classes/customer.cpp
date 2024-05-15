#include "customer.h"

Customer::Customer(char* customer_name, char* customer_email, char* customer_password){

    name = (char*) malloc(sizeof(char) * PRMTRSIZE);
    mail = (char*) malloc(sizeof(char) * PRMTRSIZE);
    password = (char*) malloc(sizeof(char) * PRMTRSIZE);

    strcpy(name, customer_name);
    strcpy(mail, customer_email);                                                                                  
    strcpy(password, customer_password);
}

Customer::~Customer(){
    free(name);
    free(mail);
    free(password);
}

Customer* Customer::from_stream(redisReply* reply, int stream_num, int msg_num){
    
    char key[KEYSIZE];
    char value[PRMTRSIZE];

    char name[PRMTRSIZE];
    char mail[PRMTRSIZE];
    char password[PRMTRSIZE];

    for (int field_num = 2; field_num < ReadStreamMsgNumVal(reply, stream_num, msg_num); field_num +=  2) {
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num, key);
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num + 1, value);
        
        if (!strcmp(key, "name")) {
            snprintf(name, PRMTRSIZE, "%s", value);

        } else if (!strcmp(key, "mail")) {
            snprintf(mail, PRMTRSIZE, "%s", value);

        } else if (!strcmp(key, "password")) {
            snprintf(password, PRMTRSIZE, "%s", value);

        } else {
            throw std::invalid_argument("Stream error: invalid fields");
        }
    }

    return new Customer(name, mail, password);
}
    