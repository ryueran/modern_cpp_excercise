#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/listener.h>
#include <arpa/inet.h>
#include <iostream>
#include <cstring>
#include <unistd.h>

#define PORT 8888

void read_cb(struct bufferevent *bev, void *ctx)
{
    char buffer[1024];
    memset(buffer, 0 ,sizeof(buffer));

    int len = bufferevent_read(bev, buffer, sizeof(buffer) - 1);
    if(len > 0)
    {
        std::cout << "Received:" << buffer << std::endl;
        bufferevent_write(bev, buffer, len);
    }
}

void event_cb(struct bufferevent *bev, short events, void *ctx)
{
    if(events & (BEV_EVENT_EOF | BEV_EVENT_ERROR))
    {
        std::cout << "CLient disconnected." << std::endl;
        bufferevent_free(bev);
    }
}

void accept_cb(
    struct evconnlistener *listner,
    evutil_socket_t fd,
    struct sockaddr *addr,
    int socklen,
    void *ctx)
{
    std::cout << "New client connected!" << std::endl;
    
}

