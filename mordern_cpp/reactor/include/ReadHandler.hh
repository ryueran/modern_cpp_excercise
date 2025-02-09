#ifndef READHANDLER_HH_
#define READHANDLER_HH_

#include "Handler.hh"

class ReadHandler: public Handler
{
public:
    ReadHandler (int fd): Handler(fd) {
        event_type_ = EventType::Read;
    }
};

#endif