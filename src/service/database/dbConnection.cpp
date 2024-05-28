#include "dbConnection.h"

//connessione al database
DbConnection::DbConnection(const char *hostname, const char *port, const char *username, const char *password,const char *dbname) {

    char info[1000]; // buffer contente tutte le infomazioni

    sprintf(info, "host=%s port=%s user=%s password=%s dbname=%s",hostname,port,username,password,dbname);

    // connette l'utente al database
    dbConnection = PQconnectdb(info);

    if (PQstatus(dbConnection) != CONNECTION_OK) {
        fprintf(stderr, "connectToDatabase(%s): Errore nella connessione al database!!: %s\n",
                dbname,
                PQerrorMessage(dbConnection)
        );
        disconnectFromDatabase();
    }

}

void DbConnection::disconnectFromDatabase() {

    PQfinish(dbConnection);
    dbConnection = NULL;
    exit(1);

}

//Esegue una query che non ritorna un valore.
PGresult* DbConnection::RunActionQuery(char *query){

    if (dbConnection == NULL){
        fprintf(stderr, "RunActionQuery(): Errore, connssione al db assente\n");
        exit (1);
    }

    PGresult *res = PQexec(dbConnection, query);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "RunActionQuery(): Errore nell'esecuzione di questa query: %s\n", query);
    }

    return (res);
}

PGresult* DbConnection::RunDataQuery(char *query)
{

    if (dbConnection == NULL){
        fprintf(stderr, "RunDataQuery(): Errore, connssione al db assente\n");
    }
    PGresult *res = PQexec(dbConnection, query);

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "RunDataQuery(): Errore nell'esecuzione di questa query: %s\n", query);
    }

    return (res);
}

PGresult* DbConnection::RunQuery(char* query, bool moreValue) {
    PGresult* resp; // resp: variabile per memorizzare il risultato della query principale

    PGresult* t_resp; // variabile per memorizzare il risultato delle query di inizio e fine transazione
    char sqlCmd[7]; //array di caratteri per memorizzare i comandi SQL di inizio e fine transazione

    //std::cout << "Eseguo la query: "<< query << std::endl;


    // Inizia una transazione
    sprintf(sqlCmd, "BEGIN"); //NON CAMBIARE "BEGIN"
    t_resp = RunActionQuery(sqlCmd);

    // Verifica se l'inizio della transazione è andato a buon fine
    if (PQresultStatus(t_resp) != PGRES_COMMAND_OK) {
        PQclear(t_resp);
        return t_resp;
    }

    PQclear(t_resp);

    // Esegue la query passata come parametro
    // Se moreValue è false, esegue una query di azione (INSERT, UPDATE, DELETE, ecc.)
    // Se moreValue è true, esegue una query di selezione dati (SELECT)
    // moreValue: parametro booleano che indica se la query restituirà più valori (true) o meno (false)
    resp = !moreValue ? RunActionQuery(query) : RunDataQuery(query);

    // Chiude la transazione
    sprintf(sqlCmd, "COMMIT");  //NON CAMBIARE "COMMIT"
    t_resp = RunActionQuery(sqlCmd);

    // Verifica se si sono verificati errori durante l'esecuzione della query
    if(PQresultStatus(resp) != PGRES_COMMAND_OK && PQresultStatus(resp) != PGRES_TUPLES_OK){
        PQclear(t_resp);
        PQclear(resp);
        return resp;
    }
        // Verifica se si sono verificati errori durante la chiusura della transazione
    else if(PQresultStatus(t_resp) != PGRES_COMMAND_OK){
        PQclear(resp);
        PQclear(t_resp);
        return t_resp;
    }
    else{
        PQclear(t_resp);
        // Se la query non ha restituito risultati (non è una SELECT), libera la memoria di resp
        if (PQresultStatus(resp) != PGRES_TUPLES_OK) PQclear(resp);
        return resp;
    }
}
