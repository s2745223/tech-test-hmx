#include "ScalarResults.h"
#include <stdexcept>

ScalarResults::~ScalarResults() = default;

std::optional<ScalarResult> ScalarResults::operator[](const std::string& tradeId) const {
    if (!containsTrade(tradeId)) {
        return std::nullopt;
    }

    std::optional<double> priceResult = std::nullopt;
    std::optional<std::string> error = std::nullopt;

    auto resultIt = results_.find(tradeId);
    if (resultIt != results_.end()) {
        priceResult = resultIt->second;
    }

    auto errorIt = errors_.find(tradeId);
    if (errorIt != errors_.end()) {
        error = errorIt->second;
    }

    return ScalarResult(tradeId, priceResult, error);
}

bool ScalarResults::containsTrade(const std::string& tradeId) const {
    return results_.find(tradeId) != results_.end() || errors_.find(tradeId) != errors_.end();
}

void ScalarResults::addResult(const std::string& tradeId, double result) {
    results_[tradeId] = result;
}

void ScalarResults::addError(const std::string& tradeId, const std::string& error) {
    errors_[tradeId] = error;
}

ScalarResults::Iterator& ScalarResults::Iterator::operator++() {
    throw std::runtime_error("Iterator not implemented");
}

ScalarResult ScalarResults::Iterator::operator*() const {
    throw std::runtime_error("Iterator not implemented");
}

bool ScalarResults::Iterator::operator!=(const Iterator& other) const {
    throw std::runtime_error("Iterator not implemented");
}

ScalarResults::Iterator ScalarResults::begin() const {
    throw std::runtime_error("Not implemented");
}

ScalarResults::Iterator ScalarResults::end() const {
    throw std::runtime_error("Not implemented");
}
