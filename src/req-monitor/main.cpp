#include "main.h"


int micro_sleep(long usec)
{
    struct timespec ts;
    int res;

    if (usec < 0) {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = usec / 1000000;
    ts.tv_nsec = (usec % 1000000) * 1000;

    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    return res;  
}


// monitoro la durata media delle sessioni dei clienti e il tempo medio di risposta delle comunicazioni.

int main() {
    Con2DB log_db = Con2DB(POSTGRESQL_SERVER, POSTGRESQL_PORT, POSTGRESQL_USER, POSTGRESQL_PSW, POSTGRESQL_DBNAME);
    PGresult *query_res;
    char query[QUERYSIZE];

    while(true) {
        // Query per calcolare la durata media delle sessioni in millisecondi
        sprintf(query, "SELECT EXTRACT(EPOCH FROM AVG(disconnection_instant - connection_instant)) * 1000 as avg FROM Client WHERE disconnection_instant IS NOT NULL");

        query_res = log_db.RunQuery(query, true);

        if ((PQresultStatus(query_res) != PGRES_COMMAND_OK && PQresultStatus(query_res) != PGRES_TUPLES_OK) || PQntuples(query_res) <= 0) {
            printf("DB_ERROR\n");
            continue;
        }

        char* average = PQgetvalue(query_res, 0, PQfnumber(query_res, "avg"));

        if(strlen(average) == 0) sprintf(average, "0");      

        char response_status[8];

        // Verifica se la durata media della sessione è entro il limite consentito
        if (atof(average) <= MAX_CONNECTION_TIME) {
            sprintf(response_status, "SUCCESS");
        } else {
            sprintf(response_status, "ERROR");
        }

        // Query per inserire le statistiche della sessione nel database
        sprintf(query, "INSERT INTO SessionStatistic(type, end_instant, value, response_status) VALUES ('Session', CURRENT_TIMESTAMP, %s, \'%s\')", average, response_status);

        query_res = log_db.RunQuery(query, false);
        if (PQresultStatus(query_res) != PGRES_COMMAND_OK && PQresultStatus(query_res) != PGRES_TUPLES_OK) {
            printf("DB_ERROR\n");
            continue;
        }

        // Query per calcolare il tempo medio di risposta in millisecondi
        sprintf(query, "SELECT EXTRACT(EPOCH FROM AVG(response_instant - request_instant)) * 1000 as avg FROM Communication WHERE response_instant IS NOT NULL");

        query_res = log_db.RunQuery(query, true);

        if ((PQresultStatus(query_res) != PGRES_COMMAND_OK && PQresultStatus(query_res) != PGRES_TUPLES_OK) || PQntuples(query_res) <= 0) {
            printf("DB_ERROR\n");
            continue;
        }

        average = PQgetvalue(query_res, 0, PQfnumber(query_res, "avg"));

        if(strlen(average) == 0)sprintf(average, "0");       

        // Verifica se il tempo medio di risposta è entro il limite consentito
        if (atof(average) <= MAX_RESPONSE_TIME_AVG) {
            sprintf(response_status, "SUCCESS");
        } else {
            sprintf(response_status, "ERROR");
        }

        // Query per inserire le statistiche del tempo di risposta nel database
        sprintf(query, "INSERT INTO SessionStatistic(type, end_instant, value, response_status) VALUES ('Response', CURRENT_TIMESTAMP, %s, \'%s\')", average, response_status);

        query_res = log_db.RunQuery(query, false);

        if (PQresultStatus(query_res) != PGRES_COMMAND_OK && PQresultStatus(query_res) != PGRES_TUPLES_OK) {
            printf("DB_ERROR\n");
            continue;
        }

        micro_sleep(60000000);
    }

    log_db.finish();

    return 0;
}