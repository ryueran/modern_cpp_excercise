#ifndef ROCESSOR_HH
#define PROCESSOR_HH

class Processor
{
public:
    virtual void process_message() {};
    virtual ~Processor() {}
};

#endif