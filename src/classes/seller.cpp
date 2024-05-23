#include "seller.h"

Seller::Seller(char* seller_name, char* seller_piva, char* seller_description, char* seller_mail){

    name = (char*) malloc(sizeof(char) * PRMTRSIZE);
    piva = (char*) malloc(sizeof(char) * PRMTRSIZE);
    descrpition = (char*) malloc(sizeof(char) * PRMTRSIZE);
    mail = (char*) malloc(sizeof(char) * PRMTRSIZE);

    strcpy(name, seller_name);
    strcpy(piva, seller_piva);
    strcpy(descrpition, seller_description);
    strcpy(mail, seller_mail);
}

Seller::~Seller(){
    free(name);
    free(piva);
    free(descrpition);
    free(mail);
}

Seller* Seller::from_stream(redisReply* reply, int stream_num, int msg_num){

    char key[KEYSIZE];
    char value[PRMTRSIZE];

    char name[PRMTRSIZE];
    char piva[PRMTRSIZE];
    char description[PRMTRSIZE];
    char mail[PRMTRSIZE];

    // itero attraverso i campi del messaggio nel flusso Redis. Ogni campo è rappresentato da una coppia chiave-valore.
    for (int field_num = 2; field_num < ReadStreamMsgNumVal(reply, stream_num, msg_num); field_num += 2) {
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num, key);
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num + 1, value);

        if (!strcmp(key, "name")) {
            sprintf(name, "%s", value);
            
        }else if (!strcmp(key, "piva")) {
            sprintf(piva, "%s", value);
                
        }else if (!strcmp(key, "description")) {
            sprintf(description, "%s", value);
            
        }else if (!strcmp(key, "mail")) {
            sprintf(mail, "%s", value);

        } else {
            throw std::invalid_argument("Stream error: invalid fields");
        }
    }

    return new Seller(name, piva, description, mail);

}
    