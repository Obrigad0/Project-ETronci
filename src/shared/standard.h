#ifndef standard_h
#define standard_h

// MODIFICARE QUESTI VALORI PER CAMBIARE I RISULTATI DESIDERATI
//TO DO!!
#define QUERYSIZE 1500 //QUERY_LEN
#define RESPONSESIZE 200 // RESPONSE_LEN
#define PRMTRSIZE 400 //PARAMETER_LEN
#define KEYSIZE 30 //KEY_LEN
#define VALUESIZE 30 //VALUE_LEN
#define MSGIDSIZE 30 //MESSAGE_ID_LEN

#define REDISSPACE "##" //SPACE_REDIS_STRING
#define BLANKSPACE " " //SPACE


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


