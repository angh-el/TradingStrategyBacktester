#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <vector>
// #include <queue>

#include "MarketData.hpp"
#include "CSVManager.hpp"
#include "Indicators.hpp"


int main(){
    const char * fp = "../data/aapl_1min.csv";
    MarketData mdata;

    CSVManager::parseCSV(&mdata, fp);
    // mdata.printCandles();


    while(true){
        Candle candle = mdata.getNextCandle();
        if(candle.date.empty()){
            // std::cout<<"end\n";
            break;
        }

        Indicators::update(candle);

    }

    return 0;
}