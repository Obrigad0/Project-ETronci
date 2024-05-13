#include "main.h"

int main() {
    redisContext *redConn;
    redisReply *redReply;

    PGresult *query_res;

    char query[QUERYSIZE], response[RESPONSE_LEN], msg_id[MSGIDSIZE], first_key[KEY_LEN], client_id[VALUESIZE], second_key[KEY_LEN], purchase_id[VALUESIZE];

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

        if(strcmp(first_key, "client_id")){
            send_response_status(redConn, WRITE_STREAM, client_id, "BAD_REQUEST", msg_id, 0);
            continue;
        }

        // input
        ReadStreamMsgVal(redReply, 0, 0, 2, second_key);
        ReadStreamMsgVal(redReply, 0, 0, 3, purchase_id);

        if(strcmp(second_key, "purchase_id") || (ReadStreamMsgNumVal(redReply, 0, 0) > 4)){
            send_response_status(redConn, WRITE_STREAM, client_id, "BAD_REQUEST", msg_id, 0);
            continue;
        }
        
        // per ora prendo solo gli ordini che sono stati assegnati ad una spedizine
        // però dovremmo prendere anche quelli non assegnati. magari usando un oggetto delivery vuoto?
        sprintf(query, "SELECT * FROM order JOIN delivery ON delivery.order = order.id;")

        query_res = db.RunQuery(query, true);

        if ((PQresultStatus(query_res) != PGRES_COMMAND_OK && PQresultStatus(query_res) != PGRES_TUPLES_OK) || (PQntuples(query_res) == 0)) {
            send_response_status(redConn, WRITE_STREAM, client_id, "BAD_REQUEST", msg_id, 0);
            continue;
        }

        std::list<Order*> orders;
        std::list<Delivery*> deliveries;

        for(int row = 0; row < PQntuples(query_res); row++) {
            Order * order;
            order = new Order(PQgetvalue(query_res, row, PQfnumber(query_res, "id")),
                                PQgetvalue(query_res, row, PQfnumber(query_res, "customer")),
                                PQgetvalue(query_res, row, PQfnumber(query_res, "product")),
                                PQgetvalue(query_res, row, PQfnumber(query_res, "quanity")),
                                PQgetvalue(query_res, row, PQfnumber(query_res, "date")),
                                PQgetvalue(query_res, row, PQfnumber(query_res, "zip_code")),
                                PQgetvalue(query_res, row, PQfnumber(query_res, "address")));
            
            orders.push_back(order);

            Delivery * delivery;
            delivery = new delivery(PQgetvalue(query_res, row, PQfnumber(query_res, "id")),
                                    PQgetvalue(query_res, row, PQfnumber(query_res, "order")),
                                    PQgetvalue(query_res, row, PQfnumber(query_res, "courier")),
                                    PQgetvalue(query_res, row, PQfnumber(query_res, "date")),
                                    PQgetvalue(query_res, row, PQfnumber(query_res, "status")));

            deliveries.push_back(delivery);
        }

        send_response_status(redConn, WRITE_STREAM, client_id, "REQUEST_SUCCESS", msg_id, PQntuples(query_res));

        for(int row = 0; row<PQntuples(query_res); row++) {

            Order* order = orders.front();
            orders.pop_front();

            Delivery* delivery = deliveries.front();
            deliveries.pop_front();

            // non ho compreso order di Delivery perchè mi sembre inutile
            redReply = RedisCommand(redConn, "XADD %s * row %d order_id %s customer %s product %s quantity %s date %s zip_code %s address %s delivery_id %s courier %s delivery_date %s status %s", WRITE_STREAM, row, order->id, order->customer, order->product, order->quantity, order->date, order->zip_code, order->address, delivery->id, delivery->courier, delivery->date, delivery->status);
            assertReplyType(redConn, redReply, REDIS_REPLY_STRING);
            freeReplyObject(redReply);
        }
    }

    db.finish();

    return 0;
}