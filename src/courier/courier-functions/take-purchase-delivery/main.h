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

#include "../../../classes/delivery.h"
#include "../../../../utils/src/utils.h"
#include "../../../../utils/src/const.h"

// costanti

#define READ_STREAM "take-purchase-delivery-in"
#define WRITE_STREAM "take-purchase-delivery-out"

#define REDIS_SERVER "localhost"
#define REDIS_PORT 6379

#define POSTGRESQL_SERVER "localhost"
#define POSTGRESQL_PORT "5432"
#define POSTGRESQL_USER "courier"
#define POSTGRESQL_PSW "courier"
#define POSTGRESQL_DBNAME "ecommerce"

using namespace std;

#endif