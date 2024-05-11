#include "main.h"

int main() {
    // connessioni con i db
    redisContext *redConn; // cambiata da c2r a redConn
    redisReply *redReply; // cambiata da reply a redPly

    // per comunicare con il db
    PGresult *query_res;

    // eliminato response[RESPONSE_LEN]
    std::string msg_id[MSGIDSIZE], redis_key[KEY_LEN], client_id[VALUESIZE];
    std::string query;

    // connessione ai 2 database
    Con2DB db(POSTGRESQL_SERVER, POSTGRESQL_PORT, POSTGRESQL_USER, POSTGRESQL_PSW, POSTGRESQL_DBNAME);
    redConn = redisConnect(REDIS_SERVER, REDIS_PORT);

    delivery = new Delivery(); // cambiata la classe da DeliveryPurchase a Delivery

    while(true) {

        // lettura dallo stream redis
        redReply = RedisCommand(redConn, "XREADGROUP GROUP main courier BLOCK 0 COUNT 1 STREAMS %s >", READ_STREAM);
        assertReply(redConn, redReply);

        if (ReadNumStreams(redReply) == 0) {
            continue;
        } 

        // Only one stream --> stream_num = 0
        // Only one message in stream --> msg_num = 0
        ReadStreamNumMsgID(redReply, 0, 0, msg_id);

        // controllo la coppia chiave-valore in redis
        ReadStreamMsgVal(redReply, 0, 0, 0, redis_key);
        ReadStreamMsgVal(redReply, 0, 0, 1, client_id);

        if(strcmp(redis_key, "client_id")){ 
            send_response_status(redConn, WRITE_STREAM, client_id, "BAD_REQUEST", msg_id, 0);
            continue;
        }

        // Convert request
        try{ // si prova a convertire il messaggio dello stream in un oggetto
            delivery = Delivery::from_stream(redReply, 0, 0);
        }
        catch(std::invalid_argument exp){
            send_response_status(redConn, WRITE_STREAM, client_id, "BAD_REQUEST", msg_id, 0);
            continue;
        }

        // prendo la stringa query associata alla delivery
        query = delivery->to_insert_query();
        
        query_res = db.RunQuery((char *) query.c_str(), false);

        if (PQresultStatus(query_res) != PGRES_COMMAND_OK && PQresultStatus(query_res) != PGRES_TUPLES_OK) {
            send_response_status(redConn, WRITE_STREAM, client_id, "DB_ERROR", msg_id, 0);
            continue;
        }

        send_response_status(redConn, WRITE_STREAM, client_id, "REQUEST_SUCCESS", msg_id, 0);
        
    }

    db.finish();

    return 0;
}