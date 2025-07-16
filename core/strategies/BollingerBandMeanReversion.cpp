#include "IStrategy.hpp"

BollingerBandMeanReversion::BollingerBandMeanReversion(int per, double stdMult, double entryThresh, double exitThresh)
    : period(per), stdDevMultiplier(stdMult), entryThreshold(entryThresh), exitThreshold(exitThresh),
      prevPrice(0.0), prevUpperBand(0.0), prevLowerBand(0.0), prevMiddleBand(0.0), hasPreviousValues(false) {
    bollingerBands = std::make_unique<BollingerBands>(period, stdDevMultiplier);
}

Signal BollingerBandMeanReversion::generateSignal(const Candle &candle, const IndicatorManager &indicators) {
    // Update the Bollinger Bands with current candle
    bollingerBands->update(candle);
    
    
    
    double currentPrice = candle.close;
    double upperBand = bollingerBands->getUpper();
    double lowerBand = bollingerBands->getLower();
    double middleBand = bollingerBands->getMiddle();

    // std::cout<<upperBand<<" "<<middleBand<<" "<<lowerBand<<"\n";
    
    Signal signal(SignalType::HOLD, candle.close, candle.date);
    
    // Only generate signals if we have previous values
    if (hasPreviousValues) {
        // Buy signal: Price bounces off lower band (mean reversion up)
        if (isPriceNearLowerBand(currentPrice, lowerBand, middleBand) && 
            isBounceOffLowerBand(currentPrice, prevPrice, lowerBand, prevLowerBand)) {
            signal.type = SignalType::BUY;
        }
        // Sell signal: Price bounces off upper band (mean reversion down)
        else if (isPriceNearUpperBand(currentPrice, upperBand, middleBand) && 
                 isBounceOffUpperBand(currentPrice, prevPrice, upperBand, prevUpperBand)) {
            signal.type = SignalType::SELL;
        }
        // Exit signal: Price returns near middle band (mean reversion complete)
        else if (isPriceNearMiddleBand(currentPrice, middleBand, upperBand, lowerBand)) {
            // This would typically be handled by your position management
            // For now, we'll use SELL as exit signal
            signal.type = SignalType::SELL;
        }
        // std::cout<<"yo ";
    }
    
    // Store current values for next iteration
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
    // Price was at or below lower band and is now moving up
    // return (prevPrice <= prevLowerBand) && (currentPrice > lowerBand);
    return prevPrice <= prevLowerBand && currentPrice > prevPrice;
}

bool BollingerBandMeanReversion::isBounceOffUpperBand(double currentPrice, double prevPrice, double upperBand, double prevUpperBand) const {
    // Price was at or above upper band and is now moving down
    return (prevPrice >= prevUpperBand) && (currentPrice < upperBand);
}