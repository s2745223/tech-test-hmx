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


ScalarResults::Iterator::Iterator(const ScalarResults* parent, bool isEnd)
    : parent_(parent) {

    if (!parent_) {
        atEnd_ = true;
        return;
    }

    resIt_  = parent_->results_.cbegin();
    resEnd_ = parent_->results_.cend();

    errIt_  = parent_->errors_.cbegin();
    errEnd_ = parent_->errors_.cend();

    if (isEnd) {
        resIt_ = resEnd_;
        errIt_ = errEnd_;
        atEnd_ = true;
        return;
    }

    atEnd_ = false;
    advanceToNext();
}

void ScalarResults::Iterator::advanceToNext() {
    if (resIt_ == resEnd_ && errIt_ == errEnd_) {
        atEnd_ = true;
        currentKey_.clear();
        return;
    }

    if (resIt_ != resEnd_ && errIt_ != errEnd_) {
        if (resIt_->first < errIt_->first) {
            currentKey_ = resIt_->first;
            ++resIt_;
        } else if (errIt_->first < resIt_->first) {
            currentKey_ = errIt_->first;
            ++errIt_;
        } else {
            currentKey_ = resIt_->first;
            ++resIt_;
            ++errIt_;
        }
    } else if (resIt_ != resEnd_) {
        currentKey_ = resIt_->first;
        ++resIt_;
    } else {
        currentKey_ = errIt_->first;
        ++errIt_;
    }
}

ScalarResults::Iterator& ScalarResults::Iterator::operator++() {
    if (!atEnd_) {
        advanceToNext();
    }
    return *this;
}

ScalarResult ScalarResults::Iterator::operator*() const {
    if (atEnd_ || !parent_) {
        throw std::runtime_error("Dereferencing end iterator");
    }

    auto result = (*parent_)[currentKey_];
    if (!result.has_value()) {
        throw std::runtime_error("Invalid iterator state");
    }

    return result.value();
}

bool ScalarResults::Iterator::operator!=(const Iterator& other) const {
    if (atEnd_ != other.atEnd_) return true;
    if (atEnd_) return false;
    return currentKey_ != other.currentKey_ || parent_ != other.parent_;
}

ScalarResults::Iterator ScalarResults::begin() const {
    return Iterator(this, false);
}

ScalarResults::Iterator ScalarResults::end() const {
    return Iterator(this, true);
}