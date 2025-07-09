#include "MarketData.hpp"


void MarketData::addCandle(Candle candle){
    // std::cout<<candle.open<<" ";
    candles.push_back(candle);
    std::cout<<candles.size();
}

void MarketData::printCandles(){
    std::cout<<candles.size();
    for(int i = 0; i < candles.size(); ++i){
        std::cout<<"D: "<<candles.at(i).date;
        std::cout<<" O: "<<candles.at(i).open;
        std::cout<<" H: "<<candles.at(i).high;
        std::cout<<" L: "<<candles.at(i).low;
        std::cout<<" C: "<<candles.at(i).close;
        std::cout<<" V: "<<candles.at(i).volume;
        std::cout<<"\n";
    }
}

