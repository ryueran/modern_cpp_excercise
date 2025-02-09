#ifndef WRITEHANDLER_HH_
#define WRITEHANDLER_HH_

#include "Handler.hh"

class WriteHandler: public Handler
{
public:
    WriteHandler (int fd): Handler(fd) {
        event_type_ = EventType::Read;
    }
};

#endif