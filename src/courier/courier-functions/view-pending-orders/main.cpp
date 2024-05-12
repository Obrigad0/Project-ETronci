#include "main.h"

int main() {
    redisContext *redConn;
    redisReply *redReply;

    PGresult *query_res;

    char query[QUERYSIZE], response[RESPONSE_LEN], msg_id[MSGIDSIZE], first_key[KEY_LEN], client_id[VALUESIZE], order_id[VALUESIZE];

    Con2DB db(POSTGRESQL_SERVER, POSTGRESQL_PORT, POSTGRESQL_USER, POSTGRESQL_PSW, POSTGRESQL_DBNAME);
    redConn = redisConnect(REDIS_SERVER, REDIS_PORT);

    while(true) {
        redReply = RedisCommand(redConn, "XREADGROUP GROUP main customer BLOCK 0 COUNT 1 STREAMS %s >", READ_STREAM);

        assertReply(redConn, redReply);

        if (ReadNumStreams(redReply) == 0) {
            continue;
        } 

        // stream_num = 0  indica un singolo stream
        // msg_num = 0     indica un solo messaggio nello stream
        ReadStreamNumMsgID(redReply, 0, 0, msg_id);

        // controllo la coppia chiave-valore in redis
        ReadStreamMsgVal(redReply, 0, 0, 0, first_key);
        ReadStreamMsgVal(redReply, 0, 0, 1, client_id);

        if(strcmp(first_key, "client_id") || (ReadStreamMsgNumVal(redReply, 0, 0) > 2)){
            send_response_status(redConn, WRITE_STREAM, client_id, "BAD_REQUEST", msg_id, 0);
            continue;
        }

        // order potrebbe dare problemi nelle query?
        sprintf(query, "SELECT * FROM order WHERE id NOT IN (SELECT order FROM delivery)");

        query_res = db.RunQuery(query, true);
        if (PQresultStatus(query_res) != PGRES_COMMAND_OK && PQresultStatus(query_res) != PGRES_TUPLES_OK) {
            send_response_status(redConn, WRITE_STREAM, client_id, "DB_ERROR", msg_id, 0);
            continue;
        }

        // compongo la lista di tutti gli ordini disponibili per il corriere
        std::list<Order*> orders;

        for(int row = 0; row < PQntuples(query_res); row++) {
            Order *order;
            order = new Order(PQgetvalue(query_res, row, PQfnumber(query_res, "id")),
                                PQgetvalue(query_res, row, PQfnumber(query_res, "date")),
                                PQgetvalue(query_res, row, PQfnumber(query_res, "product")),
                                PQgetvalue(query_res, row, PQfnumber(query_res, "quantity")),
                                PQgetvalue(query_res, row, PQfnumber(query_res, "customer")),
                                PQgetvalue(query_res, row, PQfnumber(query_res, "zip_code")),
                                PQgetvalue(query_res, row, PQfnumber(query_res, "address")));
            
            orders.push_back(order);
        }

        send_response_status(redConn, WRITE_STREAM, client_id, "REQUEST_SUCCESS", msg_id, PQntuples(query_res));

        for(int row = 0; row < PQntuples(query_res); row++) {
            Order* order = orders.front();

            orders.pop_front();

            redReply = RedisCommand(redConn, "XADD %s * row %d order_id %s order_date %s product_id %s product_quantity %s order_customer %s order_zip_code %s order_address %s", 
                                        WRITE_STREAM, row, order->id, order->date, order->product, order->quantity, order->customer, order->zip_code, order->address);
            assertReplyType(redConn, redReply, REDIS_REPLY_STRING);
            freeReplyObject(redReply);

        }
    }

    db.finish();

    return 0;
}