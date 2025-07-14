#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <vector>
// #include <queue>

#include "csvManager/MarketData.hpp"
#include "csvManager/CSVManager.hpp"
#include "indicators/IIndicator.hpp"

#include "strategies/IStrategy.hpp"



int main(){
    const char * fp = "../data/aapl_1min.csv";
    MarketData mdata;

    CSVManager::parseCSV(&mdata, fp);
    // mdata.printCandles();

    IndicatorManager inidcatorManager;

    double sma;
    double lowerbb;
    double upperbb;
    double stochasticK;
    double stochasticD;
    double rsi;
    double adx;



    while(true){
        Candle candle = mdata.getNextCandle();
        
        if(candle.date.empty()) break;
    

        inidcatorManager.update(candle);
        // sma = inidcatorManager.sma.getSMA();
        // lowerbb = inidcatorManager.bb.getLower();
        // upperbb = inidcatorManager.bb.getUpper();
        // stochasticK = inidcatorManager.stochastic.getPercentK();
        // stochasticD = inidcatorManager.stochastic.getPercentD();
        // rsi = inidcatorManager.rsi.getValue();
        // adx = inidcatorManager.adx.getADX();
        
        // std::cout<<"date: "<<candle.date<<"\t";
        // std::cout<<"SMA: "<<sma<<"\t";
        // std::cout<<"lowerbb: "<<lowerbb<<"\t";
        // std::cout<<"upperbb: "<<upperbb<<"\t";
        // std::cout<<"stochasticK: "<<stochasticK<<"\t";
        // std::cout<<"stochasticD: "<<stochasticD<<"\t";
        // std::cout<<"rsi: "<<rsi<<"\t";
        // std::cout<<"adx: "<<adx<<std::endl;
        


    }

    return 0;
}