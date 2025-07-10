#include "Indicators.hpp"


// int Indicators::period = 5;

void Indicators::update(Candle candle){
    // Simpel moving average
    static SimpleMovingAverage sma;
    sma.calculate(candle);

    // Bollinger Bands
    static BollingerBands bb;
    bb.calculate(candle);

    // Relative Strength Index
    static RelativeStrengthIndex rsi;
    rsi.calculate(candle);

    // Stochastic
    static Stochastic s;
    s.calculate(candle);

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


void Indicators::BollingerBands::calculate(Candle candle){
    window.push_back(candle.close);
    if (window.size() > period){
        window.pop_front();
    }

    if (window.size() == period){
        double sum = 0;
        for (double val : window){
            sum += val;
        }
        double mean = sum / period;

        double variance = 0;
        for (double val : window){
            variance += (val - mean) * (val - mean);
        }
        double sd = sqrt(variance / period);

        double upper = mean + multiplier * sd;
        double lower = mean - multiplier * sd;
        // std::cout<<"Standard Deviation: "<<sd<<"\n";
        // std::cout << "Mean: " << mean << " Upper: " << upper << " Lower: " << lower << "\n";
    }
}

void Indicators::RelativeStrengthIndex::calculate(Candle candle){
    window.push_back(candle.close);
    if(window.size() > period){
        window.pop_front();
    }

    if(window.size() == period){
        double sumGains = 0;
        double sumLoses = 0;

        for(int i = 0; i < window.size() - 1; i++){
            // that means we have a gain
            if(window[i+1] >= window[i]){
                sumGains += window[i+1] - window[i];
            }
            // we have a loss
            else{
                sumLoses += window[i] - window[i+1];
            }
        }

        // now we can calculate the Relative Strength
        // RS = Avg Gain / Avg Loss
        double rs = (sumGains / period - 1) / (sumLoses / period - 1);
        // double rs = (sumGains / period) / (sumLoses / period);
        
        // rsi = 100 - (100 / (1 + RS))
        double rsi = 100 - (100 / (1 + rs));

        // std::cout<<"RSI: "<<rsi<<"\n";
    }


}

void Indicators::Stochastic::calculate(Candle candle){
    lowestLow = std::min(lowestLow, candle.low);
    highestHigh = std::max(highestHigh, candle.high);
    // std::cout<<lowestLow<<" ";
    if(currentDayInPeriod == percentKPeriod){
        double percentK = ((candle.close - lowestLow) / (highestHigh - lowestLow) * 100);

        window.push(percentK);
        currentDayInPeriod = 0;

        std::cout<<"Percent K: "<<percentK<<"\n";
    }

    currentDayInPeriod++;
}