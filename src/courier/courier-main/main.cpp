#include "main.h"

int main(){
    int num_types = 4;
    int server_port = 42070

    std::string req_types[num_types] = {"register-courier", "take-delivery", "update-delivery-status", "view-pending-orders"};

    Server server(SERVER_ID, server_port, REDIS_SERVER, REDIS_PORT, req_types, num_types);

    server.run();
}