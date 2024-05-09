#include "main.h"

int main(){
    int num_types = 4;
    int server_port = 42070
    // TO DO
    // cambiare i nomi ai metodi dei corrieri
    std::string req_types[num_types] = {"take-purchase-delivery", "update-assigned-delivery", "update-refund-request", "view-available-purchase"};

    Server server(SERVER_ID, server_port, REDIS_SERVER, REDIS_PORT, req_types, num_types);

    server.run();
}