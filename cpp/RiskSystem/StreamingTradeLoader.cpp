#include "StreamingTradeLoader.h"
#include "../Loaders/BondTradeLoader.h"
#include "../Loaders/FxTradeLoader.h"
#include "PricingConfigLoader.h"
#include <stdexcept>

#include "../Pricers/GovBondPricingEngine.h"
#include "../Pricers/CorpBondPricingEngine.h"
#include "../Pricers/FxPricingEngine.h"

std::vector<ITradeLoader*> StreamingTradeLoader::getTradeLoaders() {
    std::vector<ITradeLoader*> loaders;
    
    BondTradeLoader* bondLoader = new BondTradeLoader();
    bondLoader->setDataFile("TradeData/BondTrades.dat");
    loaders.push_back(bondLoader);
    
    FxTradeLoader* fxLoader = new FxTradeLoader();
    fxLoader->setDataFile("TradeData/FxTrades.dat");
    loaders.push_back(fxLoader);
    
    return loaders;
}

void StreamingTradeLoader::loadPricers() {
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

StreamingTradeLoader::~StreamingTradeLoader() {
    for(auto [name,engine] : pricers_){
        delete engine;
    }
    pricers_.clear();
}

void StreamingTradeLoader::loadAndPrice(IScalarResultReceiver* resultReceiver) {
    loadPricers();
    auto loaders = getTradeLoaders();
    
    for(auto curr_loader : loaders){
        
        while(true){
            auto  trade = curr_loader->loadOneByOneTrades();
            
            if (trade == nullptr) break;
            
            std::string tradeType = trade->getTradeType();
            if (pricers_.find(tradeType) == pricers_.end()) {
                resultReceiver->addError(trade->getTradeType(), "No Pricing Engines available for this trade type");
                continue;
            }
            
            IPricingEngine* pricer = pricers_[tradeType];
            pricer->price(trade, resultReceiver);
            
        }
    }
    
    
    
}
