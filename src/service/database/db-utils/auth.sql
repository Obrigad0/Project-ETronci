-- try to understant if we need USAGE ON LANGUAGE
\c :dbname postgres

-- user already exists
GRANT ALL PRIVILEGES ON DATABASE :dbname to :admin;
GRANT CONNECT ON DATABASE :dbname TO :courier;
GRANT CONNECT ON DATABASE :dbname TO :seller;
GRANT CONNECT ON DATABASE :dbname TO :customer;

ALTER TABLE Customer OWNER TO :admin;
ALTER TABLE Seller OWNER TO :admin;
ALTER TABLE Courier OWNER TO :admin;
ALTER TABLE Product OWNER TO :admin;
ALTER TABLE Warehouse OWNER TO :admin;
ALTER TABLE OrderedProducts OWNER TO :admin;
ALTER TABLE Delivery OWNER TO :admin;
ALTER TABLE Review OWNER TO :admin;
 --  TABLE blank OWNER TO :admin;

GRANT ALL ON SCHEMA public TO :admin;
GRANT ALL PRIVILEGES ON ALL TABLES IN SCHEMA public TO :admin;

-- grant usage and select permission to autoincrements

GRANT USAGE, SELECT ON ALL SEQUENCES IN SCHEMA public TO :admin;
GRANT USAGE, SELECT ON ALL SEQUENCES IN SCHEMA public TO :customer;
GRANT USAGE, SELECT ON ALL SEQUENCES IN SCHEMA public TO :courier;
GRANT USAGE, SELECT ON ALL SEQUENCES IN SCHEMA public TO :seller;

-- courier

GRANT SELECT ON OrderedProducts TO :courier;
GRANT INSERT, UPDATE, SELECT ON Delivery TO :courier;

-- seller

GRANT INSERT, UPDATE, SELECT, DELETE ON Product TO :seller;
GRANT INSERT, SELECT ON Product TO :seller;
GRANT INSERT ON Seller TO :seller;
GRANT SELECT ON OrderedProducts TO :seller;

-- customer

GRANT INSERT ON Review TO :customer;
GRANT SELECT ON Product TO :customer;
GRANT SELECT ON Delivery TO :customer;
GRANT SELECT, INSERT ON Customer TO :customer;
GRANT SELECT, INSERT ON OrderedProducts TO :customer;
