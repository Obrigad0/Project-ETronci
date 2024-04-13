#include "handler.h"

Handler::Handler(const char* redis_ip, int redis_port, std::string client_requests[], int req_num){
    // ho inserito client_requests al posto di req_types e req_num al posto di num_req_types; così sono uguali al server
    
    c2r = redisConnect(redis_ip, redis_port); // redConn oppure conn2redis al posto di c2r ?
    types = client_requests;
    num_types = req_num;

    init_streams();
}

bool Handler::send_to_managers(int client_id, std::string msg){ // TO DO

}

bool Handler::read_from_managers(std::string* out_str_ptr, int* client_id_ptr){ // TO DO

}

void Handler::init_streams(){  // TO DO

}