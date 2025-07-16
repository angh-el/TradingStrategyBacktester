#include "IStrategy.hpp"

BollingerBandMeanReversion::BollingerBandMeanReversion(int per, double stdMult, double entryThresh, double exitThresh)
    : period(per), stdDevMultiplier(stdMult), entryThreshold(entryThresh), exitThreshold(exitThresh),
      prevPrice(0.0), prevUpperBand(0.0), prevLowerBand(0.0), prevMiddleBand(0.0), hasPreviousValues(false) {
    bollingerBands = std::make_unique<BollingerBands>(period, stdDevMultiplier);
}

Signal BollingerBandMeanReversion::generateSignal(const Candle &candle, const IndicatorManager &indicators) {
    bollingerBands->update(candle);
    
    
    
    double currentPrice = candle.close;
    double upperBand = bollingerBands->getUpper();
    double lowerBand = bollingerBands->getLower();
    double middleBand = bollingerBands->getMiddle();
    
    Signal signal(SignalType::HOLD, candle.close, candle.date);
    
    if (hasPreviousValues) {
        if (isPriceNearLowerBand(currentPrice, lowerBand, middleBand) && 
            isBounceOffLowerBand(currentPrice, prevPrice, lowerBand, prevLowerBand)) {
            signal.type = SignalType::BUY;
        }
        else if (isPriceNearUpperBand(currentPrice, upperBand, middleBand) && 
                 isBounceOffUpperBand(currentPrice, prevPrice, upperBand, prevUpperBand)) {
            signal.type = SignalType::SELL;
        }
        else if (isPriceNearMiddleBand(currentPrice, middleBand, upperBand, lowerBand)) {
            signal.type = SignalType::SELL;
        }
    }
    
    prevPrice = currentPrice;
    prevUpperBand = upperBand;
    prevLowerBand = lowerBand;
    prevMiddleBand = middleBand;
    hasPreviousValues = true;
    return signal;
}

std::string BollingerBandMeanReversion::getName() const {
    return "Bollinger Band Mean Reversion (" + std::to_string(period) + "," + 
           std::to_string(stdDevMultiplier) + "," + std::to_string(entryThreshold) + "," + 
           std::to_string(exitThreshold) + ")";
}

void BollingerBandMeanReversion::reset() {
    prevPrice = 0.0;
    prevUpperBand = 0.0;
    prevLowerBand = 0.0;
    prevMiddleBand = 0.0;
    hasPreviousValues = false;
}

bool BollingerBandMeanReversion::isPriceNearUpperBand(double price, double upperBand, double middleBand) const {
    double bandWidth = upperBand - middleBand;
    double threshold = upperBand - (bandWidth * entryThreshold);
    return price >= threshold;
}

bool BollingerBandMeanReversion::isPriceNearLowerBand(double price, double lowerBand, double middleBand) const {
    double bandWidth = middleBand - lowerBand;
    double threshold = lowerBand + (bandWidth * entryThreshold);
    return price <= threshold;
}

bool BollingerBandMeanReversion::isPriceNearMiddleBand(double price, double middleBand, double upperBand, double lowerBand) const {
    double upperBandWidth = upperBand - middleBand;
    double lowerBandWidth = middleBand - lowerBand;
    double upperThreshold = middleBand + (upperBandWidth * exitThreshold);
    double lowerThreshold = middleBand - (lowerBandWidth * exitThreshold);
    return price <= upperThreshold && price >= lowerThreshold;
}

bool BollingerBandMeanReversion::isBounceOffLowerBand(double currentPrice, double prevPrice, double lowerBand, double prevLowerBand) const {
    // return (prevPrice <= prevLowerBand) && (currentPrice > lowerBand);
    return prevPrice <= prevLowerBand && currentPrice > prevPrice;
}

bool BollingerBandMeanReversion::isBounceOffUpperBand(double currentPrice, double prevPrice, double upperBand, double prevUpperBand) const {
    return (prevPrice >= prevUpperBand) && (currentPrice < upperBand);
}