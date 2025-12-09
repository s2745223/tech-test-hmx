#include "StreamingTradeLoader.h"
#include "../Loaders/BondTradeLoader.h"
#include "../Loaders/FxTradeLoader.h"
#include "PricingConfigLoader.h"
#include <stdexcept>

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
    throw std::runtime_error("Not implemented");
}

StreamingTradeLoader::~StreamingTradeLoader() {
    
}

void StreamingTradeLoader::loadAndPrice(IScalarResultReceiver* resultReceiver) {
    throw std::runtime_error("Not implemented");
}
