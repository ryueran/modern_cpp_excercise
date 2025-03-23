#include <openssl/ssl.h>
#include <openssl/err.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <errno.h>
#include <fcntl.h>

int main() {
    // 初始化 OpenSSL
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();

    // 创建 SSL 上下文
    SSL_CTX* ctx = SSL_CTX_new(TLS_server_method());
    if (!ctx) {
        fprintf(stderr, "SSL_CTX_new 失败\n");
        return 1;
    }

    // 加载证书和私钥
    if (SSL_CTX_use_certificate_file(ctx, "/home/muzi/Documents/c++_gateway/ca_certificate/ca.crt", SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        return 1;
    }
    if (SSL_CTX_use_PrivateKey_file(ctx, "/home/muzi/Documents/c++_gateway/ca_certificate/ca.key", SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        return 1;
    }
    if (!SSL_CTX_check_private_key(ctx)) {
        fprintf(stderr, "证书与私钥不匹配\n");
        return 1;
    }

    // 配置协议和加密套件
    SSL_CTX_set_min_proto_version(ctx, TLS1_2_VERSION);
    SSL_CTX_set_max_proto_version(ctx, TLS1_3_VERSION);
    SSL_CTX_set_cipher_list(ctx, "HIGH:!aNULL:!eNULL:!EXPORT:!DES:!RC4:!MD5:!PSK:!SRP");

    // 创建套接字并绑定端口
    int sockfd = socket(AF_INET6, SOCK_STREAM, 0);
    struct sockaddr_in6 addr = {0};
    addr.sin6_family = AF_INET6;
    addr.sin6_port = htons(8443);
    addr.sin6_addr = in6addr_any;
    bind(sockfd, (struct sockaddr*)&addr, sizeof(addr));
    listen(sockfd, 10);

    while (1) {
        // 接受连接
        int client_sock = accept(sockfd, NULL, NULL);
        SSL* ssl = SSL_new(ctx);
        SSL_set_fd(ssl, client_sock);

        // 处理 SSL 握手
        int ret;
        do {
            ret = SSL_accept(ssl);
            if (ret <= 0) {
                int err = SSL_get_error(ssl, ret);
                if (err == SSL_ERROR_WANT_READ || err == SSL_ERROR_WANT_WRITE) {
                    // 等待套接字可读/可写（需实现 select/poll）
                    continue;
                } else {
                    ERR_print_errors_fp(stderr);
                    break;
                }
            }
        } while (ret <= 0);

        // 发送响应
        const char* response = "HTTP/1.1 200 OK\r\nContent-Length: 12\r\n\r\nHello World!";
        SSL_write(ssl, response, strlen(response));
        SSL_shutdown(ssl);
        SSL_free(ssl);
        close(client_sock);
    }

    SSL_CTX_free(ctx);
    return 0;
}