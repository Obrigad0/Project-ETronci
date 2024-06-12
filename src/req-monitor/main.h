#ifndef req_monitor_h
#define req_monitor_h

#include <cstring> 
#include <iostream> 
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h> 
#include <cerrno>
#include <vector>
#include <sstream>

#include "../service/database/dbConnection.h"
#include "../shared/standard.h"

#define MAX_CONNECTION_TIME 60000.0
#define MAX_RESPONSE_TIME_AVG 30000.0

#define POSTGRESQL_SERVER "localhost"
#define POSTGRESQL_PORT "5432"
#define POSTGRESQL_USER "admin"
#define POSTGRESQL_PSW "admin"
#define POSTGRESQL_DBNAME "logdb"

#endif