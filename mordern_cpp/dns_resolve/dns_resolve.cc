#include <iostream>
#include <netdb.h>
#include <arpa/inet.h>
#include <cstring>

void resolve_host(const char* hostname) {
    struct addrinfo hints, *res, *p;
    char ipstr[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC; // 允许 IPv4 或 IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP 连接

    int status = getaddrinfo(hostname, nullptr, &hints, &res);
    if (status != 0) {
        std::cerr << "getaddrinfo error: " << gai_strerror(status) << std::endl;
        return;
    }

    std::cout << "Resolved addresses for " << hostname << ":\n";
    for (p = res; p != nullptr; p = p->ai_next) {
        void* addr;
        if (p->ai_family == AF_INET) { // IPv4
            struct sockaddr_in* ipv4 = (struct sockaddr_in*)p->ai_addr;
            addr = &(ipv4->sin_addr);
        } else { // IPv6
            struct sockaddr_in6* ipv6 = (struct sockaddr_in6*)p->ai_addr;
            addr = &(ipv6->sin6_addr);
        }

        inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
        std::cout << "  " << ipstr << std::endl;
    }

    freeaddrinfo(res);
}

int main() {
    resolve_host("www.youtub.com");
    return 0;
}
