#include "main.h"

int main() {
    redisContext *redConn;
    redisReply *redReply;

    PGresult *query_res;

    char query[QUERYSIZE], msg_id[MSGIDSIZE], first_key[KEYSIZE], client_id[VALUESIZE];

    DbConnection db(POSTGRESQL_SERVER, POSTGRESQL_PORT, POSTGRESQL_USER, POSTGRESQL_PSW, POSTGRESQL_DBNAME);
    redConn = redisConnect(REDIS_SERVER, REDIS_PORT);

    Seller* seller;

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

        // Convert request
        try{
            seller = Seller::from_stream(redReply, 0, 0);
        }
        catch(std::invalid_argument exp){
            send_response_status(redConn, WRITE_STREAM, client_id, "BAD_REQUEST", msg_id, 0);
            continue;
        }
        //std::string n = seller->name;
        //std::string d = seller->descrpition;
        //n = replace_substring(n, REDISSPACE, BLANKSPACE);
        //d = replace_substring(d, REDISSPACE, BLANKSPACE);

        sprintf(query, "INSERT INTO Seller (name, piva, description, mail) VALUES (\'%s\', \'%s\', \'%s\', \'%s\')",
                seller->name, seller->piva,seller->descrpition, seller->mail);

        query_res = db.RunQuery(query, false);

        if (PQresultStatus(query_res) != PGRES_COMMAND_OK && PQresultStatus(query_res) != PGRES_TUPLES_OK) {
            send_response_status(redConn, WRITE_STREAM, client_id, "DB_ERROR", msg_id, 0);
            continue;
        }

        send_response_status(redConn, WRITE_STREAM, client_id, "REQUEST_SUCCESS", msg_id, 0);
        
    }

    db.disconnectFromDatabase();

    return 0;
}