#ifndef ISTRATEGY_HPP
#define ISTRATEGY_HPP

#include "../MarketData.hpp"
#include "../indicators/IIndicator.hpp"

#include <string>

enum class SignalType {HOLD, BUY, SELL};

struct Signal {
    SignalType type;
    double price;
    std::string timestamp;
    
    Signal(SignalType t = SignalType::HOLD, double p = 0.0, const std::string &ts = "")
        : type(t), price(p), timestamp(ts){}

    bool isBuy() const {return type == SignalType::BUY;}
    bool isSell() const {return type == SignalType::SELL;}
    bool isHolde() const {return type == SignalType::HOLD;}
};

class IStrategy{
public:
    virtual ~IStrategy() = default;
    virtual Signal generateSignal(const Candle &candle, const IndicatorManager &indicators) = 0;
    virtual std::string getName() const = 0;
    virtual void reset() = 0;
};

#endif




/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef SIMPLEMOVINGAVERAGECROSSOVER_HPP
#define SIMPLEMOVINGAVERAGECROSSOVER_HPP

#include <memory>

class SimpleMovingAverageCrossover : public IStrategy{
private:
    int fastPeriod;
    int slowPeriod;
    
    std::unique_ptr<SimpleMovingAverage> fastSMA;
    std::unique_ptr<SimpleMovingAverage> slowSMA;

    double prevFastSMA;
    double prevSlowSMA;
    bool hasPreviousValues;

    bool isBullishCrossover(double currentFast, double currentSlow, double prevFast, double prevSlow) const;
    bool isBearishCrossover(double currentFast, double currentSlow, double prevFast, double prevSlow) const;

public:
    SimpleMovingAverageCrossover(int fast = 10, int slow = 20);
    ~SimpleMovingAverageCrossover() = default;

    Signal generateSignal(const Candle &candle, const IndicatorManager &indicators) override;
    std::string getName() const override;
    void reset() override;

    int getFastPeriod() const {return fastPeriod;}
    int getSlowPeriod() const {return slowPeriod;}

};

#endif


