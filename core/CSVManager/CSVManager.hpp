#ifndef CSVMANAGER_HPP
#define CSVMANAGER_HPP

#include "MarketData.hpp"
#include "../broker/Trade.hpp"
#include "../Backtester.hpp"

#include <vector>
#include <string>

#include <fstream>
#include <sstream>
#include <iostream>

namespace CSVManager{
    void parseCSV(MarketData *mdata, const char *fp);


    // bool headerWritten = false;
    // std::string indicatorsPath = "../../data/indicators.csv";

    
    void writeHeaderIfNeeded();
    void logIndicators( const std::string& date, double sma, double lowerBB, double upperBB, double stochasticK, double stochasticD, double rsi,double adx);

    // log trades
    void logTrades(const std::vector<Trade> trades);

    // log metrics
    void logMetrics(const Backtester& backtester);
}







#endif