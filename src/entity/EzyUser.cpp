#include "EzyUser.h"

EZY_NAMESPACE_START_WITH(entity)

EzyUser::EzyUser(long identifier, std::string name) {
    this->mId = identifier;
    this->mName = name;
}

EZY_NAMESPACE_END_WITH
