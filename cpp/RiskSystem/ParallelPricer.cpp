#include "ParallelPricer.h"
#include "../Pricers/GovBondPricingEngine.h"
#include "../Pricers/CorpBondPricingEngine.h"
#include "../Pricers/FxPricingEngine.h"
#include <stdexcept>

ParallelPricer::~ParallelPricer() {

}

void ParallelPricer::loadPricers() {
    PricingConfigLoader pricingConfigLoader;
    pricingConfigLoader.setConfigFile("./PricingConfig/PricingEngines.xml");
    PricingEngineConfig pricerConfig = pricingConfigLoader.loadConfig();
    
    for (const auto& configItem : pricerConfig) {
                const std::string tradeType = configItem.getTradeType();
        

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

void ParallelPricer::price(const std::vector<std::vector<ITrade*>>& tradeContainers, 
                           IScalarResultReceiver* resultReceiver) {
    loadPricers();
    
    std::vector<std::future<void>> futures;
    futures.reserve(tradeContainers.size());

    // Note the resultReceiver will also be needed to make thread safe. 

    std::vector<std::thread> threads;
    threads.reserve(tradeContainers.size());
    for (const auto& tradeContainer : tradeContainers) {
        
        for (ITrade* trade : tradeContainer) {
            if (!trade) continue;

            std::promise<void> p;
            futures.push_back(p.get_future());

            threads.emplace_back([this, trade, &resultReceiver, p = std::move(p)]() mutable {
                try {
                    const std::string tradeType = trade->getTradeType();

                    auto it = pricers_.find(tradeType);
                    if (it == pricers_.end() || it->second == nullptr) {
                        resultReceiver->addError(trade->getTradeId(),
                            "No Pricing Engines available for this trade type");
                        p.set_value();
                        return;
                    }

                    it->second->price(trade, resultReceiver);

                    p.set_value();
                } catch (...) {
                    
                    p.set_exception(std::current_exception());
                }
            });
        }
    }
    
    for (auto& f : futures) {
        f.get();
    }

    
    for (auto& t : threads) {
        if (t.joinable()) t.join();
    }

}
