#include "FxTradeLoader.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <ctime>
#include <iomanip>
#include <chrono>

// NOTE: These methods are only here to allow the solution to compile prior to the test being completed.
FxTrade* FxTradeLoader::createTradeFromLine(std::string line) {

    for (size_t p = 0; (p = line.find(u8"¬", p)) != std::string::npos; )
        line.replace(p, std::string(u8"¬").size(), "|");

    std::vector<std::string> items;
    std::stringstream ss(line);
    std::string item;
    
    while (std::getline(ss, item, separator)) {
        if (!item.empty() && (item.back() == '\r' || item.back() == '\n' || item.back() == '\t'))
            item.pop_back();
        items.push_back(item);
        
    }
    
    if((items[0] == "END"))
        return nullptr;

    if (items.size() < 7) {
        throw std::runtime_error("Invalid line format");
    }
    const std::string& tradeType = items[0];
    FxTrade* trade = new FxTrade(items[7], tradeType);
    
    std::tm tm = {};
    std::istringstream dateStream(items[1]);
    dateStream >> std::get_time(&tm, "%Y-%m-%d");
    auto timePoint = std::chrono::system_clock::from_time_t(std::mktime(&tm));
    trade->setTradeDate(timePoint);

    std::tm tm_value = {};
    std::istringstream dateStream_value(items[6]);
    dateStream_value >> std::get_time(&tm_value, "%Y-%m-%d");
    auto timePoint_value = std::chrono::system_clock::from_time_t(std::mktime(&tm_value));
    trade->setValueDate(timePoint_value);
    
    trade->setInstrument(items[2] + items[3]);
    trade->setCounterparty(items[7]);
    trade->setNotional(std::stod(items[4]));
    trade->setRate(std::stod(items[5]));
    
    return trade;
}


ITrade* FxTradeLoader::loadOneByOneTrades(){
    if (dataFile_.empty()) {
        throw std::invalid_argument("Filename cannot be null");
    }
    
    // This stream variable can be made into a class variable rather than creating a reading object again and again. But this decision can be made after proper analysis. 
    std::ifstream stream(dataFile_);
    if (!stream.is_open()) {
        throw std::runtime_error("Cannot open file: " + dataFile_);
    }
    
    int lineCount = 0;
    std::string line;
    ITrade* tempTrade = nullptr;
    while (std::getline(stream, line)) {
        if (lineCount <= last_lineCount) {
        } else {
            tempTrade = createTradeFromLine(line);
            break;
        }
        lineCount++;
    }
    last_lineCount = lineCount;
    return tempTrade;
}

void FxTradeLoader::loadTradesFromFile(std::string filename, FxTradeList& tradeList) {
    if (filename.empty()) {
        throw std::invalid_argument("Filename cannot be null");
    }
    
    std::ifstream stream(filename);
    if (!stream.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }
    
    int lineCount = 0;
    std::string line;
    while (std::getline(stream, line)) {
        if (lineCount <= 1) {
        } else {
            auto trade = createTradeFromLine(line);
            if(trade != nullptr)
                tradeList.add(trade);    
        }
        lineCount++;
    }
}

std::vector<ITrade*> FxTradeLoader::loadTrades() {
    FxTradeList tradeList;
    loadTradesFromFile(dataFile_, tradeList);
    
    std::vector<ITrade*> result;
    for (size_t i = 0; i < tradeList.size(); ++i) {
        result.push_back(tradeList[i]);
    }
    return result;
}

std::string FxTradeLoader::getDataFile() const {
    return dataFile_;
}

void FxTradeLoader::setDataFile(const std::string& file) {
    dataFile_ = file;
}
