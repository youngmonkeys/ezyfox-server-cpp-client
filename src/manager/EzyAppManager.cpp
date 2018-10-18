#include "EzyAppManager.h"
#include "../util/EzyMaps.h"
#include "../entity/EzyApp.h"

EZY_NAMESPACE_START_WITH(manager)

EzyAppManager::EzyAppManager() {
}

EzyAppManager::~EzyAppManager() {
    clear();
}

void EzyAppManager::addApp(entity::EzyApp* app) {
    mAppByIds[app->getId()] = app;
    mAppByNames[app->getName()] = app;
}

entity::EzyApp* EzyAppManager::getAppById(int appId) {
    return mAppByIds[appId];
}

entity::EzyApp* EzyAppManager::getAppByName(std::string appName) {
    return mAppByNames[appName];
}

std::vector<entity::EzyApp*> EzyAppManager::getApps() {
    return EzyMaps::getValues<int, entity::EzyApp>(mAppByIds);
}

void EzyAppManager::clear() {
    std::vector<entity::EzyApp*> apps = getApps();
    EZY_SAFE_DELETE_VECTOR(apps, entity::EzyApp)
    mAppByIds.clear();
    mAppByNames.clear();
}

EZY_NAMESPACE_END_WITH
