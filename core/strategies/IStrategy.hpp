#ifndef ISTRATEGY_HPP
#define ISTRATEGY_HPP

#include "../csvManager/MarketData.hpp"
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








/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef STOCHASTICOSCILLATORCROSSOVER_HPP
#define STOCHASTICOSCILLATORCROSSOVER_HPP

#include "IStrategy.hpp"
#include "../indicators/IIndicator.hpp"
#include <memory>

class StochasticOscillatorCrossover : public IStrategy {
private:
    int kPeriod;        
    int dPeriod;        
    double overboughtLevel;  
    double oversoldLevel;    
    
    std::unique_ptr<Stochastic> stochastic;
    
    double prevK;
    double prevD;
    bool hasPreviousValues;
    
    bool isBullishCrossover(double currentK, double currentD, double prevK, double prevD) const;
    bool isBearishCrossover(double currentK, double currentD, double prevK, double prevD) const;
    bool isInOversoldZone(double k, double d) const;
    bool isInOverboughtZone(double k, double d) const;

public:
    StochasticOscillatorCrossover(int kPer = 14, int dPer = 3, double overbought = 80.0, double oversold = 20.0);
    ~StochasticOscillatorCrossover() = default;
    
    Signal generateSignal(const Candle &candle, const IndicatorManager &indicators) override;
    std::string getName() const override;
    void reset() override;
    
    // Getters
    int getKPeriod() const { return kPeriod; }
    int getDPeriod() const { return dPeriod; }
    double getOverboughtLevel() const { return overboughtLevel; }
    double getOversoldLevel() const { return oversoldLevel; }
};

#endif







/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////





#ifndef BOLLINGERBANDMEANREVERSION_HPP
#define BOLLINGERBANDMEANREVERSION_HPP

#include "IStrategy.hpp"
#include "../indicators/IIndicator.hpp"
#include <memory>

class BollingerBandMeanReversion : public IStrategy {
private:
    int period;             
    double stdDevMultiplier; 
    double entryThreshold;   
    double exitThreshold;    
    
    std::unique_ptr<BollingerBands> bollingerBands;
    
    double prevPrice;
    double prevUpperBand;
    double prevLowerBand;
    double prevMiddleBand;
    bool hasPreviousValues;
    
    bool isPriceNearUpperBand(double price, double upperBand, double middleBand) const;
    bool isPriceNearLowerBand(double price, double lowerBand, double middleBand) const;
    bool isPriceNearMiddleBand(double price, double middleBand, double upperBand, double lowerBand) const;
    bool isBounceOffLowerBand(double currentPrice, double prevPrice, double lowerBand, double prevLowerBand) const;
    bool isBounceOffUpperBand(double currentPrice, double prevPrice, double upperBand, double prevUpperBand) const;

public:
    BollingerBandMeanReversion(int per = 20, double stdMult = 2.0, double entryThresh = 0.6, double exitThresh = 0.1);
    ~BollingerBandMeanReversion() = default;
    
    Signal generateSignal(const Candle &candle, const IndicatorManager &indicators) override;
    std::string getName() const override;
    void reset() override;
    
    // Getters
    int getPeriod() const { return period; }
    double getStdDevMultiplier() const { return stdDevMultiplier; }
    double getEntryThreshold() const { return entryThreshold; }
    double getExitThreshold() const { return exitThreshold; }
};


#endif