#ifndef HTTPPROCESSOR_HH_
#define HTTPPROCESSOR_HH_

#include "Processor.hh"
#include <string>
#include <openssl/ssl.h>
#include <openssl/err.h>

class HttpProcessor: public Processor
{
public:
    HttpProcessor(const std::string& root_path): root_path_(root_path) {}
    enum class ReuqestType
    {
        VISITREQUEST,
        DOWNLOADREQUEST
    };
    void process_message( // Technical debt
        ReuqestType request_type,
        const std::string& message,
        int status,
        int fd,
        const std::string& file_path,
        SSL* ssl);
    void send_response(int fd, const std::string& message, int status, SSL *ssl);
    void handle_file_download(int fd, const std::string& file_path, SSL* ssl);
private:
    std::string root_path_;
};

#endif