#define standard_h

// MODIFICARE QUESTI VALORI PER CAMBIARE I RISULTATI DESIDERATI
#define QUERYSIZE 1500
#define RESPONSESIZE 200
#define PRMTRSIZE 400
#define KEYSIZE 30
#define VALUESIZE 30
#define MSGIDSIZE 30

#define REDISSPACE "##"
#define BLANKSPACE " "


#include <string>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <sstream>

/* Local libraries */

#include "../service/redis/redisConnection.h"

/* Funzioni standard universali */

void send_response_status(redisContext* c2r, const char *stream, const char *client_id, const char *resp_status, const char *msg_id, const int nrows);

std::string replace_substring(std::string input, const std::string& target, const std::string& replacement);

std::string get_current_timestamp_as_string();


