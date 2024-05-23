\c :log_dbname

CREATE DOMAIN StringS AS VARCHAR(50);
CREATE DOMAIN StringL AS VARCHAR(1000);

CREATE DOMAIN RealGEZ AS real CHECK (VALUE >= 0);

CREATE TYPE SessionStatisticType AS ENUM ('Session', 'Response');

CREATE TABLE IF NOT EXISTS Client (
                                      server_name StringS NOT NULL,
                                      file_descriptor integer NOT NULL,
                                      connection_instant timestamp NOT NULL,
                                      disconnection_instant timestamp,

                                      PRIMARY KEY (server_name, file_descriptor, connection_instant),
    CONSTRAINT disconnection_instant_after_connection_instant CHECK (disconnection_instant IS NULL OR disconnection_instant >= connection_instant)
    );

CREATE TABLE IF NOT EXISTS Communication (
                                             request StringL NOT NULL,
                                             request_instant timestamp NOT NULL,
                                             response_status StringS,
                                             response_instant timestamp,
                                             client_server_name StringS NOT NULL,
                                             client_file_descriptor integer NOT NULL,
                                             client_connection_instant timestamp NOT NULL,

                                             PRIMARY KEY (client_server_name, client_file_descriptor, client_connection_instant, request_instant),
    CONSTRAINT fk_communication_client FOREIGN KEY (client_server_name, client_file_descriptor, client_connection_instant) REFERENCES Client(server_name, file_descriptor, connection_instant),
    CONSTRAINT response_instant_after_request_instant CHECK (response_instant IS NULL OR response_instant >= request_instant),
    CONSTRAINT request_instant_after_connection_instant CHECK (request_instant >= client_connection_instant)
    );

CREATE TABLE IF NOT EXISTS SessionStatistic (
                                                id SERIAL NOT NULL PRIMARY KEY,
                                                type SessionStatisticType NOT NULL,
                                                end_instant timestamp NOT NULL,
                                                value RealGEZ NOT NULL,
                                                response_status StringS NOT NULL,
                                                UNIQUE (type, end_instant)

    );