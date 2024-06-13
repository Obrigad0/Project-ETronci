Progetto per il corso di Ingegneria del Software del Prof. Enrico Tronci 2023/2024, svolto da *Flavio Corsetti* (1997818), *Nicolas Lattanzi* (1992502) e *Beatrice Lettieri* (1945750).
# Specifiche progetto
Come da specifica, il nostro progetto è il back-end di un sito e-commerce, e include diverse componenti:
- un modello (test generator) per i clienti (customers), cioè coloro che acquistano i prodotti in vendita;
- un modello per i fornitori (sellers), cioè coloro che inseriscono nel sito prodotti da vendere;
- un modello per i trasportatori (couriers), cioè coloro che consegnano il prodotto al customer;
- un server al quale i clienti si connettono per interagire con il sistema;
- un server al quale i fornitori si connettono per interagire con il sistema;
- un server al quale i trasportatori si connettono per interagire con il sistema;
- un database per i dati ed i log;
- monitor per 3 proprietà funzionali;
- monitor per 2 proprietà non funzionali.
---
## Relazione
Per informazioni specifiche sulla struttura del progetto, diagrammi esplicativi per il database e descrizioni dei monitors, vi preghiamo di consultare la [relazione](<https://raw.githubusercontent.com/Obrigad0/Project-ETronci/main/relazione/relazione.pdf>).

---
# Compilare ed eseguire il progetto
Per compilare ed eseguire il progetto è necessario seguire questi step:
#### 1- Compilazione
Per compilare il programma e il database del progetto è sufficiente eseguire il seguente script:
```sh
sh compile.sh
```
#### 2- Esecuzione
Se la compilazione è andata a buon fine, per eseguire il progetto appena compilato, eseguire il seguente script:
```sh
sh run.sh
```
#### 3- Terminazione
Se si vuole terminare tutti i processi del progetto, basta eseguire il seguente comando:
```sh
sh run.sh --kill
```
