#ifndef PRICINGENGINECONFIG_H
#define PRICINGENGINECONFIG_H

#include "PricingEngineConfigItem.h"
#include <vector>

class PricingEngineConfig : public std::vector<PricingEngineConfigItem> {
public:
    PricingEngineConfig() = default;
};

#endif // PRICINGENGINECONFIG_H

