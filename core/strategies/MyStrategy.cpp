#include "IStrategy.hpp"


MyStrategy::MyStrategy(
    int smaPer, int bbPer, double bbMult, int rsiPer, 
    int stochKPer, int stochDPer, int adxPer, double minADX,
    double rsiNeutral, double stochLow, double stochHigh)
    : smaPeriod(smaPer), bbPeriod(bbPer), bbMultiplier(bbMult),
      rsiPeriod(rsiPer), stochasticKPeriod(stochKPer), stochasticDPeriod(stochDPer),
      adxPeriod(adxPer), minADXThreshold(minADX), rsiNeutralLevel(rsiNeutral),
      stochasticLowZone(stochLow), stochasticHighZone(stochHigh),
      prevRSI(0.0), prevStochasticK(0.0), prevStochasticD(0.0), prevPrice(0.0),
      hasPreviousValues(false), inPosition(false), entryPrice(0.0), 
      currentPosition(SignalType::HOLD) {
}

// Signal MyStrategy::generateSignal(const Candle &candle, const IndicatorManager &indicators) {
//     double currentPrice = candle.close;
    
//     // Get all indicator values
//     double sma = indicators.sma->getSMA();
//     double upperBB = indicators.bb->getUpper();
//     double lowerBB = indicators.bb->getLower();
//     double middleBB = indicators.bb->getMiddle();
//     double rsi = indicators.rsi->getValue();
//     double stochasticK = indicators.stochastic->getPercentK();
//     double stochasticD = indicators.stochastic->getPercentD();
//     double adx = indicators.adx->getADX();
    
//     // std::cout<<rsi<<" ";

//     Signal signal(SignalType::HOLD, currentPrice, candle.date);
    
//     if (!hasPreviousValues) {
//         // Store first values, no signal generation yet
//         prevRSI = rsi;
//         prevStochasticK = stochasticK;
//         prevStochasticD = stochasticD;
//         prevPrice = currentPrice;
//         hasPreviousValues = true;
//         return signal;
//     }
    
//     // Check exit conditions first if we're in a position
//     if (inPosition) {
//         bool shouldExit = false;
        
//         // Exit at middle Bollinger Band (mean reversion)
//         if (shouldExitAtMiddleBB(currentPrice, middleBB)) {
//             shouldExit = true;
//         }
        
//         // Exit on RSI reversal
//         if (shouldExitOnRSIReversal(rsi, prevRSI, currentPosition)) {
//             shouldExit = true;
//         }
        
//         // Exit on Stochastic reversal
//         if (shouldExitOnStochasticReversal(stochasticK, stochasticD, prevStochasticK, prevStochasticD, currentPosition)) {
//             shouldExit = true;
//         }
        
//         if (shouldExit) {
//             inPosition = false;
//             currentPosition = SignalType::HOLD;
//             signal.type = SignalType::SELL; // Close position
//         }
//     }
    
//     // Check entry conditions only if not in position
//     if (!inPosition) {
//         // LONG Entry Conditions
//         bool trendConfirmed = isTrendConfirmed(currentPrice, sma);
//         bool rsiMomentum = isRSICrossingAbove50(rsi, prevRSI);
//         // bool rsiMomentum = true;
//         bool volatilityBreakout = isBreakoutAboveUpperBB(currentPrice, upperBB);
//         bool strongTrend = isADXStrong(adx);
//         bool stochasticTiming = isStochasticBullishCrossover(stochasticK, stochasticD, prevStochasticK, prevStochasticD) &&
//                                isStochasticInLowZone(stochasticK, stochasticD);
        
//         // std::cout << "Conditions - Trend: " << trendConfirmed << " RSI: " << rsiMomentum 
//         //           << " BB: " << volatilityBreakout << " ADX: " << strongTrend 
//         //           << " Stoch: " << stochasticTiming << std::endl;

//         // All conditions must be met for a LONG entry
//         if (trendConfirmed && rsiMomentum && volatilityBreakout && strongTrend && stochasticTiming) {
//             inPosition = true;
//             currentPosition = SignalType::BUY;
//             entryPrice = currentPrice;
//             signal.type = SignalType::BUY;
//         }
        
//         // TODO: Add SHORT conditions when needed
//         // For now, only implementing LONG trades as requested
//     }
    
//     // Update previous values
//     prevRSI = rsi;
//     prevStochasticK = stochasticK;
//     prevStochasticD = stochasticD;
//     prevPrice = currentPrice;
    
//     return signal;
// }





// Signal MyStrategy::generateSignal(const Candle &candle, const IndicatorManager &indicators) {
//     double currentPrice = candle.close;
    
//     // Get key indicator values
//     double sma = indicators.sma->getSMA();
//     double upperBB = indicators.bb->getUpper();
//     double middleBB = indicators.bb->getMiddle();
//     double rsi = indicators.rsi->getValue();
//     double adx = indicators.adx->getADX();
    
//     Signal signal(SignalType::HOLD, currentPrice, candle.date);
    
//     if (!hasPreviousValues) {
//         prevPrice = currentPrice;
//         hasPreviousValues = true;
//         return signal;
//     }
    
//     // Simple exit: price drops below middle BB or RSI drops significantly
//     if (inPosition && (currentPrice <= middleBB || rsi < 40.0)) {
//         inPosition = false;
//         currentPosition = SignalType::HOLD;
//         signal.type = SignalType::SELL;
//         return signal;
//     }
    
//     // Simple entry: Just 3 conditions instead of 5
//     if (!inPosition) {
//         bool uptrend = currentPrice > sma;              // Trend
//         bool breakout = currentPrice > upperBB;         // Volatility breakout
//         bool momentum = rsi > 50.0;                     // Momentum
        
//         if (uptrend && breakout && momentum) {
//             inPosition = true;
//             currentPosition = SignalType::BUY;
//             entryPrice = currentPrice;
//             signal.type = SignalType::BUY;
//         }
//     }
    
//     prevPrice = currentPrice;
//     return signal;
// }




Signal MyStrategy::generateSignal(const Candle &candle, const IndicatorManager &indicators) {
    double currentPrice = candle.close;
    double sma = indicators.sma->getSMA();
    double rsi = indicators.rsi->getValue();
    
    Signal signal(SignalType::HOLD, currentPrice, candle.date);
    
    if (!hasPreviousValues) {
        prevRSI = rsi;
        prevPrice = currentPrice;
        hasPreviousValues = true;
        return signal;
    }
    
    if (inPosition) {
        if (rsi < prevRSI - 2.0 || currentPrice < prevPrice * 0.995) {
            inPosition = false;
            currentPosition = SignalType::HOLD;
            signal.type = SignalType::SELL;
        }
    }
    
    if (!inPosition) {
        bool priceUp = currentPrice > prevPrice;
        bool rsiUp = rsi > prevRSI;
        bool aboveSMA = currentPrice > sma;
        
        int conditions = (priceUp ? 1 : 0) + (rsiUp ? 1 : 0) + (aboveSMA ? 1 : 0);
        
        if (conditions >= 2) {
            inPosition = true;
            currentPosition = SignalType::BUY;
            entryPrice = currentPrice;
            signal.type = SignalType::BUY;
        }
    }
    
    prevRSI = rsi;
    prevPrice = currentPrice;
    return signal;
}




std::string MyStrategy::getName() const {
    return "Multi-Indicator Confluence Strategy (LONG only)";
}

void MyStrategy::reset() {
    prevRSI = 0.0;
    prevStochasticK = 0.0;
    prevStochasticD = 0.0;
    prevPrice = 0.0;
    hasPreviousValues = false;
    inPosition = false;
    entryPrice = 0.0;
    currentPosition = SignalType::HOLD;
}

bool MyStrategy::isTrendConfirmed(double price, double sma) const {
    return price > sma; 
}

bool MyStrategy::isRSICrossingAbove50(double currentRSI, double prevRSI) const {
    return prevRSI < rsiNeutralLevel && currentRSI >= rsiNeutralLevel;
}

bool MyStrategy::isRSICrossingBelow50(double currentRSI, double prevRSI) const {
    return prevRSI > rsiNeutralLevel && currentRSI <= rsiNeutralLevel;
}

bool MyStrategy::isBreakoutAboveUpperBB(double price, double upperBB) const {
    return price > upperBB; 
}

bool MyStrategy::isBreakoutBelowLowerBB(double price, double lowerBB) const {
    return price < lowerBB; 
}

bool MyStrategy::isADXStrong(double adx) const {
    return adx > minADXThreshold; 
}

bool MyStrategy::isStochasticBullishCrossover(double currentK, double currentD, double prevK, double prevD) const {
    return prevK <= prevD && currentK > currentD; 
}

bool MyStrategy::isStochasticBearishCrossover(double currentK, double currentD, double prevK, double prevD) const {
    return prevK >= prevD && currentK < currentD; 
}

bool MyStrategy::isStochasticInLowZone(double k, double d) const {
    return k < stochasticLowZone && d < stochasticLowZone; 
}

bool MyStrategy::isStochasticInHighZone(double k, double d) const {
    return k > stochasticHighZone && d > stochasticHighZone; 
}

bool MyStrategy::shouldExitAtMiddleBB(double price, double middleBB) const {
    if (currentPosition == SignalType::BUY) {
        return price <= middleBB;
    }
    return false;
}

bool MyStrategy::shouldExitOnRSIReversal(double currentRSI, double prevRSI, SignalType position) const {
    if (position == SignalType::BUY) {
        return isRSICrossingBelow50(currentRSI, prevRSI);
    }
    return false;
}

bool MyStrategy::shouldExitOnStochasticReversal(double currentK, double currentD, double prevK, double prevD, SignalType position) const {
    if (position == SignalType::BUY) {
        return isStochasticBearishCrossover(currentK, currentD, prevK, prevD) && 
               isStochasticInHighZone(currentK, currentD);
    }
    return false;
}