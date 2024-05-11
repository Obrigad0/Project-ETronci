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

        // Only one stream --> stream_num = 0
        // Only one message in stream --> msg_num = 0
        ReadStreamNumMsgID(redReply, 0, 0, msg_id);

        // controllo la coppia chiave-valore in redis
        ReadStreamMsgVal(redReply, 0, 0, 0, first_key);
        ReadStreamMsgVal(redReply, 0, 0, 1, client_id);

        if(strcmp(first_key, "client_id") || (ReadStreamMsgNumVal(redReply, 0, 0) > 2)){
            send_response_status(redConn, WRITE_STREAM, client_id, "BAD_REQUEST", msg_id, 0);
            continue;
        }

        sprintf(query, "SELECT id, order_instant, fare, customer, card, zip_code, street, street_number FROM Order WHERE NOT EXISTS(SELECT * FROM AssignedDelivery WHERE order = id)");

        query_res = db.RunQuery(query, true);

        if (PQresultStatus(query_res) != PGRES_COMMAND_OK && PQresultStatus(query_res) != PGRES_TUPLES_OK) {
            send_response_status(redConn, WRITE_STREAM, client_id, "DB_ERROR", msg_id, 0);
            continue;
        }

        std::list<Order*> orders;

        for(int row = 0; row < PQntuples(query_res); row++) {
            Order * order;
            order = new Order(PQgetvalue(query_res, row, PQfnumber(query_res, "id")),
                                    PQgetvalue(query_res, row, PQfnumber(query_res, "order_instant")),
                                    PQgetvalue(query_res, row, PQfnumber(query_res, "fare")),
                                    PQgetvalue(query_res, row, PQfnumber(query_res, "customer")),
                                    PQgetvalue(query_res, row, PQfnumber(query_res, "card")),
                                    PQgetvalue(query_res, row, PQfnumber(query_res, "zip_code")),
                                    PQgetvalue(query_res, row, PQfnumber(query_res, "street")),
                                    PQgetvalue(query_res, row, PQfnumber(query_res, "street_number")));
            
            orders.push_back(order);
        }

        send_response_status(redConn, WRITE_STREAM, client_id, "REQUEST_SUCCESS", msg_id, PQntuples(query_res));

        for(int row = 0; row<PQntuples(query_res); row++) {
            Order* order = orders.front();

            orders.pop_front();

            redReply = RedisCommand(redConn, "XADD %s * row %d order_id %s order_instant %s order_fare %s order_customer %s order_card %s order_zip_code %s order_street %s order_street_number %s", WRITE_STREAM, row, order->id, order->order_instant, order->fare, order->customer, order->card, order->zip_code, order->street, order->street_number);
            assertReplyType(redConn, redReply, REDIS_REPLY_STRING);
            freeReplyObject(redReply);

        }
    }

    db.finish();

    return 0;
}