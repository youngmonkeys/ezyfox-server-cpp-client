#ifndef EzyZone_h
#define EzyZone_h

#include <string>
#include "../EzyMacro.h"

EZY_NAMESPACE_START
class EzyClient;
EZY_NAMESPACE_END

EZY_NAMESPACE_START_WITH_ONLY(manager)
class EzyAppManager;
class EzyPluginManager;
EZY_NAMESPACE_END_WITH

EZY_NAMESPACE_START_WITH(entity)

class EzyZone {
protected:
    EZY_SYNTHESIZE_READONLY(int, Id);
    EZY_SYNTHESIZE_READONLY(std::string, Name);
    EZY_SYNTHESIZE_READONLY(EzyClient*, Client);
    EZY_SYNTHESIZE_READONLY(manager::EzyAppManager*, AppManager);
    EZY_SYNTHESIZE_READONLY(manager::EzyPluginManager*, PluginManager);
public:
    EzyZone(EzyClient* client, int identifier, std::string name);
    ~EzyZone();
};

EZY_NAMESPACE_END_WITH

#endif /* EzyZone_h */
