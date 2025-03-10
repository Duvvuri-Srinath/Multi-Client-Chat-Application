#include <iostream>
#include <vector>
#include <map>
#include <thread>
#include <mutex>
#include <cstring>
#include <unistd.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define PORT 8080
#define MAX_EVENTS 10
#define BUFFER_SIZE 1024

std::mutex clients_mutex;
std::map<int, std::string> clients;

void set_nonblocking(int sock) {
    int flags = fcntl(sock, F_GETFL, 0);
    fcntl(sock, F_SETFL, flags | O_NONBLOCK);
}

void broadcast_message(const std::string &message, int sender_fd) {
    std::lock_guard<std::mutex> lock(clients_mutex);
    for (auto &[fd, name] : clients) {
        if (fd != sender_fd) {
            send(fd, message.c_str(), message.size(), 0);
        }
    }
}

void handle_client(int client_fd) {
    char buffer[BUFFER_SIZE];
    while (true) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_fd, buffer, BUFFER_SIZE, 0);
        if (bytes_received <= 0) {
            std::lock_guard<std::mutex> lock(clients_mutex);
            std::cout << "Client " << clients[client_fd] << " disconnected\n";
            clients.erase(client_fd);
            close(client_fd);
            return;
        }
        std::string message = clients[client_fd] + ": " + buffer;
        std::cout << message;
        broadcast_message(message, client_fd);
    }
}

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket failed");
        return EXIT_FAILURE;
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        return EXIT_FAILURE;
    }

    if (listen(server_fd, SOMAXCONN) < 0) {
        perror("Listen failed");
        return EXIT_FAILURE;
    }

    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("epoll_create1 failed");
        return EXIT_FAILURE;
    }

    epoll_event event{};
    event.data.fd = server_fd;
    event.events = EPOLLIN;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &event);

    std::cout << "Server started on port " << PORT << std::endl;

    epoll_event events[MAX_EVENTS];

    while (true) {
        int event_count = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        for (int i = 0; i < event_count; i++) {
            if (events[i].data.fd == server_fd) {
                sockaddr_in client_addr{};
                socklen_t client_len = sizeof(client_addr);
                int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
                if (client_fd < 0) continue;

                set_nonblocking(client_fd);
                epoll_event client_event{};
                client_event.data.fd = client_fd;
                client_event.events = EPOLLIN;
                epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &client_event);

                std::string welcome_message = "Enter your name: ";
                send(client_fd, welcome_message.c_str(), welcome_message.size(), 0);

                char name[BUFFER_SIZE];
                recv(client_fd, name, BUFFER_SIZE, 0);
                {
                    std::lock_guard<std::mutex> lock(clients_mutex);
                    clients[client_fd] = name;
                }

                std::thread(handle_client, client_fd).detach();
            }
        }
    }

    close(server_fd);
    return 0;
}
