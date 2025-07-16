#include "IStrategy.hpp"

StochasticOscillatorCrossover::StochasticOscillatorCrossover(int kPer, int dPer, double overbought, double oversold)
    : kPeriod(kPer), dPeriod(dPer), overboughtLevel(overbought), oversoldLevel(oversold),prevK(0.0), prevD(0.0), hasPreviousValues(false) {
    
    stochastic = std::make_unique<Stochastic>(kPeriod, dPeriod);
}

Signal StochasticOscillatorCrossover::generateSignal(const Candle &candle, const IndicatorManager &indicators) {
    stochastic->update(candle);
    

    
    double currentK = stochastic->getPercentK();
    double currentD = stochastic->getPercentD();
    
    Signal signal(SignalType::HOLD, candle.close, candle.date);
    
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

    prevK = 0.0;
    prevD = 0.0;
    hasPreviousValues = false;
}

bool StochasticOscillatorCrossover::isBullishCrossover(double currentK, double currentD, double prevK, double prevD) const {
    return (prevK <= prevD) && (currentK > currentD);
}

bool StochasticOscillatorCrossover::isBearishCrossover(double currentK, double currentD, double prevK, double prevD) const {
    return (prevK >= prevD) && (currentK < currentD);
}

bool StochasticOscillatorCrossover::isInOversoldZone(double k, double d) const {
    return (k <= oversoldLevel && d <= oversoldLevel);
}

bool StochasticOscillatorCrossover::isInOverboughtZone(double k, double d) const {
    return (k >= overboughtLevel && d >= overboughtLevel);
}