#ifndef FXTRADELOADER_H
#define FXTRADELOADER_H

#include "ITradeLoader.h"
#include "../Models/FxTrade.h"
#include "../Models/FxTradeList.h"
#include <string>
#include <vector>
#include <memory>

class FxTradeLoader : public ITradeLoader {
private:
    static constexpr char separator = '|';
    std::string dataFile_;
    
    FxTrade* createTradeFromLine(std::string line);
    void loadTradesFromFile(std::string filename, FxTradeList& tradeList);
    int last_lineCount = 1;
public:
    // NOTE: These methods are only here to allow the solution to compile prior to the test being completed.
    ITrade* loadOneByOneTrades() override;
    std::vector<ITrade*> loadTrades() override;
    std::string getDataFile() const override;
    void setDataFile(const std::string& file) override;
};

#endif // FXTRADELOADER_H
