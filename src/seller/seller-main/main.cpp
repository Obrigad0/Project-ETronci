#include "main.h"

int main(){
    int num_types = 3;
    int server_port = 42071;

    std::string req_types[3] = {"register-seller", "add-product", "update-product"};

    Server server(SERVER_ID, server_port, REDIS_SERVER, REDIS_PORT, req_types, num_types);

    server.run();
}