#include "Acceptor.hh"

Acceptor::Acceptor()
{
    std::cout << "Acceptor constructed!" << std::endl;
    std::cout << "Construct sockt file!" << std::endl;

    if ((server_fd_ = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "Configure server address!" << std::endl;

    server_addr_.sin_family = AF_INET;
    server_addr_.sin_addr.s_addr = INADDR_ANY;
    server_addr_.sin_port = htons(PORT);
}

void Acceptor::server_bind()
{
    // Bind the socket
    if (bind(server_fd_, (struct sockaddr *)&server_addr_, sizeof(server_addr_)) < 0) {
        perror("bind failed");
        close(server_fd_);
        exit(EXIT_FAILURE);
    }
}

void Acceptor::server_listen()
{
    // Start listening
    if (listen(server_fd_, 10) < 0) {
        perror("listen");
        close(server_fd_);
        exit(EXIT_FAILURE);
    }
}

// void Acceptor::server_accept() // tech debt