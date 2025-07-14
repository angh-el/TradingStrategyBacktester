#include "MarketData.hpp"


void MarketData::addCandle(Candle candle){
    candles.push_back(candle);
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

Candle MarketData::getNextCandle(){
    if(entryIndex >= candles.size()){
        // just a way to represent that we have reached the end of the entries
        return Candle{"", 0, 0, 0, 0, 0};
    }
    return candles.at(entryIndex++);
}

