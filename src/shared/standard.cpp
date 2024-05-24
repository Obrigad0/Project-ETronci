#include "standard.h"

// Funzione per inviare lo stato della risposta a un contesto Redis
void send_response_status(redisContext* c2r, const char *stream, const char *client_id, const char *resp_status, const char *msg_id, const int nrows) {
    // Dichiarazione di un puntatore a redisReply
    redisReply *reply;

    // Stampa della risposta sullo standard output
    printf("Response: %s %s %s %s %d\n", stream, client_id, resp_status, msg_id, nrows);

    // Esecuzione del comando Redis XADD per aggiungere un messaggio a uno stream
    reply = RedisCommand(c2r, "XADD %s * client_id %s resp_status %s num_rows %d", stream, client_id, resp_status, nrows);

    // Verifica che il tipo di risposta sia una stringa
    assertReplyType(c2r, reply, REDIS_REPLY_STRING);

    // Liberazione della memoria allocata per la risposta
    freeReplyObject(reply);
}

// Funzione per sostituire tutte le occorrenze di una sottostringa in una stringa
std::string replace_substring(std::string input, const std::string& target, const std::string& replacement) {
    // Trova la posizione della prima occorrenza della sottostringa target
    size_t pos = input.find(target);

    // Continua a sostituire finché ci sono occorrenze della sottostringa target
    while (pos != std::string::npos) {
        // Sostituisce la sottostringa target con la sottostringa replacement
        input.replace(pos, target.length(), replacement);

        // Trova la posizione della prossima occorrenza della sottostringa target
        pos = input.find(target, pos + replacement.length());
    }

    // Ritorna la stringa modificata
    return input;
}

// Funzione per ottenere il timestamp corrente come stringa
std::string get_current_timestamp_as_string() {
    // Ottiene il tempo corrente dal sistema
    auto now = std::chrono::system_clock::now();

    // Converte il tempo corrente in un tipo time_t
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

    // Converte il tempo corrente in un formato locale
    std::tm *localTime = std::localtime(&currentTime);

    // Crea un oggetto stringstream per formattare il timestamp
    std::ostringstream oss;
    oss << std::put_time(localTime, "%Y-%m-%d %H:%M:%S");

    // Ritorna il timestamp formattato come stringa
    return oss.str();
}
