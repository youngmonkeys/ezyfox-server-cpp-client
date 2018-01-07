#include "EzyArrayRequestSerializer.h"

EZY_NAMESPACE_START
namespace request {
  
entity::EzyArray* EzyArrayRequestSerializer::serialize(EzyRequest *request) {
    auto array = new entity::EzyArray();
    auto command = request->getCommand();
    auto params = request->getParams()->serialize();
    array->addInt(command);
    array->addItem(params);
    return array;
}
    
};
EZY_NAMESPACE_END
