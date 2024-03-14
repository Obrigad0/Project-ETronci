
CREATE TABLE users (
    id INTEGER PRIMARY KEY,
    name VARCHAR(255) NOT NULL,
    mail VARCHAR(255) NOT NULL,
    password VARCHAR (255) NOT NULL
);

-- sellers oppure providers ?
CREATE TABLE sellers (
    id INTEGER PRIMARY KEY,
    name VARCHAR(255) NOT NULL,
    description VARCHAR(511),   --magari una breve descrizione dell'azienda boh
    mail VARCHAR(255) NOT NULL,
    password VARCHAR (255) NOT NULL
)

CREATE TABLE carriers (
    id INTEGER PRIMARY KEY,
    name VARCHAR(255) NOT NULL,
    mail VARCHAR(255) NOT NULL,
    password VARCHAR (255) NOT NULL
)

CREATE TABLE product (
    id INTEGER PRIMARY KEY,
    name VARCHAR(255) NOT NULL,
    description VARCHAR(1023) NOT NULL,
    price INTEGER NOT NULL,
    seller INTEGER NOT NULL,
    FOREIGN KEY (seller) REFERENCES sellers (id) -- ON DELETE CASCADE ?
)