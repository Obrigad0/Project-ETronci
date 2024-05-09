#include "handler.h"

Handler::Handler(const char* redip, int redport, std::string client_requests[], int req_num){

    // c2r = redisConnect(redip, redport); // redConn oppure conn2redis al posto di c2r ?
    types = client_requests;
    num_types = req_num;
    startStream();

}

bool Handler::sendToFunctions(int idClient, std::string msg){ // TO DO
    // redisReply* reply;    // TO DO
    bool isValid; // Variabile booleana per indicare se il tipo di richiesta è valido
    int i;

    // Ottieni il tipo di richiesta
    for (i = 0; i < msg.length(); i++) {
        if (msg[i] == 32)  break;   // 32 è il carattere Spazio in ASCII
    }

    if (i >= msg.length()) {
        std::cout << "\nMessaggio NON VALIDO" << std::endl;
        return false;
    }

    std::string req_type = msg.substr(0, i); // Estrae il tipo di richiesta dalla stringa del messaggio
    std::string req_cmd = msg.substr(i); // Estrae il comando della richiesta dalla stringa del messaggio

    isValid = false;
    for (i = 0; i < num_types; i++) {
        if (req_type.compare(types[i]) == 0) {
            isValid = true;
            break;
        }
    }

    if (!isValid) {
        // Tipo di richiesta non valido
        std::cout << "\nTipo di richiesta non valido" << std::endl;
        return false;
    }

    // Invia il comando sullo stream -in del gestore corrispondente
    std::string redis_cmd = "XADD " + req_type + "-in * client_id " + std::to_string(idClient) + " " + req_cmd;

    std::cout << "\n" << redis_cmd << std::endl;

    // reply = RedisCommand(c2r, redis_cmd.c_str());  // TO DO
    // assertReply(c2r, reply);  // TO DO
    return true;

}

bool Handler::readFromFunctions(std::string* outstr, int* idClient){ // TO DO

    // redisReply* reply;
    char msg_id[MESSAGE_ID_LEN], tmp_buffer[30], client_id[VALUE_LEN], resp_status[30], num_rows[30], row[30];
    int i, j, num_rows_int, curr_row, row_columns;
    std::string tmp_str;
    std::string out_str;

    for (i = 0; i < num_types; i++) {
        // reply = RedisCommand(c2r, "XREADGROUP GROUP main handler COUNT 1 STREAMS %s-out >", types[i].c_str());
        assertReply(c2r, reply);
        if (ReadNumStreams(reply) == 0)     // Se vuoto, controlla il prossimo gestore
            continue;

        // Ottieni l'ID del client
        ReadStreamMsgVal(reply, 0, 0, 0, tmp_buffer);
        ReadStreamMsgVal(reply, 0, 0, 1, client_id);

        if (strcmp(tmp_buffer, "client_id"))
            continue;   // Ignora la risposta non valida

        *idClient = strtol(client_id, NULL, 10);

        // Ottieni il numero di righe
        ReadStreamMsgVal(reply, 0, 0, 4, tmp_buffer);
        ReadStreamMsgVal(reply, 0, 0, 5, num_rows);

        if (strcmp(tmp_buffer, "num_rows"))
            continue;   // Ignora la risposta non valida

        num_rows_int = strtol(num_rows, NULL, 10);

        // Ottieni lo stato della risposta
        ReadStreamMsgVal(reply, 0, 0, 2, tmp_buffer);
        ReadStreamMsgVal(reply, 0, 0, 3, resp_status);

        if (strcmp(tmp_buffer, "resp_status"))
            continue;   // Ignora la risposta non valida

        if (strcmp(resp_status, "REQUEST_SUCCESS"))
            num_rows_int = 0;   // "Brucia" le possibili tuple

        freeReplyObject(reply);

        curr_row = 0;

        // Aggiungi lo stato della risposta
        tmp_str = resp_status;
        out_str = tmp_str + "\n";

        while (curr_row < num_rows_int - 1) {
            reply = RedisCommand(c2r, "XREADGROUP GROUP main handler BLOCK 0 COUNT 1 STREAMS %s-out >", types[i].c_str());

            assertReply(c2r, reply);
            if (ReadNumStreams(reply) == 0) {
                out_str = "BAD_RESPONSE";
                break;
            }

            // Controlla se la prima coppia chiave/valore è il num_rows
            ReadStreamMsgVal(reply, 0, 0, 0, tmp_buffer);    // Indice del primo campo del messaggio = 0
            ReadStreamMsgVal(reply, 0, 0, 1, row);

            if (strcmp(tmp_buffer, "row")) {
                out_str = "BAD_TUPLES";
                break;
            }

            curr_row = strtol(row, NULL, 10);
            row_columns = ReadStreamMsgNumVal(reply, 0, 0);

            out_str = out_str + "{";

            for (j = 2; j < row_columns; j += 2) {
                ReadStreamMsgVal(reply, 0, 0, j, tmp_buffer);
                tmp_str = tmp_buffer;
                out_str = out_str + tmp_str + ": ";

                ReadStreamMsgVal(reply, 0, 0, j + 1, tmp_buffer);
                tmp_str = tmp_buffer;
                out_str = out_str + tmp_str + ", ";
            }

            out_str = out_str + "}\n";
        }

        *outstr = out_str;

        return true;
    }

    return false;

}

void Handler::startStream() {

    redisReply* reply; // Puntatore a una struttura redisReply per memorizzare la risposta del server Redis
    std::string read; // Stringa per memorizzare il nome dello stream di lettura
    std::string write; // Stringa per memorizzare il nome dello stream di scrittura

    for (int i = 0; i < num_types; i++) { // Ciclo for che itera su tutti i tipi definiti in types[]

        read = types[i] + "-in"; // Costruisce il nome dello stream di lettura aggiungendo "-in" al tipo corrente
        write = types[i] + "-out"; // Costruisce il nome dello stream di scrittura aggiungendo "-out" al tipo corrente

        reply = RedisCommand(c2r, "DEL %s", read.c_str()); // Esegue il comando Redis "DEL" per eliminare lo stream di lettura
        assertReply(c2r, reply); // Verifica la risposta del server Redis per assicurarsi che il comando sia stato eseguito correttamente

        reply = RedisCommand(c2r, "DEL %s", write.c_str()); // Esegue il comando Redis "DEL" per eliminare lo stream di scrittura
        assertReply(c2r, reply); // Verifica la risposta del server Redis per assicurarsi che il comando sia stato eseguito correttamente

        initStreams(c2r, read.c_str()); // Inizializza lo stream di lettura chiamando la funzione initStreams con il nome dello stream di lettura
        initStreams(c2r, write.c_str()); // Inizializza lo stream di scrittura chiamando la funzione initStreams con il nome dello stream di scrittura
    }
}
