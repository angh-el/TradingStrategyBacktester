#ifndef ISTRATEGY_HPP
#define ISTRATEGY_HPP

#include "../MarketData.hpp"
#include "../Signal.hpp"

class IStrategy {
public:
    virtual ~IStrategy() = default;
    virtual Signal generateSignal(const Candle &candle) = 0;
};

#endif


// SMA Crossover Strat
#ifndef SMACROSSSTRATEGY_HPP
#define SMACROSSSTRATEGY_HPP

#include "IStrategy.hpp"
#include "../indicators/IIndicator.hpp"

class SmaCrossStrategy : public IStrategy{
private:
    SimpleMovingAverage shortSma{10};
    SimpleMovingAverage longSma{20};
    bool positionOpen = false;
public:
    Signal generateSignal(const Candle &cnadle) override;
    void setPositionOpen(bool open);
};


#endif