-- create the database :log_dbname
DROP DATABASE IF EXISTS :log_dbname ;
CREATE DATABASE :log_dbname ;

-- create the user :admin
\c :log_dbname postgres

REASSIGN OWNED BY :admin TO postgres ;

-- create the user :handler
\c :log_dbname postgres

REASSIGN OWNED BY :handler TO postgres ;

REVOKE ALL PRIVILEGES ON ALL TABLES IN SCHEMA public FROM :handler ;
REVOKE ALL PRIVILEGES ON ALL SEQUENCES IN SCHEMA public FROM :handler ;
REVOKE ALL PRIVILEGES ON ALL FUNCTIONS IN SCHEMA public FROM :handler ;

DROP OWNED BY :handler;
DROP USER IF EXISTS :handler ;

CREATE USER :handler WITH ENCRYPTED PASSWORD 'handler' ;