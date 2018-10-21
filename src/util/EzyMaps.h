#pragma once

#include <map>
#include <vector>
#include "../EzyMacro.h"

EZY_NAMESPACE_START_WITH(util)

class EzyMaps {
private:
    EzyMaps() {}
public:
    template <class K, class V>
    static std::vector<V*> getValues(std::map<K,V*> map);
};

template <class K, class V>
std::vector<V*> EzyMaps::getValues(std::map<K,V*> map) {
    std::vector<V*> answer;
    for(typename std::map<K,V*>::iterator it = map.begin() ; it != map.end() ; ++it) {
        answer.push_back(it->second);
    }
    return answer;
}

EZY_NAMESPACE_END_WITH
