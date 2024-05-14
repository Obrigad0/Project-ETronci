#include "seller.h"

Seller::Seller(char* seller_id, char* seller_name, char* seller_piva, char* seller_description, char* seller_mail, char* seller_password){

    id = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    name = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    piva = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    descrpition = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    mail = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    password = (char*) malloc(sizeof(char) * PARAMETERS_LEN);

    strcpy(id, seller_id);
    strcpy(name, seller_name);
    strcpy(piva, seller_piva);
    strcpy(descrpition, seller_description);
    strcpy(mail, seller_mail);
    strcpy(password, seller_password);
}

Seller::~Seller(){
    free(id);
    free(name);
    free(piva);
    free(descrpition);
    free(mail);
    free(password);
}

Seller* Seller::from_stream(redisReply* reply, int stream_num, int msg_num){

    char key[KEY_LEN];
    char value[PARAMETERS_LEN];

    char name[PARAMETERS_LEN];
    char piva[PARAMETERS_LEN];
    char description[PARAMETERS_LEN];
    char mail[PARAMETERS_LEN];
    char password[PARAMETERS_LEN];

    // itero attraverso i campi del messaggio nel flusso Redis. Ogni campo è rappresentato da una coppia chiave-valore.
    for (int field_num = 2; field_num < ReadStreamMsgNumVal(reply, stream_num, msg_num); field_num += 2) {
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num, key);
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num + 1, value);

        if (!strcmp(key, "id")) {
            sprintf(id, "%s", value);
        
        }else if (!strcmp(key, "name")) {
            sprintf(name, "%s", value);
            
        }else if (!strcmp(key, "piva")) {
            sprintf(piva, "%s", value);
                
        }else if (!strcmp(key, "description")) {
            sprintf(description, "%s", value);
            
        }else if (!strcmp(key, "mail")) {
            sprintf(mail, "%s", value);
        
        }else if (!strcmp(key, "password")) {
            sprintf(password, "%s", value);

        } else {
            throw std::invalid_argument("Stream error: invalid fields");
        }
    }

    return new Seller(id, name, piva, description, mail, password);

}
    