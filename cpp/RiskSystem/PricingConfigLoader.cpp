#include "PricingConfigLoader.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <cctype>

static std::string readAll(const std::string& file) {
    std::ifstream in(file, std::ios::binary);
    if (!in.is_open()) throw std::runtime_error("Cannot open file: " + file);
    std::ostringstream ss;
    ss << in.rdbuf();
    return ss.str();
}

static inline void stripUtf8Bom(std::string& s) {
    if (s.size() >= 3 &&
        (unsigned char)s[0] == 0xEF &&
        (unsigned char)s[1] == 0xBB &&
        (unsigned char)s[2] == 0xBF) {
        s.erase(0, 3);
    }
}

static inline std::string trim(const std::string& s) {
    size_t a = 0, b = s.size();
    while (a < b && std::isspace((unsigned char)s[a])) ++a;
    while (b > a && std::isspace((unsigned char)s[b - 1])) --b;
    return s.substr(a, b - a);
}

// Extract attribute value from a tag string like: key="value"
static std::string getAttr(const std::string& tag, const std::string& key) {
    const std::string pat = key + "=\"";
    size_t p = tag.find(pat);
    if (p == std::string::npos) return "";
    p += pat.size();
    size_t q = tag.find('"', p);
    if (q == std::string::npos) return "";
    return tag.substr(p, q - p);
}

std::string PricingConfigLoader::getConfigFile() const {
    return configFile_;
}

void PricingConfigLoader::setConfigFile(const std::string& file) {
    configFile_ = file;
}

PricingEngineConfig PricingConfigLoader::loadConfig() {
        if (configFile_.empty()) {
        throw std::invalid_argument("Config filename cannot be empty");
    }

    std::string xml = readAll(configFile_);
    stripUtf8Bom(xml);

    // Optional: quickly sanity-check root exists
    if (xml.find("<PricingEngines") == std::string::npos) {
        throw std::runtime_error("Missing <PricingEngines> in config");
    }

    PricingEngineConfig cfg;

    size_t pos = 0;
    while (true) {
        // Find next <Engine
        size_t start = xml.find("<Engine", pos);
        if (start == std::string::npos) break;

        // Find the end of this tag '>'
        size_t end = xml.find('>', start);
        if (end == std::string::npos) {
            throw std::runtime_error("Malformed <Engine> tag");
        }

        // Grab the full tag "<Engine ... >"
        std::string tag = xml.substr(start, end - start + 1);

        // Extract required attributes (matches your file)
        std::string tradeType     = trim(getAttr(tag, "tradeType"));
        std::string assembly      = trim(getAttr(tag, "assembly"));
        std::string pricingEngine = trim(getAttr(tag, "pricingEngine"));

        if (tradeType.empty() || assembly.empty() || pricingEngine.empty()) {
            throw std::runtime_error("Engine element missing required attributes");
        }

        
        PricingEngineConfigItem item;
        item.setTradeType(tradeType);
        item.setAssembly(assembly);
        item.setTypeName(pricingEngine);
        cfg.push_back(item);
        

        pos = end + 1;
    }

    return cfg;
}
