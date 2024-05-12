#ifndef main_h
#define main_h

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stddef.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/times.h>
#include <cassert>
#include <cerrno>

// librerie locali

#include "../../../service/database/dbConnection.h"
#include "../../../service/redis/redisConnection.h"
#include "../../../classes/address.h"
#include "../shared/standard.h"

// costanti

#define READ_STREAM "add-address-in"
#define WRITE_STREAM "add-address-out"

#define REDIS_SERVER "localhost"
#define REDIS_PORT 6379

#define POSTGRESQL_SERVER "localhost"
#define POSTGRESQL_PORT "5432"
#define POSTGRESQL_USER "customer"
#define POSTGRESQL_PSW "customer"
#define POSTGRESQL_DBNAME "ecommerce"

using namespace std;

#endif