#ifndef MESSAGEPARSER_HH
#define MESSAGEPARSER_HH

#include <string>
#include <memory>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include "processor/HttpProcessor.hh"

class MessageParser
{
public:
    MessageParser(const std::string& root_path):root_path_(root_path) {
        http_pro_.reset(new HttpProcessor(root_path_));
    };
    void parse(int fd, char *message, SSL* ssl);
    void set_root_path(const std::string& root_path);
private:
    std::string root_path_;
    std::unique_ptr<HttpProcessor> http_pro_;
};

#endif