#ifndef IPRICINGENGINE_H
#define IPRICINGENGINE_H

#include "ITrade.h"
#include "IScalarResultReceiver.h"

class IPricingEngine {
public:
    virtual ~IPricingEngine() = default;
    virtual void price(ITrade* trade, IScalarResultReceiver* resultReceiver) = 0;
};

#endif // IPRICINGENGINE_H

