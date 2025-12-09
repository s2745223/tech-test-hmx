#ifndef PRICINGCONFIGLOADER_H
#define PRICINGCONFIGLOADER_H

#include "PricingEngineConfig.h"
#include <string>

class PricingConfigLoader {
private:
    std::string configFile_;
    PricingEngineConfig parseXml(const std::string& content);
    
public:
    std::string getConfigFile() const;
    void setConfigFile(const std::string& file);
    PricingEngineConfig loadConfig();
};

#endif // PRICINGCONFIGLOADER_H
