#include "Indicators.hpp"


// int Indicators::period = 5;

void Indicators::update(Candle candle){
    // Simpel moving average
    static SimpleMovingAverage sma;
    sma.calculate(candle);


}


void Indicators::SimpleMovingAverage::calculate(Candle candle){
    window.push(candle.close);
    sum += candle.close;

    if(window.size() > period){
        sum -= window.front();
        window.pop();
    }

    if(window.size() == period){
        double sma = sum/period;
        std::cout<<"SMA: "<<sma<<"\n";
    }
}
