#include "main.h"

int main() {
    redisContext *c2r;
    redisReply *reply;

    PGresult *query_res;

    char query[QUERYSIZE], response[RESPONSE_LEN], msg_id[MSGIDSIZE], first_key[KEY_LEN], client_id[VALUESIZE], second_key[KEY_LEN], product_name[VALUESIZE];

    Con2DB db(POSTGRESQL_SERVER, POSTGRESQL_PORT, POSTGRESQL_USER, POSTGRESQL_PSW, POSTGRESQL_DBNAME);
    c2r = redisConnect(REDIS_SERVER, REDIS_PORT);

    while(true) {

        reply = RedisCommand(c2r, "XREADGROUP GROUP main customer BLOCK 0 COUNT 1 STREAMS %s >", READ_STREAM);

        assertReply(c2r, reply);

        if (ReadNumStreams(reply) == 0) {
            continue;
        } 

        // stream_num = 0  indica un singolo stream
        // msg_num = 0     indica un solo messaggio nello stream
        ReadStreamNumMsgID(reply, 0, 0, msg_id);

        // controllo la coppia chiave-valore in redis
        ReadStreamMsgVal(reply, 0, 0, 0, first_key);
        ReadStreamMsgVal(reply, 0, 0, 1, client_id); 

        if(strcmp(first_key, "client_id")){
            send_response_status(c2r, WRITE_STREAM, client_id, "BAD_REQUEST", msg_id, 0);
            continue;
        }

        // per ricevere input
        ReadStreamMsgVal(reply, 0, 0, 2, second_key);
        ReadStreamMsgVal(reply, 0, 0, 3, product_name); 
        
        if(strcmp(second_key, "product_name") || (ReadStreamMsgNumVal(reply, 0, 0) > 4)){
            send_response_status(c2r, WRITE_STREAM, client_id, "BAD_REQUEST", msg_id, 0);
            continue;
        }

        std::string str_product_name = product_name;
        std::string search_parameter = "%"+ str_product_name + "%";
        sprintf(query, "SELECT * FROM product WHERE name LIKE \'%s\' ", (char*)search_parameter.c_str());

        query_res = db.RunQuery(query, true);

        if (PQresultStatus(query_res) != PGRES_COMMAND_OK && PQresultStatus(query_res) != PGRES_TUPLES_OK) {
            send_response_status(c2r, WRITE_STREAM, client_id, "DB_ERROR", msg_id, 0);
            continue;
        }

        std::list<Product*> products;

        // mi interessa prendere anche seller e warehouse?
        for(int row = 0; row < PQntuples(query_res); row++){
            Product * product;
            product = new Product(PQgetvalue(query_res, row, PQfnumber(query_res, "id")),
                                        PQgetvalue(query_res, row, PQfnumber(query_res, "name")),
                                        PQgetvalue(query_res, row, PQfnumber(query_res, "description")),
                                        PQgetvalue(query_res, row, PQfnumber(query_res, "price_tag")));
            
            products.push_back(product);
        }

        send_response_status(c2r, WRITE_STREAM, client_id, "REQUEST_SUCCESS", msg_id, PQntuples(query_res));
        
        for(int row = 0; row<PQntuples(query_res); row++){

            Product *p = products.front();

            products.pop_front();

            reply = RedisCommand(c2r, "XADD %s * row %d id %s name %s description %s price_tag %s", WRITE_STREAM, row, p->id, p->name, p->description, p->price_tag);
            assertReplyType(c2r, reply, REDIS_REPLY_STRING);
            freeReplyObject(reply);

        }
    }

    db.finish();

    return 0;
}