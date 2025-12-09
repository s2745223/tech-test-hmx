#include "SerialTradeLoader.h"
#include "../Loaders/BondTradeLoader.h"
#include "../Loaders/FxTradeLoader.h"

std::vector<ITradeLoader*> SerialTradeLoader::getTradeLoaders() {
    std::vector<ITradeLoader*> loaders;
    
    BondTradeLoader* bondLoader = new BondTradeLoader();
    bondLoader->setDataFile("TradeData/BondTrades.dat");
    loaders.push_back(bondLoader);
    
    FxTradeLoader* fxLoader = new FxTradeLoader();
    fxLoader->setDataFile("TradeData/FxTrades.dat");
    loaders.push_back(fxLoader);
    
    return loaders;
}

std::vector<std::vector<ITrade*>> SerialTradeLoader::loadTrades() {
    auto loaders = getTradeLoaders();
    std::vector<std::vector<ITrade*>> result;
    
    for (auto loader : loaders) {
        result.push_back(loader->loadTrades());
    }
    
    return result;
}

