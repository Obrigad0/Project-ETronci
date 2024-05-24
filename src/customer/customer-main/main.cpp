#include "main.h"

int main(){
    int num_types = 5;
    int server_port = 42069;

    std::string req_types[5] = {"register-customer", "search-products", "make-order", "check-order", "review-order"};

    Server server(SERVER_ID, server_port, REDIS_SERVER, REDIS_PORT, req_types, num_types);

    server.run();
}