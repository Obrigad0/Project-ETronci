#include "main.h"

int main(){
    int num_types = 5;
    int server_port = 42071;

    // eliminato add-restock (al massimo lo rimettiamo come "restock-product")
    // eliminato view-statistic
    std::string req_types[num_types] = {"register-seller", "add-product", "update-product"};

    Server server(SERVER_ID, server_port, REDIS_SERVER, REDIS_PORT, req_types, num_types);

    server.run();
}