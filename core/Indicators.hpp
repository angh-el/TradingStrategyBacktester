#ifndef INDICATORS_HPPP
#define INDICATORS_HPPP

#include "MarketData.hpp"

#include <queue>
#include <deque>
#include <cmath>

namespace Indicators{
    
    

    void update(Candle candle);

    class SimpleMovingAverage{
    private:
        std::queue<double> window;
        double sum = 0;
        double sma;
        const int period = 10;   
    public:
        void calculate(Candle candle);
        double getSMA();
    };

    class BollingerBands{
    private:
        std::deque<double> window;
        double upperBand;
        double lowerBand;
        const int multiplier = 2;
        const int period = 10; 
    public:
        void calculate(Candle candle);
    };

    class RelativeStrengthIndex{
    private:
        std::deque<double> window;
        const int period = 7; 
    public:
        void calculate(Candle candle);
    };

    class Stochastic{
    private:
        std::deque<Candle> candleWindow;
        std::deque<double> percentKWindow;
        double percentKSum = 0;
        
        const int percentKPeriod = 14;
        const int percentDPeriod = 3;
    public: 
        void calculate(Candle candle);
    };

    class AverageDirectionalIndex {
    private:
        std::deque<Candle> candleWindow;
        int period = 14;

        double prevHigh = 0.0;
        double prevLow = 0.0;
        double prevClose = 0.0;

        std::deque<double> trValues;
        std::deque<double> plusDMValues;
        std::deque<double> minusDMValues;

        std::deque<double> dxValues;
        double trueRange(double high, double low, double prevClose);
    public:
        void calculate(const Candle& candle);
    };



}


#endif