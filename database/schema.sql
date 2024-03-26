CREATE DOMAIN String AS VARCHAR(255);

CREATE TYPE statusOrder AS ENUM ('not shipped', 'shipped', 'delivered');


CREATE TABLE customer (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name String NOT NULL,
    mail String NOT NULL,
    password String NOT NULL,
    address String 
);

CREATE TABLE seller (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    piva VARCHAR(11) NOT NULL,
    name String NOT NULL,
    description TEXT,   
    mail String NOT NULL,
    password String NOT NULL
);

CREATE TABLE courier(
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name String NOT NULL,
    mail String NOT NULL,
    password String NOT NULL,
    warehouse INTEGER NOT NULL,

    FOREIGN KEY (warehouse) REFERENCES warehouse (id) ON DELETE CASCADE 
);

CREATE TABLE product (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name String NOT NULL,
    description TEXT NOT NULL,
    quantity INTEGER NOT NULL,
    price_tag FLOAT NOT NULL,
    seller INTEGER NOT NULL,
    warehouse INTEGER NOT NULL,

    FOREIGN KEY (seller) REFERENCES seller (id) ON DELETE CASCADE,
    FOREIGN KEY (warehouse) REFERENCES warehouse (id) ON DELETE CASCADE,
    check(quantity >= 0)
);

CREATE TABLE warehouse (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name String NOT NULL,
    address String NOT NULL

);

CREATE TABLE order (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    user INTEGER NOT NULL,
    product INTEGER NOT NULL,
    quantity INTEGER NOT NULL,
    date DATETIME NOT NULL,
    status statusOrder DEFAULT 'not shipped',

    FOREIGN KEY (user) REFERENCES user (id) ON DELETE CASCADE,
    FOREIGN KEY (courier) REFERENCES courier (id) ON DELETE CASCADE,
    FOREIGN KEY (product) REFERENCES product (id) ON DELETE CASCADE

);

CREATE TABLE wishList (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    user INTEGER NOT NULL,
    FOREIGN KEY (user) REFERENCES user (id) ON DELETE CASCADE

);

-- prodotti aggiunti alla wishlist 
CREATE TABLE addedProduct (
    product INTEGER NOT NULL,
    wishlist INTEGER NOT NULL, 

    PRIMARY KEY (product, wishlist),
    FOREIGN KEY (product) REFERENCES product (id) ON DELETE CASCADE,
    FOREIGN KEY (wishlist) REFERENCES wishlist (id) ON DELETE CASCADE

);


CREATE TABLE review (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    user INTEGER NOT NULL,
    product INTEGER NOT NULL,
    stars INTEGER NOT NULL,
    description TEXT,

    FOREIGN KEY (product) REFERENCES product (id) ON DELETE CASCADE,
    FOREIGN KEY (user) REFERENCES user (id) ON DELETE CASCADE,
    check(stars >= 1 AND stars <= 5)

);
