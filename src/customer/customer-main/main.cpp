#include "main.h"

int main(){
    int num_types = 10;
    int server_port = 42069
    // cambiato add-rating in review-order. eliminato add-card
    // cambiato view-order al plurale. cambiato add-customer in register-customer
    // cambiato add-purchase in make-order
    std::string req_types[num_types] = {"register-customer", "search-products", "make-order", "view-orders", "review-order"};

    Server server(SERVER_ID, server_port, REDIS_SERVER, REDIS_PORT, req_types, num_types);

    server.run();
}