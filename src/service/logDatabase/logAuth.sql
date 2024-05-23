-- try to understand if we need USAGE ON LANGUAGE
\c :log_dbname postgres

-- user already exists
GRANT ALL PRIVILEGES ON DATABASE :log_dbname to :admin;
GRANT CONNECT ON DATABASE :log_dbname TO :handler;

ALTER TABLE Client OWNER TO :admin;
ALTER TABLE Communication OWNER TO :admin;

GRANT ALL ON SCHEMA public TO :admin;
GRANT ALL PRIVILEGES ON ALL TABLES IN SCHEMA public TO :admin;

-- autoincrements
GRANT USAGE, SELECT ON ALL SEQUENCES IN SCHEMA public TO :admin;

-- handler
GRANT SELECT, INSERT, UPDATE ON Client TO :handler;
GRANT SELECT, INSERT, UPDATE ON Communication TO :handler;