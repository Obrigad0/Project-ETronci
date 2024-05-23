#include "main.h"

// Funzione per sospendere l'esecuzione del programma per un determinato numero di microsecondi
int micro_sleep(long usec)
{
    struct timespec ts; // Struttura per specificare il tempo di sospensione
    int res; // Variabile per memorizzare il risultato della funzione nanosleep

    if (usec < 0)
    {
        errno = EINVAL; // Imposta l'errore a EINVAL (argomento non valido)
        return -1; // Ritorna -1 per indicare un errore
    }

    // Calcola i secondi e i nanosecondi dalla quantità di microsecondi
    ts.tv_sec = usec / 1000000;
    ts.tv_nsec = (usec % 1000000) * 1000;

    // Esegue nanosleep fino a quando non viene interrotto da un segnale
    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    return res; // Ritorna il risultato di nanosleep
}

int main() {
    // Connessione al database PostgreSQL
    DbConnection log_db = DbConnection(POSTGRESQL_SERVER, POSTGRESQL_PORT, POSTGRESQL_USER, POSTGRESQL_PSW, POSTGRESQL_DBNAME);
    PGresult *query_res; // Risultato della query

    char query[QUERYSIZE]; // Stringa per memorizzare la query

    while(1) {
        // Query per calcolare la media del tempo di disconnessione in millisecondi
        sprintf(query, "SELECT EXTRACT(EPOCH FROM AVG(disconnection_instant - connection_instant)) * 1000 as avg FROM Client WHERE disconnection_instant IS NOT NULL");

        // Esegue la query
        query_res = log_db.RunQuery(query, true);

        // Controlla se la query ha avuto successo e se ci sono risultati
        if ((PQresultStatus(query_res) != PGRES_COMMAND_OK && PQresultStatus(query_res) != PGRES_TUPLES_OK) || PQntuples(query_res) <= 0) {
            printf("DB_ERROR\n");
            continue;
        }

        // Ottiene il valore medio dalla query
        char* average = PQgetvalue(query_res, 0, PQfnumber(query_res, "avg"));

        // Se la media è vuota, imposta a "0"
        if(strlen(average)==0){
            sprintf(average, "0");
        }

        char response_status[8]; // Stato della risposta

        // Controlla se la media è inferiore o uguale al tempo massimo di connessione
        if (atof(average) <= MAX_CONNECTION_TIME_AVG) {
            sprintf(response_status, "SUCCESS");
        } else {
            sprintf(response_status, "ERROR");
        }

        // Inserisce le statistiche della sessione nel database
        sprintf(query, "INSERT INTO SessionStatistic(type, end_instant, value, response_status) VALUES ('Session', CURRENT_TIMESTAMP, %s, \'%s\')", average, response_status);

        // Esegue la query di inserimento
        query_res = log_db.RunQuery(query, false);

        // Controlla se la query di inserimento ha avuto successo
        if (PQresultStatus(query_res) != PGRES_COMMAND_OK && PQresultStatus(query_res) != PGRES_TUPLES_OK) {
            printf("DB_ERROR\n");
            continue;
        }

        // Query per calcolare la media del tempo di risposta in millisecondi
        sprintf(query, "SELECT EXTRACT(EPOCH FROM AVG(response_instant - request_instant)) * 1000 as avg FROM Communication WHERE response_instant IS NOT NULL");

        // Esegue la query
        query_res = log_db.RunQuery(query, true);

        // Controlla se la query ha avuto successo e se ci sono risultati
        if ((PQresultStatus(query_res) != PGRES_COMMAND_OK && PQresultStatus(query_res) != PGRES_TUPLES_OK) || PQntuples(query_res) <= 0) {
            printf("DB_ERROR\n");
            continue;
        }

        // Ottiene il valore medio dalla query
        average = PQgetvalue(query_res, 0, PQfnumber(query_res, "avg"));

        // Se la media è vuota, imposta a "0"
        if(strlen(average) == 0){
            sprintf(average, "0");
        }

        // Controlla se la media è inferiore o uguale al tempo massimo di risposta
        if (atof(average) <= MAX_RESPONSE_TIME_AVG) {
            sprintf(response_status, "SUCCESS");
        } else {
            sprintf(response_status, "ERROR");
        }

        // Inserisce le statistiche della risposta nel database
        sprintf(query, "INSERT INTO SessionStatistic(type, end_instant, value, response_status) VALUES ('Response', CURRENT_TIMESTAMP, %s, \'%s\')", average, response_status);

        // Esegue la query di inserimento
        query_res = log_db.RunQuery(query, false);

        // Controlla se la query di inserimento ha avuto successo
        if (PQresultStatus(query_res) != PGRES_COMMAND_OK && PQresultStatus(query_res) != PGRES_TUPLES_OK) {
            printf("DB_ERROR\n");
            continue;
        }

        // Sospende l'esecuzione per 60 secondi
        micro_sleep(60000000);
    }

    // Disconnette dal database
    log_db.disconnectFromDatabase();

    return 0;
}
