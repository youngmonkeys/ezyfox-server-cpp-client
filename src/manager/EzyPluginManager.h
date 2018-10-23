#pragma once

#include <map>
#include <vector>
#include <string>
#include "../EzyMacro.h"

EZY_NAMESPACE_START_WITH_ONLY(entity)
class EzyPlugin;
EZY_NAMESPACE_END_WITH

EZY_NAMESPACE_START_WITH(manager)

class EzyPluginManager {
protected:
    std::vector<entity::EzyPlugin*> mPluginList;
    std::map<int, entity::EzyPlugin*> mPluginByIds;
    std::map<std::string, entity::EzyPlugin*> mPluginByNames;
public:
    EzyPluginManager();
    ~EzyPluginManager();
    void clear();
    void addPlugin(entity::EzyPlugin* plugin);
    entity::EzyPlugin* getPluginById(int PluginId);
    entity::EzyPlugin* getPluginByName(std::string pluginName);
    std::vector<entity::EzyPlugin*> getPlugins();
};

EZY_NAMESPACE_END_WITH
