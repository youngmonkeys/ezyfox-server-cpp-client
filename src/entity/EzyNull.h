//
//  EzyNull.h
//  ezyfox-ssl
//
//  Created by Dzung on 04/06/2021.
//

#ifndef EzyNull_h
#define EzyNull_h

#include "EzyValue.h"
#include "../EzyMacro.h"

EZY_NAMESPACE_START_WITH(entity)

class EzyNull : public EzyValue {
#ifdef EZY_DEBUG
    virtual void printToOutStream(std::ostringstream& stream, int padding) override;
#endif
    
public:
    EzyNull();
    virtual ~EzyNull();
    virtual void writeToBuffer(codec::EzyDataEncoder* writer) override;
public:
    std::string toString() const override;
};

EZY_NAMESPACE_END_WITH

#endif /* EzyNull_h */
