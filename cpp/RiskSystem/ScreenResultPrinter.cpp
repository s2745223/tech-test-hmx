#include "ScreenResultPrinter.h"
#include <iostream>

void ScreenResultPrinter::printResults(ScalarResults& results) {
    
    for (const auto& r : results) {
        std::cout << r.getTradeId();

        if (r.getResult().has_value()) {
            std::cout << " : " << r.getResult().value();
        }

        if (r.getError().has_value()) {
            std::cout << " : " << r.getError().value();
        }

        std::cout << std::endl;
    }
    
}
