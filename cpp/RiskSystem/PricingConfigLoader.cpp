#include "PricingConfigLoader.h"
#include <stdexcept>

std::string PricingConfigLoader::getConfigFile() const {
    return configFile_;
}

void PricingConfigLoader::setConfigFile(const std::string& file) {
    configFile_ = file;
}

PricingEngineConfig PricingConfigLoader::loadConfig() {
    throw std::runtime_error("Not implemented");
}
