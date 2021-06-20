#ifndef EzySender_h
#define EzySender_h

#include "../EzyMacro.h"
#include "../request/EzyRequest.h"

EZY_NAMESPACE_START_WITH(socket)
 
class EzySender {
public:
    virtual void send(request::EzyRequest* request, bool encrypted) = 0;
};

EZY_NAMESPACE_END_WITH

#endif /* EzySender_h */
