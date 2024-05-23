-- create the database :dbname
DROP DATABASE IF EXISTS :dbname ;
CREATE DATABASE :dbname ;

-- create the user :admin
\c :dbname postgres

REASSIGN OWNED BY :admin TO postgres ;

REVOKE ALL PRIVILEGES ON ALL TABLES IN SCHEMA public FROM :admin ;
REVOKE ALL PRIVILEGES ON ALL SEQUENCES IN SCHEMA public FROM :admin ;
REVOKE ALL PRIVILEGES ON ALL FUNCTIONS IN SCHEMA public FROM :admin ;

DROP OWNED BY :admin;
DROP USER IF EXISTS :admin ;

CREATE USER :admin WITH ENCRYPTED PASSWORD 'admin' ;

-- create the user :customer
\c :dbname postgres

REASSIGN OWNED BY :customer TO postgres ;

REVOKE ALL PRIVILEGES ON ALL TABLES IN SCHEMA public FROM :customer ;
REVOKE ALL PRIVILEGES ON ALL SEQUENCES IN SCHEMA public FROM :customer ;
REVOKE ALL PRIVILEGES ON ALL FUNCTIONS IN SCHEMA public FROM :customer ;

DROP OWNED BY :customer;
DROP USER IF EXISTS :customer ;

CREATE USER :customer WITH ENCRYPTED PASSWORD 'customer' ;

-- create the user :seller
\c :dbname postgres

REASSIGN OWNED BY :seller TO postgres ;

REVOKE ALL PRIVILEGES ON ALL TABLES IN SCHEMA public FROM :seller ;
REVOKE ALL PRIVILEGES ON ALL SEQUENCES IN SCHEMA public FROM :seller ;
REVOKE ALL PRIVILEGES ON ALL FUNCTIONS IN SCHEMA public FROM :seller ;

DROP OWNED BY :seller ;
DROP USER IF EXISTS :seller ;

CREATE USER :seller WITH ENCRYPTED PASSWORD 'seller' ;

-- create the user :courier
\c :dbname postgres

REASSIGN OWNED BY :courier TO postgres ;

REVOKE ALL PRIVILEGES ON ALL TABLES IN SCHEMA public FROM :courier ;
REVOKE ALL PRIVILEGES ON ALL SEQUENCES IN SCHEMA public FROM :courier ;
REVOKE ALL PRIVILEGES ON ALL FUNCTIONS IN SCHEMA public FROM :courier ;

DROP OWNED BY :courier ;
DROP USER IF EXISTS :courier ;

CREATE USER :courier WITH ENCRYPTED PASSWORD 'courier' ;