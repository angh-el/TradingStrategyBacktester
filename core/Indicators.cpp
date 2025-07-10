#include "Indicators.hpp"


// int Indicators::period = 5;

void Indicators::update(Candle candle){
    // Simpel moving average
    static SimpleMovingAverage sma;
    sma.calculate(candle);

    // Bollinger Bands
    static BollingerBands bb;
    bb.calculate(candle);

}


void Indicators::SimpleMovingAverage::calculate(Candle candle){
    window.push(candle.close);
    sum += candle.close;

    if(window.size() > period){
        sum -= window.front();
        window.pop();
    }

    if(window.size() == period){
        sma = sum/period;
        // std::cout<<"SMA: "<<sma<<"\n";
    }
}

double Indicators::SimpleMovingAverage::getSMA(){
    return sma;
}


void Indicators::BollingerBands::calculate(Candle candle) {
    window.push_back(candle.close);
    if (window.size() > period) {
        window.pop_front();
    }

    if (window.size() == period) {
        double sum = 0;
        for (double val : window) sum += val;
        double mean = sum / period;

        double variance = 0;
        for (double val : window)
            variance += (val - mean) * (val - mean);
        double sd = sqrt(variance / period);

        double upper = mean + multiplier * sd;
        double lower = mean - multiplier * sd;
        // std::cout<<"Standard Deviation: "<<sd<<"\n";
        // std::cout << "Mean: " << mean << " Upper: " << upper << " Lower: " << lower << "\n";
    }
}