#ifndef handler_h
#define handler_h

/* System libraries */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/times.h>
#include <cassert>
#include <cerrno>
#include <iostream>

/* Local libraries */

 // #include "../../lib/con2redis/src/con2redis.h" TO DO 
// #include "../../lib/con2db/pgsql.h" TO DO 
// #include "../../utils/src/const.h" TO DO 

/* Classes */
class Handler{
    public:
        Handler(const char* redis_ip, int redis_port, std::string client_requests[], int req_num);

        bool send_to_managers(int client_id, std::string cmd);
        bool read_from_managers(std::string* out_str_ptr, int* client_id_ptr);

    private:
        // redisContext *c2r; TO DO 
        std::string* types;
        int num_types;
        
        void init_streams();
};

#endif