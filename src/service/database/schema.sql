\c :dbname

CREATE DOMAIN String AS VARCHAR(255);
CREATE DOMAIN PartitaIva AS VARCHAR(11);
CREATE TYPE StatusOrder AS ENUM ('not shipped', 'shipped', 'delivered');


CREATE TABLE IF NOT EXISTS Customer (
    id SERIAL PRIMARY KEY ,
    name String NOT NULL,
    mail String NOT NULL,
    UNIQUE(mail)
);

CREATE TABLE IF NOT EXISTS Seller (
    id SERIAL PRIMARY KEY ,
    name String NOT NULL,
    piva PartitaIva NOT NULL,
    description TEXT,
    mail String NOT NULL,
    UNIQUE(mail)

);

CREATE TABLE IF NOT EXISTS  Warehouse (
    id SERIAL PRIMARY KEY,
    name String NOT NULL,
    address String NOT NULL
);


CREATE TABLE IF NOT EXISTS Courier(
    id SERIAL PRIMARY KEY ,
    name String NOT NULL,
    mail String NOT NULL,
    warehouse INTEGER NOT NULL,
    FOREIGN KEY (warehouse) REFERENCES Warehouse(id) ON DELETE CASCADE,
    UNIQUE(mail)
);

CREATE TABLE IF NOT EXISTS Product (
    id SERIAL PRIMARY KEY,
    name String NOT NULL,
    description TEXT NOT NULL,
    price_tag FLOAT NOT NULL,
    seller INTEGER NOT NULL,
    warehouse INTEGER NOT NULL,
    FOREIGN KEY (seller) REFERENCES Seller(id) ON DELETE CASCADE,
    UNIQUE(name, seller),
    FOREIGN KEY (warehouse) REFERENCES Warehouse(id) ON DELETE CASCADE
);


-- ex Ordine
CREATE TABLE IF NOT EXISTS OrderedProduct (
    id SERIAL PRIMARY KEY,
    customer INTEGER NOT NULL,
    product INTEGER NOT NULL,
    quantity INTEGER NOT NULL,
    date TIMESTAMP NOT NULL,
    zip_code String NOT NULL,
    address String NOT NULL,

    FOREIGN KEY (customer) REFERENCES Customer(id) ON DELETE CASCADE,
    FOREIGN KEY (product) REFERENCES Product(id) ON DELETE CASCADE,
    CHECK (quantity > 0)
);

CREATE TABLE IF NOT EXISTS Delivery (
    id SERIAL PRIMARY KEY,
    orderId INTEGER NOT NULL, -- cambiato il nome, prima era order
    courier INTEGER NOT NULL,
    date TIMESTAMP NOT NULL,
    status StatusOrder DEFAULT 'not shipped',

    FOREIGN KEY (orderId) REFERENCES OrderedProduct(id) ON DELETE CASCADE,
    FOREIGN KEY (courier) REFERENCES Courier(id) ON DELETE CASCADE

);

CREATE TABLE IF NOT EXISTS Review (
    id SERIAL PRIMARY KEY,
    customer INTEGER NOT NULL,
    product INTEGER NOT NULL,
    stars INTEGER NOT NULL,
    comment TEXT,
    FOREIGN KEY (product) REFERENCES Product(id) ON DELETE CASCADE,
    FOREIGN KEY (customer) REFERENCES Customer(id) ON DELETE CASCADE,
    UNIQUE(customer, product),
    check(stars >= 1 AND stars <= 5)
);
