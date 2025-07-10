#ifndef INDICATORS_HPPP
#define INDICATORS_HPPP

#include "MarketData.hpp"

#include <queue>

namespace Indicators{
    
    static int period = 5;

    void update(Candle candle);

    class SimpleMovingAverage{
    private:
        std::queue<double> window;
        double sum = 0;    
    public:
        void calculate(Candle candle);
    };


}


#endif