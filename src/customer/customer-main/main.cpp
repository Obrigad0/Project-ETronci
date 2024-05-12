#include "main.h"

int main(){
    int num_types = 10;
    int server_port = 42069

    // cambiato add-rating in review-order
    // cambiato view-order al plurale
    std::string req_types[num_types] = {"add-customer", "add-address", "search-products", "add-card", "add-purchase", "review-order", "view-orders"};

    Server server(SERVER_ID, server_port, REDIS_SERVER, REDIS_PORT, req_types, num_types);

    server.run();
}