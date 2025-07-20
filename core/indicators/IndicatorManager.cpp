#include "IIndicator.hpp"



// actual manager class
IndicatorManager::IndicatorManager()
    // : sma(), bb(), rsi(), stochastic(), adx()
    {
    
    // indicators.emplace_back(&sma);
    // indicators.emplace_back(&bb); 
    // indicators.emplace_back(&rsi); 
    // indicators.emplace_back(&stochastic);
    // indicators.emplace_back(&adx);
    

    // indicators.emplace_back(std::make_unique<SimpleMovingAverage>(10));
    // indicators.emplace_back(std::make_unique<BollingerBands>(10, 2));
    // indicators.emplace_back(std::make_unique<RelativeStrengthIndex>(14));
    // indicators.emplace_back(std::make_unique<Stochastic>(14, 3));
    // indicators.emplace_back(std::make_unique<AverageDirectionalIndex>(14));

    
    sma = std::make_unique<SimpleMovingAverage>();
    bb = std::make_unique<BollingerBands>(10, 4);
    rsi = std::make_unique<RelativeStrengthIndex>();
    stochastic = std::make_unique<Stochastic>();
    adx = std::make_unique<AverageDirectionalIndex>();
        
    // indicators.emplace_back(sma);
    // indicators.emplace_back(bb);
    // indicators.emplace_back(rsi);
    // indicators.emplace_back(stochastic);
    // indicators.emplace_back(adx);


}

void IndicatorManager::update(const Candle &candle){
    // for(auto &indicator:indicators){
        // indicator->update(candle);
    // }

    sma->update(candle);
    bb->update(candle);
    rsi->update(candle);
    stochastic->update(candle);
    adx->update(candle);
}










void SimpleMovingAverage::update(const Candle &candle){
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


void BollingerBands::update(const Candle &candle){
    // std::cout<<candle.date<<"\n";
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
        
        middleBand = mean;

        double variance = 0;
        for (double val : window){
            variance += (val - mean) * (val - mean);
        }
        double sd = sqrt(variance / period);

        // double upper = mean + multiplier * sd;
        // double lower = mean - multiplier * sd;
        upperBand = mean + multiplier * sd;
        lowerBand = mean - multiplier * sd;



        // std::cout<<"Standard Deviation: "<<sd<<"\n";
        // std::cout << "Mean: " << mean << " Upper: " << upper << " Lower: " << lower << "\n";
    }
}


void RelativeStrengthIndex::update(const Candle &candle){
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
        rsi = 100 - (100 / (1 + rs));

        // std::cout<<"RSI: "<<rsi<<"\n";
    }
}


void Stochastic::update(const Candle &candle) {
    candleWindow.push_back(candle);

    // Remove old candles to maintain window size
    if (candleWindow.size() > percentKPeriod) {
        candleWindow.pop_front();
    }

    // Only calculate when we have enough data
    if (candleWindow.size() == percentKPeriod) {
        double lowestLow = candleWindow.front().low;
        double highestHigh = candleWindow.front().high;

        for (const auto& c : candleWindow) {
            if (c.low < lowestLow) lowestLow = c.low;
            if (c.high > highestHigh) highestHigh = c.high;
        }

        if (highestHigh == lowestLow) {
            std::cout << "Flat range - skipping\n";
            return;  // avoid divide by zero
        }

        percentK = ((candle.close - lowestLow) / (highestHigh - lowestLow)) * 100.0;
        // std::cout << "Percent K: " << percentK << "\n";

        // Update %D window
        percentKWindow.push_back(percentK);
        percentKSum += percentK;

        if (percentKWindow.size() > percentDPeriod) {
            percentKSum -= percentKWindow.front();
            percentKWindow.pop_front();
        }

        if (percentKWindow.size() == percentDPeriod) {
            percentD = percentKSum / percentDPeriod;
            
            if (std::abs(percentD) < 1e-8){
                percentD = 0.0; 
            }
            
            // std::cout << "Percent D: " << percentD << "\n";
        }
    }
}


double AverageDirectionalIndex::trueRange(double high, double low, double prevClose) {
    double hl = high - low;
    double hc = std::abs(high - prevClose);
    double lc = std::abs(low - prevClose);

    return std::max(hl, std::max(hc, lc)); 
}


void AverageDirectionalIndex::update(const Candle& candle) {
    if (prevClose == 0.0) {
        prevHigh = candle.high;
        prevLow = candle.low;
        prevClose = candle.close;
        return; 
    }

    double upMove = candle.high - prevHigh;
    double downMove = prevLow - candle.low;

    double plusDM = 0.0;
    double minusDM = 0.0;

    if (upMove > downMove && upMove > 0)
        plusDM = upMove;
    else if (downMove > upMove && downMove > 0)
        minusDM = downMove;

    double tr = trueRange(candle.high, candle.low, prevClose);

   
    plusDMValues.push_back(plusDM);
    minusDMValues.push_back(minusDM);
    trValues.push_back(tr);

    if (plusDMValues.size() > period) {
        plusDMValues.pop_front();
        minusDMValues.pop_front();
        trValues.pop_front();
    }

    if (plusDMValues.size() == period) {
        double sumTR = 0, sumPlusDM = 0, sumMinusDM = 0;
        for (int i = 0; i < period; ++i) {
            sumTR += trValues[i];
            sumPlusDM += plusDMValues[i];
            sumMinusDM += minusDMValues[i];
        }

        double plusDI = 100 * (sumPlusDM / sumTR);
        double minusDI = 100 * (sumMinusDM / sumTR);

        double dx = 100 * std::abs(plusDI - minusDI) / (plusDI + minusDI);
        dxValues.push_back(dx);

        if (dxValues.size() > period) dxValues.pop_front();

        if (dxValues.size() == period) {
            adx = 0;
            for (double val : dxValues) adx += val;
            adx /= period;

            // std::cout << "ADX: " << adx << "\n";
        }
    }

    // Update prevs
    prevHigh = candle.high;
    prevLow = candle.low;
    prevClose = candle.close;
}