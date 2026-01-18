#ifndef SCALARRESULTS_H
#define SCALARRESULTS_H

#include "IScalarResultReceiver.h"
#include "ScalarResult.h"
#include <map>
#include <vector>
#include <optional>
#include <string>
#include <iterator>

class ScalarResults : public IScalarResultReceiver {
public:
    virtual ~ScalarResults();
    std::optional<ScalarResult> operator[](const std::string& tradeId) const;

    bool containsTrade(const std::string& tradeId) const;

    virtual void addResult(const std::string& tradeId, double result) override;

    virtual void addError(const std::string& tradeId, const std::string& error) override;

    class Iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = ScalarResult;
        using difference_type = std::ptrdiff_t;
        using pointer = ScalarResult*;
        using reference = ScalarResult&;

        Iterator() = default;

        explicit Iterator(const ScalarResults* parent, bool isEnd = false);

        Iterator& operator++();
        ScalarResult operator*() const;
        bool operator!=(const Iterator& other) const;


    private:
        void advanceToNext();  // sets currentKey_ and advances internal iterators

        const ScalarResults* parent_ = nullptr;

        std::map<std::string, double>::const_iterator resIt_{};
        std::map<std::string, double>::const_iterator resEnd_{};

        std::map<std::string, std::string>::const_iterator errIt_{};
        std::map<std::string, std::string>::const_iterator errEnd_{};

        std::string currentKey_{};
        bool atEnd_ = true;
    };

    Iterator begin() const;
    Iterator end() const;

private:
    std::map<std::string, double> results_;
    std::map<std::string, std::string> errors_;
};

#endif // SCALARRESULTS_H
