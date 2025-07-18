#include "IStrategy.hpp"

RSIMeanReversion::RSIMeanReversion(int per, double overbought, double oversold, double neutral, double exitBand)
    : period(per), overboughtLevel(overbought), oversoldLevel(oversold), 
      neutralZone(neutral), exitBandwidth(exitBand), prevRSI(0.0), hasPreviousValues(false) {
    rsi = std::make_unique<RelativeStrengthIndex>(period);
}

Signal RSIMeanReversion::generateSignal(const Candle &candle, const IndicatorManager &indicators) {
    // Update the RSI with current candle
    rsi->update(candle);
    
    // Check if we have enough data
    // if (!rsi->isReady()) {
    //     return Signal(SignalType::HOLD, candle.close, candle.timestamp);
    // }
    

    double currentRSI = rsi->getValue();
    
    // std::cout<<candle.date<<" "<<currentRSI<<"\n";

    Signal signal(SignalType::HOLD, candle.close, candle.date);
    
    // Only generate signals if we have previous values
    if (hasPreviousValues) {
        // Buy signal: RSI was oversold and is now rising (bullish reversal)
        if (isBullishReversal(currentRSI, prevRSI)) {
            signal.type = SignalType::BUY;
            // std::cout<<candle.date<<" BUY\n";
        }
        // Sell signal: RSI was overbought and is now falling (bearish reversal)
        else if (isBearishReversal(currentRSI, prevRSI)) {
            signal.type = SignalType::SELL;
            // std::cout<<"SELL\n";
        }
        // Exit signal: RSI returns to neutral zone (mean reversion complete)
        else if (isInNeutralZone(currentRSI)) {
            // This could be used for position management
            // For now, we'll use SELL as a general exit signal
            signal.type = SignalType::SELL;
        }
    }
    
    // Store current RSI for next iteration
    prevRSI = currentRSI;
    hasPreviousValues = true;
    // std::cout<<prevRSI<<"\n";
    return signal;
}

std::string RSIMeanReversion::getName() const {
    return "RSI Mean Reversion (" + std::to_string(period) + "," + 
           std::to_string(overboughtLevel) + "," + std::to_string(oversoldLevel) + "," + 
           std::to_string(neutralZone) + ")";
}

void RSIMeanReversion::reset() {
    // if (rsi) {
    //     rsi->reset();
    // }
    prevRSI = 0.0;
    hasPreviousValues = false;
}

bool RSIMeanReversion::isOversold(double rsiValue) const {
    return rsiValue <= oversoldLevel;
}

bool RSIMeanReversion::isOverbought(double rsiValue) const {
    return rsiValue >= overboughtLevel;
}

bool RSIMeanReversion::isInNeutralZone(double rsiValue) const {
    return rsiValue >= (neutralZone - exitBandwidth) && rsiValue <= (neutralZone + exitBandwidth);
}

bool RSIMeanReversion::isRSIRising(double currentRSI, double prevRSI) const {
    return currentRSI > prevRSI;
}

bool RSIMeanReversion::isRSIFalling(double currentRSI, double prevRSI) const {
    return currentRSI < prevRSI;
}

bool RSIMeanReversion::isBullishReversal(double currentRSI, double prevRSI) const {
    // RSI was oversold and is now rising (potential bullish reversal)
    return isOversold(prevRSI) && isRSIRising(currentRSI, prevRSI);
}

bool RSIMeanReversion::isBearishReversal(double currentRSI, double prevRSI) const {
    // RSI was overbought and is now falling (potential bearish reversal)
    return isOverbought(prevRSI) && isRSIFalling(currentRSI, prevRSI);
}