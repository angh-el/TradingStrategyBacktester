// Metrics
#ifndef METRICS_HPP
#define METRICS_HPP

#include <string>
#include <vector>
#include <iostream>

struct Trade {
    std::string entryTime;
    std::string exitTime;
    double entryPrice;
    double exitPrice;
    double quantity;
    double pnl;
    int duration;
};

class Metrics {
private:
    std::vector<Trade> trades;
    Trade current;

public:
    void logEntry(const std::string& time, double price, double qty);
    void logExit(const std::string& time, double price, double qty);
    void computeMetrics();
};

#endif



#ifndef BROKER_HPP
#define BROKER_HPP

#include "Signal.hpp"
#include "indicators/IIndicator.hpp"
#include "MarketData.hpp"

class Broker {
private:
    double capital;
    double allocationPct = 0.1;
    double commission = 1.0;
    double position = 0;
    double entryPrice = 0;
    double inPosition = false;
public:
    Broker (double initialCapital);
    double calculateQty(double price) const;
    void executeTrade(const Signal &signal, const Candle &candle, Metrics &metrics);
    bool isInPosition() const;
    double getCapital() const;
};

#endif

