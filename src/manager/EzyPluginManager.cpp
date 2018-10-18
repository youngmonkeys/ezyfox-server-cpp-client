#include "EzyPluginManager.h"
#include "../util/EzyMaps.h"
#include "../entity/EzyPlugin.h"

EZY_NAMESPACE_START_WITH(manager)

EzyPluginManager::EzyPluginManager() {
}

EzyPluginManager::~EzyPluginManager() {
    clear();
}

void EzyPluginManager::addPlugin(entity::EzyPlugin* plugin) {
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
    return EzyMaps::getValues<int, entity::EzyPlugin>(mPluginByIds);
}

void EzyPluginManager::clear() {
    std::vector<entity::EzyPlugin*> Plugins = getPlugins();
    EZY_SAFE_DELETE_VECTOR(Plugins, entity::EzyPlugin)
    mPluginByIds.clear();
    mPluginByNames.clear();
}

EZY_NAMESPACE_END_WITH
