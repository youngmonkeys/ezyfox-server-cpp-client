#pragma once

#include <map>
#include <vector>
#include <string>
#include "../EzyMacro.h"

EZY_NAMESPACE_START_WITH_ONLY(entity)
class EzyApp;
EZY_NAMESPACE_END_WITH

EZY_NAMESPACE_START_WITH(manager)

class EzyAppManager {
protected:
    std::map<int, entity::EzyApp*> mAppByIds;
    std::map<std::string, entity::EzyApp*> mAppByNames;
public:
    EzyAppManager();
    ~EzyAppManager();
    void clear();
    void addApp(entity::EzyApp* app);
    entity::EzyApp* getApp();
    std::vector<entity::EzyApp*> getApps();
    entity::EzyApp* getAppById(int appId);
    entity::EzyApp* getAppByName(std::string appName);
};

EZY_NAMESPACE_END_WITH
