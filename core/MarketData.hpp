#ifndef MARKET_DATA_HPP
#define MARKET_DATA_HPP

#include "string"
#include "vector"
#include "iostream"

struct Candle{
    std::string date;
    double open;
    double high;
    double low;
    double close;
    double volume;
};

class MarketData{
private:
    std::vector<Candle> candles;
    int entryIndex = 0;

public:
    void addCandle(Candle candle);
    void printCandles();
    Candle getNextCandle();
};


#endif