#include "EzyAppManager.h"
#include "../util/EzyMaps.h"
#include "../entity/EzyApp.h"
#include "../logger/EzyLogger.h"

EZY_NAMESPACE_START_WITH(manager)

using namespace std;

namespace stdex {
    template<class InputIterator, class T>
    InputIterator find (InputIterator first, InputIterator last, const T& val) {
        InputIterator iter = first;
        while (iter != last) {
            if (*iter == val) {
                return iter;
            }
            ++iter;
        }
        return last;
    }
}

EzyAppManager::EzyAppManager() {
    mAppList.clear();
    mAppByIds.clear();
    mAppByNames.clear();
}

EzyAppManager::~EzyAppManager() {
    clear();
}

entity::EzyApp* EzyAppManager::removeApp(int appId) {
    auto app = mAppByIds[appId];
    if(app) {
        mAppByIds.erase(appId);
        mAppByNames.erase(app->getName());
        auto appIndex = stdex::find(begin(mAppList), end(mAppList), app);
        
        if (appIndex != mAppList.last())
            mAppList.erase(appIndex);
    }
    return app;
}

void EzyAppManager::addApp(entity::EzyApp* app) {
    mAppList.push_back(app);
    mAppByIds[app->getId()] = app;
    mAppByNames[app->getName()] = app;
}

entity::EzyApp* EzyAppManager::getApp() {
    auto app = (entity::EzyApp*)0;
    if(mAppList.size() > 0)
        app = mAppList[0];
    if(!app)
        logger::log("has no app in zone");
    return app;
}

entity::EzyApp* EzyAppManager::getAppById(int appId) {
    return mAppByIds[appId];
}

entity::EzyApp* EzyAppManager::getAppByName(std::string appName) {
    return mAppByNames[appName];
}

std::vector<entity::EzyApp*> EzyAppManager::getApps() {
    return mAppList;
}

void EzyAppManager::clear() {
    EZY_SAFE_DELETE_VECTOR(mAppList)
    mAppList.clear();
    mAppByIds.clear();
    mAppByNames.clear();
}

EZY_NAMESPACE_END_WITH
