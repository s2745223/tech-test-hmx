#include "BondTradeLoader.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <ctime>
#include <iomanip>
#include <chrono>

BondTrade* BondTradeLoader::createTradeFromLine(std::string line) {
    
    std::vector<std::string> items;
    std::stringstream ss(line);
    std::string item;
    
    while (std::getline(ss, item, separator)) {
        if (!item.empty() && (item.back() == '\r' || item.back() == '\n' || item.back() == '\t'))
            item.pop_back();
        items.push_back(item);
        
    }
    
    if (items.size() < 7) {
        throw std::runtime_error("Invalid line format");
    }
    const std::string& tradeType = items[0];
    BondTrade* trade = new BondTrade(items[6], tradeType);
    
    std::tm tm = {};
    std::istringstream dateStream(items[1]);
    dateStream >> std::get_time(&tm, "%Y-%m-%d");
    auto timePoint = std::chrono::system_clock::from_time_t(std::mktime(&tm));
    trade->setTradeDate(timePoint);
    
    trade->setInstrument(items[2]);
    trade->setCounterparty(items[3]);
    trade->setNotional(std::stod(items[4]));
    trade->setRate(std::stod(items[5]));
    
    return trade;
}

void BondTradeLoader::loadTradesFromFile(std::string filename, BondTradeList& tradeList) {
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
        if (lineCount == 0) {
        } else {
            tradeList.add(createTradeFromLine(line));
        }
        lineCount++;
    }
}
ITrade* BondTradeLoader::loadOneByOneTrades(){
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

std::vector<ITrade*> BondTradeLoader::loadTrades() {
    BondTradeList tradeList;
    loadTradesFromFile(dataFile_, tradeList);
    
    std::vector<ITrade*> result;
    for (size_t i = 0; i < tradeList.size(); ++i) {
        result.push_back(tradeList[i]);
    }
    return result;
}

std::string BondTradeLoader::getDataFile() const {
    return dataFile_;
}

void BondTradeLoader::setDataFile(const std::string& file) {
    dataFile_ = file;
}
