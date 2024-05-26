\c :dbname


INSERT INTO Customer (mail, name)
VALUES
    ('giuseppe@gmail.com', 'giuseppe'),
    ('senad71@gmail.com', 'senad'),
    ('giorgioChina9@gmail.com', 'giorgio') ;

INSERT INTO Seller (name, piva, description, mail)
VALUES
    ('OfficialSSLazioStore.com', '12345678910', 'Official store della prima e l unica squadra della Capitale!', 'lazio@gmail.com'),
    ('MerendineBuonissime.it', '12345678911', 'Le merendine piu buone del mondo!','merendine@gmail.com'),
    ('SportShopEU.com', '12345678912', 'Prodotti sportivi in tutta Europa!','sportShop@gmail.com'),
    ('ITComputerStore.it', '12345678913', 'I migliori calcolatori a prezzi stracciati!','ITcomputer@gmail.com') ;

INSERT INTO Warehouse (name,address)
VALUES
    ('Roma1','via fuori Il Raccordo 69'),
    ('Milano1','via poco fuori Milano 71'),
    ('Napoli1','via poco fuori Napoli 26');


INSERT INTO Courier (name, mail, warehouse)
VALUES
    ('WH1Courier','WH1Courier@gmail.com',1),
    ('WH2Courier','WH2Courier@gmail.com',2),
    ('WH3Courier','WH3Courier@gmail.com',3);

INSERT INTO Product (name, description, price_tag, seller, warehouse)
VALUES
    ('Prima Maglia', 'Prima maglia Lazio 23/24', 95.99, 1, 1),
    ('Cioco rotella', 'Buonissima ruota di cioccolato da 10kg!', 30.00, 2, 3),
    ('Scarpe da corsa', 'Scarpe da corsa molto veloci!', 199.99 , 3, 2),
    ('Super computer quantisitico', 'Computer quantistico creato da google!', 799.99 , 3, 2);

-- non presenti OrederdProduct, Delivery e Review