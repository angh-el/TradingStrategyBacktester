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
    BollingerBandMeanReversion(int per = 20, double stdMult = 2.0, double entryThresh = 0.3, double exitThresh = 0.1);
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







/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////




#ifndef RSIMEANREVERSION_HPP
#define RSIMEANREVERSION_HPP

#include "IStrategy.hpp"
#include "../indicators/IIndicator.hpp"
#include <memory>

class RSIMeanReversion : public IStrategy {
private:
    int period;              
    double overboughtLevel;  
    double oversoldLevel;    
    double neutralZone;      
    double exitBandwidth;    
    
    std::unique_ptr<RelativeStrengthIndex> rsi;
    
    double prevRSI;
    bool hasPreviousValues;
    
    bool isOversold(double rsiValue) const;
    bool isOverbought(double rsiValue) const;
    bool isInNeutralZone(double rsiValue) const;
    bool isRSIRising(double currentRSI, double prevRSI) const;
    bool isRSIFalling(double currentRSI, double prevRSI) const;
    bool isBullishReversal(double currentRSI, double prevRSI) const;
    bool isBearishReversal(double currentRSI, double prevRSI) const;

public:
    RSIMeanReversion(int per = 3, double overbought = 70.0, double oversold = 30.0, 
                     double neutral = 50.0, double exitBand = 5.0);
    // RSIMeanReversion(int per = 14, double overbought = 50.0, double oversold = 10.0, 
    //                  double neutral = 50.0, double exitBand = 2.0);
    
    ~RSIMeanReversion() = default;
    
    Signal generateSignal(const Candle &candle, const IndicatorManager &indicators) override;
    std::string getName() const override;
    void reset() override;
    
    int getPeriod() const { return period; }
    double getOverboughtLevel() const { return overboughtLevel; }
    double getOversoldLevel() const { return oversoldLevel; }
    double getNeutralZone() const { return neutralZone; }
    double getExitBandwidth() const { return exitBandwidth; }
};

#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////




#ifndef MYSTRATEGY_HPP
#define MYSTRATEGY_HPP

#include "IStrategy.hpp"
#include "../indicators/IIndicator.hpp"
#include <memory>

class MyStrategy : public IStrategy {
private:
    int smaPeriod;
    int bbPeriod;
    double bbMultiplier;
    int rsiPeriod;
    int stochasticKPeriod;
    int stochasticDPeriod;
    int adxPeriod;
    double minADXThreshold;

    double rsiNeutralLevel;
    double stochasticLowZone;
    double stochasticHighZone;

    double prevRSI;
    double prevStochasticK;
    double prevStochasticD;
    double prevPrice;
    bool hasPreviousValues;

    bool inPosition;
    double entryPrice;
    SignalType currentPosition;

    bool isTrendConfirmed(double price, double sma) const;
    bool isRSICrossingAbove50(double currentRSI, double prevRSI) const;
    bool isRSICrossingBelow50(double currentRSI, double prevRSI) const;
    bool isBreakoutAboveUpperBB(double price, double upperBB) const;
    bool isBreakoutBelowLowerBB(double price, double lowerBB) const;
    bool isADXStrong(double adx) const;
    bool isStochasticBullishCrossover(double currentK, double currentD, double prevK, double prevD) const;
    bool isStochasticBearishCrossover(double currentK, double currentD, double prevK, double prevD) const;
    bool isStochasticInLowZone(double k, double d) const;
    bool isStochasticInHighZone(double k, double d) const;
    
    bool shouldExitAtMiddleBB(double price, double middleBB) const;
    bool shouldExitOnRSIReversal(double currentRSI, double prevRSI, SignalType position) const;
    bool shouldExitOnStochasticReversal(double currentK, double currentD, double prevK, double prevD, SignalType position) const;

public:
    MyStrategy(
        int smaPer = 20,
        int bbPer = 20, double bbMult = 2.0,
        int rsiPer = 14,
        int stochKPer = 14, int stochDPer = 3,
        int adxPer = 14, double minADX = 15.0,
        double rsiNeutral = 50.0,
        double stochLow = 30.0, double stochHigh = 70.0
    );

    ~MyStrategy() = default;
    Signal generateSignal(const Candle &candle, const IndicatorManager &indicators) override;
    std::string getName() const override;
    void reset() override;
    
    
    int getSMAPeriod() const { return smaPeriod; }
    int getBBPeriod() const { return bbPeriod; }
    double getBBMultiplier() const { return bbMultiplier; }
    int getRSIPeriod() const { return rsiPeriod; }
    int getStochasticKPeriod() const { return stochasticKPeriod; }
    int getStochasticDPeriod() const { return stochasticDPeriod; }
    int getADXPeriod() const { return adxPeriod; }
    double getMinADXThreshold() const { return minADXThreshold; }
    bool isInPosition() const { return inPosition; }
    SignalType getCurrentPosition() const { return currentPosition; }



};  


#endif
