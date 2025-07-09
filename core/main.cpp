#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <vector>
#include <queue>

#include "MarketData.hpp"
#include "CSVManager.hpp"


int main(){
    const char * fp = "../data/aapl_1min.csv";
    MarketData mdata;

    CSVManager::parseCSV(&mdata, fp);
    // mdata.printCandles();

    int period = 5;
    std::queue<double> window;
    double sum = 0;

    while(true){
        Candle candle = mdata.getNextCandle();
        if(candle.date.empty()){
            // std::cout<<"end\n";
            break;
        }
        
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

    return 0;
}