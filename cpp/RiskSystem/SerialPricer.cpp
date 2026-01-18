#include "SerialPricer.h"
#include "../Pricers/GovBondPricingEngine.h"
#include "../Pricers/CorpBondPricingEngine.h"
#include "../Pricers/FxPricingEngine.h"
#include <stdexcept>

SerialPricer::~SerialPricer() {
    for(auto [name,engine] : pricers_){
        delete engine;
    }
    pricers_.clear();
}

void SerialPricer::loadPricers() {
    PricingConfigLoader pricingConfigLoader;
    pricingConfigLoader.setConfigFile("./PricingConfig/PricingEngines.xml");
    PricingEngineConfig pricerConfig = pricingConfigLoader.loadConfig();
    
    for (const auto& item : pricerConfig) {
        const std::string tradeType = item.getTradeType();
        

        IPricingEngine* engine = nullptr;

        
        if (tradeType == "GovBond") {
            engine = new GovBondPricingEngine();
        } else if (tradeType == "CorpBond") {
            engine = new CorpBondPricingEngine();
        } else if ((tradeType == "FxSpot") || (tradeType == "FxFwd")) {
            engine = new FxPricingEngine();
        } else {
            throw std::runtime_error("Unknown pricing for tradeType: " + tradeType);
        }

        
        pricers_[tradeType] = engine;
    }

}

void SerialPricer::price(const std::vector<std::vector<ITrade*>>& tradeContainers, 
                         IScalarResultReceiver* resultReceiver) {
    loadPricers();
    
    for (const auto& tradeContainer : tradeContainers) {
        for (ITrade* trade : tradeContainer) {
            std::string tradeType = trade->getTradeType();
            if (pricers_.find(tradeType) == pricers_.end()) {
                resultReceiver->addError(trade->getTradeId(), "No Pricing Engines available for this trade type");
                continue;
            }
            
            IPricingEngine* pricer = pricers_[tradeType];
            pricer->price(trade, resultReceiver);
        }
    }
}
