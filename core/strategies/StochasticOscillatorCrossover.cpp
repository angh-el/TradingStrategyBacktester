#include "IStrategy.hpp"

StochasticOscillatorCrossover::StochasticOscillatorCrossover(int kPer, int dPer, double overbought, double oversold)
    : kPeriod(kPer), dPeriod(dPer), overboughtLevel(overbought), oversoldLevel(oversold),prevK(0.0), prevD(0.0), hasPreviousValues(false) {
    
    stochastic = std::make_unique<Stochastic>(kPeriod, dPeriod);
}

Signal StochasticOscillatorCrossover::generateSignal(const Candle &candle, const IndicatorManager &indicators) {
    // Update the stochastic oscillator with current candle
    stochastic->update(candle);
    
    // // Check if we have enough data
    // if (!stochastic->isReady()) {
    //     return Signal(SignalType::HOLD, candle.close, candle.timestamp);
    // }
    
    double currentK = stochastic->getPercentK();
    double currentD = stochastic->getPercentD();
    
    Signal signal(SignalType::HOLD, candle.close, candle.date);
    
    // Only generate signals if we have previous values for crossover detection
    if (hasPreviousValues) {
        // Bullish signal: %K crosses above %D in oversold zone
        if (isBullishCrossover(currentK, currentD, prevK, prevD) && isInOversoldZone(currentK, currentD)) {
            signal.type = SignalType::BUY;
        }
        // Bearish signal: %K crosses below %D in overbought zone
        else if (isBearishCrossover(currentK, currentD, prevK, prevD) && isInOverboughtZone(currentK, currentD)) {
            signal.type = SignalType::SELL;
        }
    }
    
    // Store current values for next iteration
    prevK = currentK;
    prevD = currentD;
    hasPreviousValues = true;
    
    return signal;
}

std::string StochasticOscillatorCrossover::getName() const {
    return "Stochastic Oscillator Crossover (" + std::to_string(kPeriod) + "," + 
           std::to_string(dPeriod) + "," + std::to_string(overboughtLevel) + "," + 
           std::to_string(oversoldLevel) + ")";
}

void StochasticOscillatorCrossover::reset() {
    // if (stochastic) {
    //     stochastic->reset();
    // }
    prevK = 0.0;
    prevD = 0.0;
    hasPreviousValues = false;
}

bool StochasticOscillatorCrossover::isBullishCrossover(double currentK, double currentD, double prevK, double prevD) const {
    // %K crosses above %D: previous K was below D, current K is above D
    return (prevK <= prevD) && (currentK > currentD);
}

bool StochasticOscillatorCrossover::isBearishCrossover(double currentK, double currentD, double prevK, double prevD) const {
    // %K crosses below %D: previous K was above D, current K is below D
    return (prevK >= prevD) && (currentK < currentD);
}

bool StochasticOscillatorCrossover::isInOversoldZone(double k, double d) const {
    // Both %K and %D should be in oversold zone for stronger signal
    return (k <= oversoldLevel && d <= oversoldLevel);
}

bool StochasticOscillatorCrossover::isInOverboughtZone(double k, double d) const {
    // Both %K and %D should be in overbought zone for stronger signal
    return (k >= overboughtLevel && d >= overboughtLevel);
}