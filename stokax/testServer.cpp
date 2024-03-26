#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <map>
#include <vector>

#define PORT1 8080
#define PORT2 8081
#define PORT3 8082

int create_server_socket(int port) {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        std::cerr << "Errore nella creazione della socket" << std::endl;
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cerr << "Bind fallito sulla porta " << port << std::endl;
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        std::cerr << "Listen fallito sulla porta " << port << std::endl;
        exit(EXIT_FAILURE);
    }

    return server_fd;
}

int main() {
    int server_fd1 = create_server_socket(PORT1);
    int server_fd2 = create_server_socket(PORT2);
    int server_fd3 = create_server_socket(PORT3);

    fd_set readfds;
    int max_sd;
    std::map<int, int> client_sockets;

    while (true) {
        FD_ZERO(&readfds);

        FD_SET(server_fd1, &readfds);
        FD_SET(server_fd2, &readfds);
        FD_SET(server_fd3, &readfds);

        max_sd = std::max({server_fd1, server_fd2, server_fd3});

        for (auto const& pair : client_sockets) {
            FD_SET(pair.second, &readfds);
            max_sd = std::max(max_sd, pair.second);
        }

        int activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

        if (activity < 0) {
            std::cerr << "Errore nella select" << std::endl;
            continue;
        }

        std::vector<int> ports = {PORT1, PORT2, PORT3};
        for (int port : ports) {
            int server_fd = (port == PORT1) ? server_fd1 : (port == PORT2) ? server_fd2 : server_fd3;
            if (FD_ISSET(server_fd, &readfds)) {
                struct sockaddr_in address;
                int addrlen = sizeof(address);
                int new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
                if (new_socket < 0) {
                    std::cerr << "Accept fallito sulla porta " << port << std::endl;
                    continue;
                }
                client_sockets[port] = new_socket;
                std::cout << "Nuova connessione sulla porta " << port << std::endl;
            }
        }

        for (auto& pair : client_sockets) {
            int sd = pair.second;
            if (FD_ISSET(sd, &readfds)) {
                char buffer[1024] = {0};
                int valread = read(sd, buffer, 1024);
                if (valread == 0) {
                    close(sd);
                    client_sockets.erase(pair.first);
                    std::cout << "Client disconnesso sulla porta " << pair.first << std::endl;
                } else {
                    std::cout << "Messaggio ricevuto sulla porta " << pair.first << ": " << buffer << std::endl;
                    // Gestire il messaggio ricevuto
                }
            }
        }
    }

    close(server_fd1);
    close(server_fd2);
    close(server_fd3);

    return 0;
}