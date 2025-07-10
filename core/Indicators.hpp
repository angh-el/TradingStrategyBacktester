#ifndef INDICATORS_HPPP
#define INDICATORS_HPPP

#include "MarketData.hpp"

#include <queue>
#include <deque>
#include <cmath>

namespace Indicators{
    
    static int period = 5;

    void update(Candle candle);

    class SimpleMovingAverage{
    private:
        std::queue<double> window;
        double sum = 0;
        double sma;   
    public:
        void calculate(Candle candle);
        double getSMA();
    };

    class BollingerBands{
    private:
        std::deque<double> window;
        double upperBand;
        double lowerBand;
        int multiplier = 2;
    public:
        void calculate(Candle candle);
    };


}


#endif