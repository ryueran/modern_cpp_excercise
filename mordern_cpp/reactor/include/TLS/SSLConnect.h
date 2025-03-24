#ifndef SSLCONTEXT_H
#define SSLCONTEXT_H

#include <iostream>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <unistd.h>

#define SERVER_CERT "/home/muzi/Documents/c++_gateway/ca_certificate/ca.crt"
#define SERVER_KEY "/home/muzi/Documents/c++_gateway/ca_certificate/ca.key"

static void init_openssl()
{
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
}

static SSL_CTX* create_context() {
    SSL_CTX* ctx = SSL_CTX_new(TLS_server_method());
    if(!ctx) {
        std::cerr << "Unable to crate SSL context\n";
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
    return ctx;
}

static void configure_context(SSL_CTX *ctx)
{
    if (SSL_CTX_use_certificate_file(ctx, SERVER_CERT, SSL_FILETYPE_PEM) <= 0 ||
        SSL_CTX_use_PrivateKey_file(ctx, SERVER_KEY, SSL_FILETYPE_PEM) <= 0) {
        std::cerr << "Failed to load certificate or private key\n";
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }

    if (!SSL_CTX_check_private_key(ctx)) {
        std::cerr << "Failed to pairing certficate and private key\n";
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }

    // 配置协议和加密套件
    SSL_CTX_set_min_proto_version(ctx, TLS1_2_VERSION);
    SSL_CTX_set_max_proto_version(ctx, TLS1_3_VERSION);
    SSL_CTX_set_cipher_list(ctx, "HIGH:!aNULL:!eNULL:!EXPORT:!DES:!RC4:!MD5:!PSK:!SRP");
}

#endif