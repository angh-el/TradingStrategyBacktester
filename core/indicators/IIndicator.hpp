#ifndef IINDICATOR_HPP
#define IINDICATOR_HPP

#include "../csvManager/MarketData.hpp"

#include <deque>
#include <queue>
#include <cmath>

#include <vector>
#include <memory>

class IIndicator{
public:
    virtual void update(const Candle &candle) = 0;
    virtual ~IIndicator() = default;
};


class SimpleMovingAverage : public IIndicator {
private:
    std::queue<double> window;
    double sum = 0;
    double sma = 0;
    const int period;
public:
    SimpleMovingAverage(int p = 10) : period(p) {}
    void update(const Candle& candle) override;
    double getSMA() const { return sma; }
};


class BollingerBands : public IIndicator {
private:
    std::deque<double> window;
    double upperBand = 0;
    double lowerBand = 0;
    double middleBand = 0;
    const int period;
    const int multiplier;
public:
    BollingerBands(int p = 10, int m = 2) : period(p), multiplier(m) {}
    void update(const Candle& candle) override;
    double getUpper() const { return upperBand; }
    double getLower() const { return lowerBand; }
    double getMiddle() const { return middleBand;}
};


class RelativeStrengthIndex : public IIndicator {
private:
    std::deque<double> window;
    const int period;
    double rsi = 0;
public:
    RelativeStrengthIndex(int p = 7) : period(p) {}
    void update(const Candle& candle) override;
    double getValue() const { return rsi; }
};

class Stochastic : public IIndicator {
private:
    std::deque<Candle> candleWindow;
    std::deque<double> percentKWindow;
    double percentKSum = 0;
    const int percentKPeriod;
    const int percentDPeriod;
    double percentK = 0;
    double percentD = 0;
public:
    Stochastic(int kPeriod = 14, int dPeriod = 3)
        : percentKPeriod(kPeriod), percentDPeriod(dPeriod) {}
    void update(const Candle& candle) override;
    double getPercentK() const { return percentK; }
    double getPercentD() const { return percentD; }
};


class AverageDirectionalIndex : public IIndicator {
private:
    std::deque<Candle> candleWindow;
    int period;

    double prevHigh = 0.0;
    double prevLow = 0.0;
    double prevClose = 0.0;

    std::deque<double> trValues;
    std::deque<double> plusDMValues;
    std::deque<double> minusDMValues;

    std::deque<double> dxValues;
    double adx = 0;

    double trueRange(double high, double low, double prevClose);
public:
    AverageDirectionalIndex(int p = 14) : period(p) {}
    void update(const Candle& candle) override;
    double getADX() const { return adx; }
};


// manager
class IndicatorManager{
private:
    std::vector<std::unique_ptr<IIndicator>> indicators;
public:
    SimpleMovingAverage sma;
    BollingerBands bb;
    RelativeStrengthIndex rsi;
    Stochastic stochastic;
    AverageDirectionalIndex adx;

    IndicatorManager();

    void update(const Candle &candle);

};


#endif