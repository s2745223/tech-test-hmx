#ifndef BONDTRADE_H
#define BONDTRADE_H

#include "BaseTrade.h"
#include <stdexcept>

class BondTrade : public BaseTrade {
public:
    static constexpr const char* GovBondTradeType = "GovBond";
    static constexpr const char* CorpBondTradeType = "CorpBond";
    
    BondTrade(const std::string& tradeId, const std::string& tradeType) 
        : tradeType_(tradeType) {
        if (tradeId.empty()) {
            throw std::invalid_argument("A valid non null, non empty trade ID must be provided");
        }
        tradeId_ = tradeId;
    }
    
    std::string getTradeType() const override { return tradeType_; }
    
private:
    std::string tradeType_;
};

#endif // BONDTRADE_H

