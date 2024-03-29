//#ifndef pgsql_h
//#define pgsql_h

#include <stdio.h>
#include <stdlib.h>

//extern "C"
//{
#include <libpq-fe.h>  // libreria libpq, utilizzata per la connessione al db Postgres
//}


class DbConnection {

private:
 	    
  PGconn *dbConnection; // dbconnection contiene il puntatore alla connessione al database
  // tramite dbconnection è possibile accedere alle informazioni sulla connessione e utilizzare le funzioni fornite dalla libreria libpq per interagire con il database

public:

  //Funzione che ci connette al database
  connectToDatabase(char *hostname, char *port, char *username, char *password, char *dbname);  //  PGconn*  connect2db(); 

  /* use this for commands returning no data, e.g. INSERT, UPDATE */  // vecchia :)
  //PGresult* ExecSQLcmd(char *sqlcmd); // vecchia :)

  // usata per eseguire Query che non ritornano dati. INSERT, UPDATE e DELETE
  PGresult* RunActionQuery(char *query); //TO DO 


  /* use this for commands returning data, e.g. SELECT */ // vecchia :)
  // PGresult* ExecSQLtuples(char *sqlcmd); // vecchia :)

  // usata per eseguire Query che ritornano dati. SELECT
  PGresult* RunDataQuery(char *query); //TO DO


  PGresult* RunQuery(char* query, bool is_tuples); // TO DO,  MICA HO CAPITO QUESTI PER COSA USANO STA FUNZIONE // vecchia

  //Funzione che ci disconnette dal database
  disconnectFromDatabase();
  
};


#endif 