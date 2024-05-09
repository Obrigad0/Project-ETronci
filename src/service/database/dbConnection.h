#ifndef pgsql_h
#define pgsql_h

#include <stdio.h>
#include <stdlib.h>


#include <postgresql/libpq-fe.h>  // libreria libpq, utilizzata per la connessione al db Postgres
// per farlo funzionare ricordate di installare questa libreria.
// il comando e':  sudo apt-get install libpq-dev
// potrebbe funzionare anche cosi: #include <libpq-fe.h>



class DbConnection {

private:
 	    
  PGconn *dbConnection; // dbconnection contiene il puntatore alla connessione al database
  // tramite dbconnection è possibile accedere alle informazioni sulla connessione e utilizzare le funzioni fornite dalla libreria libpq per interagire con il database

public:

  //Costruttore
  DbConnection(const char *hostname,const char *port,const char *username,const char *password,const char *dbname); //void, dopo controllare TO DO?

  // usata per eseguire Query che non ritornano dati. INSERT, UPDATE e DELETE
  PGresult* RunActionQuery(char *query); //ex ExecSQLcmd(char *sqlcmd); TO DO L.C.

  // usata per eseguire Query che ritornano dati. SELECT
  PGresult* RunDataQuery(char *query); //TO DO ex ExecSQLtuples(char *sqlcmd); TO DO L.C.

  // runQuery e' la funzione principale per l'esecuzione delle query, chiama, a seconda del valore di moreValue, o RunActionQuery o RunDataQuery
  PGresult* RunQuery(char* query, bool moreValue);

  //Funzione che ci disconnette dal database
  void disconnectFromDatabase();
  
};


#endif 