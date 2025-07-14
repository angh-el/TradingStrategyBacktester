#include "IStrategy.hpp"


Signal SmaCrossStrategy::generateSignal(const Candle& candle)  {
    shortSma.update(candle);
    longSma.update(candle);

    double shortVal = shortSma.getSMA();
    double longVal = longSma.getSMA();

    // std::cout<<shortVal<<" "<<longVal<<"\n";
    if (!std::isnan(shortVal) && !std::isnan(longVal)) {
        if (!positionOpen && shortVal > longVal){
            // std::cout<<"BUY\n";
            return { SignalType::BUY };
        }
        else if (positionOpen && shortVal < longVal){
            // std::cout<<"SELL\n";
            return { SignalType::SELL };
        }
    }
    // std::cout<<"HOLD\n";
    return { SignalType::HOLD };
}

void SmaCrossStrategy::setPositionOpen(bool open) { positionOpen = open; }
