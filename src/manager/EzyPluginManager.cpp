#include "EzyPluginManager.h"
#include "../util/EzyMaps.h"
#include "../entity/EzyPlugin.h"

EZY_NAMESPACE_START_WITH(manager)

EzyPluginManager::EzyPluginManager() {
    mPluginList.clear();
    mPluginByIds.clear();
    mPluginByNames.clear();
}

EzyPluginManager::~EzyPluginManager() {
    clear();
}

void EzyPluginManager::addPlugin(entity::EzyPlugin* plugin) {
    mPluginList.push_back(plugin);
    mPluginByIds[plugin->getId()] = plugin;
    mPluginByNames[plugin->getName()] = plugin;
}

entity::EzyPlugin* EzyPluginManager::getPluginById(int pluginId) {
    return mPluginByIds[pluginId];
}

entity::EzyPlugin* EzyPluginManager::getPluginByName(std::string pluginName) {
    return mPluginByNames[pluginName];
}

std::vector<entity::EzyPlugin*> EzyPluginManager::getPlugins() {
    return mPluginList;
}

void EzyPluginManager::clear() {
    EZY_SAFE_DELETE_VECTOR(mPluginList)
    mPluginByIds.clear();
    mPluginByNames.clear();
}

EZY_NAMESPACE_END_WITH
