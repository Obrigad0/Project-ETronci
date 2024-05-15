#include "customer.h"

Customer::Customer(char* customer_name, char* customer_email, char* customer_password){

    name = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    mail = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    password = (char*) malloc(sizeof(char) * PARAMETERS_LEN);

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
    
    char key[KEY_LEN];
    char value[PARAMETERS_LEN];

    char name[PARAMETERS_LEN];
    char mail[PARAMETERS_LEN];
    char password[PARAMETERS_LEN];

    for (int field_num = 2; field_num < ReadStreamMsgNumVal(reply, stream_num, msg_num); field_num +=  2) {
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num, key);
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num + 1, value);
        
        if (!strcmp(key, "name")) {
            snprintf(name, PARAMETERS_LEN, "%s", value);

        } else if (!strcmp(key, "mail")) {
            snprintf(mail, PARAMETERS_LEN, "%s", value);

        } else if (!strcmp(key, "password")) {
            snprintf(password, PARAMETERS_LEN, "%s", value);

        } else {
            throw std::invalid_argument("Stream error: invalid fields");
        }
    }

    return new Customer(name, mail, password);
}
    