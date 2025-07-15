#include "IStrategy.hpp"
#include "sstream"

SimpleMovingAverageCrossover::SimpleMovingAverageCrossover(int fast, int slow)
    : fastPeriod(fast), slowPeriod(slow), prevFastSMA(0.0), prevSlowSMA(0.0), hasPreviousValues(false){

    fastSMA = std::make_unique<SimpleMovingAverage> (fastPeriod);
    slowSMA = std::make_unique<SimpleMovingAverage> (slowPeriod);
}

Signal SimpleMovingAverageCrossover::generateSignal(const Candle &candle, const IndicatorManager &indicators){
    fastSMA -> update(candle);
    slowSMA -> update(candle);

    double currentFastSMA = fastSMA -> getSMA();
    double currentSlowSMA = slowSMA -> getSMA();

    // std::cout<<"fast: "<<currentFastSMA<<" slow: "<<currentSlowSMA<<"\n";

    // at least two data points needed for crossover
    if(!hasPreviousValues){
        prevFastSMA = currentFastSMA;
        prevSlowSMA = currentSlowSMA;
        hasPreviousValues = true;
        // std::cout<<"HOLD\n";
        return Signal(SignalType::HOLD, candle.close, candle.date);
    }

    if(isBullishCrossover(currentFastSMA, currentSlowSMA, prevFastSMA, prevSlowSMA)){
        prevFastSMA = currentFastSMA;
        prevSlowSMA = currentSlowSMA;
        // std::cout<<"BUY\n";
        return Signal(SignalType::BUY, candle.close, candle.date);
    }

    else if(isBearishCrossover(currentFastSMA, currentSlowSMA, prevFastSMA, prevSlowSMA)){
        prevFastSMA = currentFastSMA;
        prevSlowSMA = currentSlowSMA;
        // std::cout<<"SELL\n";
        return Signal(SignalType::SELL, candle.close, candle.date);
    }

    // if no crossover just hold
    prevFastSMA = currentFastSMA;
    prevSlowSMA = currentSlowSMA;
    // std::cout<<"HOLD\n";
    return Signal(SignalType::HOLD, candle.close, candle.date); 
}

std::string SimpleMovingAverageCrossover::getName() const{
    std::stringstream ss;
    ss << "SMA Crossover ("<<fastPeriod<<"/"<<slowPeriod<<")";
    return ss.str();
}

void SimpleMovingAverageCrossover::reset(){
    prevFastSMA = 0.0;
    prevSlowSMA = 0.0;
    hasPreviousValues = false;

    fastSMA = std::make_unique<SimpleMovingAverage> (fastPeriod);
    slowSMA = std::make_unique<SimpleMovingAverage> (slowPeriod);
}

bool SimpleMovingAverageCrossover::isBullishCrossover(double currentFast, double currentSlow, double prevFast, double prevSlow) const{
    // bullish when fast was below slow BUT now fast is above slow
    return (prevFast <= prevSlow) && (currentFast > currentSlow);
}

bool SimpleMovingAverageCrossover::isBearishCrossover(double currentFast, double currentSlow, double prevFast, double prevSlow) const{
    // bearish when fast was above slow BUT now fast is below slow
    return (prevFast >= prevSlow) && (currentFast < currentSlow);
}