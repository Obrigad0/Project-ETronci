#include "redisConnection.h"

// Funzione per leggere il numero di stream da una risposta Redis
int ReadNumStreams(redisReply *r) {
    // Ritorna il numero di elementi nella risposta
    return (r->elements);
}

// Funzione per leggere il nome di uno stream da una risposta Redis
int ReadStreamName(redisReply *r, char *streamname, long unsigned int k) {
    // Copia il nome dello stream nella variabile streamname
    strcpy(streamname, r->element[k]->element[0]->str);
    return(0);
}

// Funzione per leggere il numero di messaggi in uno stream
int ReadStreamNumMsg(redisReply *r, long unsigned int streamnum) {
    // Ritorna il numero di elementi nel messaggio specificato
    return(r->element[streamnum]->element[1]->elements);
}

// Funzione per leggere l'ID di un messaggio in uno stream
int ReadStreamNumMsgID(redisReply *r, long unsigned int streamnum, int msgnum, char *msgid) {
    // Copia l'ID del messaggio nella variabile msgid
    strcpy(msgid, r->element[streamnum]->element[1]->element[msgnum]->element[0]->str);
    return(0);
}

// Funzione per leggere il numero di valori in un messaggio di uno stream
int ReadStreamMsgNumVal(redisReply *r, long unsigned int streamnum, int msgnum) {
    // Ritorna il numero di elementi nel messaggio specificato
    return(r->element[streamnum]->element[1]->element[msgnum]->element[1]->elements);
}

// Funzione per leggere un valore specifico in un messaggio di uno stream
int ReadStreamMsgVal(redisReply *r, long unsigned int streamnum, int msgnum, int entry, char *value) {
    // Copia il valore specificato nella variabile value
    strcpy(value, r->element[streamnum]->element[1]->element[msgnum]->element[1]->element[entry]->str);
    return(0);
}

// Funzione per stampare i tipi di risposta Redis
void print_reply_types() {
    printf("REDIS_REPLY_STRING=%d,\
REDIS_REPLY_STATUS=%d,\
REDIS_REPLY_INTEGER=%d,\
REDIS_REPLY_NIL=%d,\
REDIS_REPLY_ERROR=%d,\
REDIS_REPLY_ARRAY=%d\n",
           REDIS_REPLY_STRING,
           REDIS_REPLY_STATUS,
           REDIS_REPLY_INTEGER,
           REDIS_REPLY_NIL,
           REDIS_REPLY_ERROR,
           REDIS_REPLY_ARRAY
    );
}

// Funzione per verificare il tipo di risposta Redis
void assertReplyType(redisContext *c, redisReply *r, int type) {
    if (r == NULL)
        dbg_abort("NULL redisReply (error: %s)", c->errstr);
    if (r->type != type) {
        print_reply_types();
        dbg_abort("Expected reply type %d but got type %d", type, r->type);
    }
}

// Funzione per verificare la validità della risposta Redis
void assertReply(redisContext *c, redisReply *r) {
    if (r == NULL)
        dbg_abort("NULL redisReply (error: %s)", c->errstr);
}

// Funzione per stampare il contenuto di una risposta Redis
void dumpReply(redisReply *r, int indent) {
    sds buffer = sdsempty();

    printf("%d", r->type);
    switch (r->type) {
        case REDIS_REPLY_STRING:
            buffer = sdscatfmt(buffer, "(string) %s\n", r->str);
            break;
        case REDIS_REPLY_STATUS:
            buffer = sdscatfmt(buffer, "(status) %s\n", r->str);
            break;
        case REDIS_REPLY_INTEGER:
            buffer = sdscatlen(buffer, "(integer) %lld\n", r->integer);
            break;
        case REDIS_REPLY_NIL:
            buffer = sdscatlen(buffer, "(nill)\n", 7);
            break;
        case REDIS_REPLY_ERROR:
            buffer = sdscatfmt(buffer, "(error) %s", r->str);
            break;
        case REDIS_REPLY_ARRAY:
            for (size_t i = 0; i < r->elements; i++) {
                dumpReply(r->element[i], indent + 2);
            }
            break;
        default:
            dbg_abort("Don't know how to deal with reply type %d", r->type);
    }

    if (sdslen(buffer) > 0) {
        for (int i = 0; i < indent; i++)
            printf(" ");

        printf("%s", buffer);
    }

    sdsfree(buffer);
}

// Funzione per inizializzare uno stream Redis
void initStreams(redisContext *c, const char *stream) {
    redisReply *r = RedisCommand(c, "XGROUP CREATE %s main $ MKSTREAM", stream);
    assertReply(c, r);
    freeReplyObject(r);
}
