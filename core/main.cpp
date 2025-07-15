#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include <memory>
// #include <queue>

#include "csvManager/MarketData.hpp"
#include "csvManager/CSVManager.hpp"
#include "indicators/IIndicator.hpp"
#include "strategies/IStrategy.hpp"
#include "Backtester.hpp"


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

    // BacktestConfig config(100.00);
    // config.sizingMethod = PositionSizingMethod::PERCENTAGE_CAPITAL;
    // config.positionSize = 0.005;
    // config.allowPyramiding = false;
    // config.allowShortSelling = false;
    // config.transactionCost = 0.0;

    // auto strategy =  std::make_unique<SimpleMovingAverageCrossover>(10,20);
    // Backtester backtester(std::move(strategy), config);

    // while(true){
    //     Candle candle = mdata.getNextCandle();
        
    //     if(candle.date.empty()) break;
    

    //     // inidcatorManager.update(candle);

    //     // backtester.processCandle(candle, inidcatorManager);
        
    //     // sma = inidcatorManager.sma.getSMA();
    //     // lowerbb = inidcatorManager.bb.getLower();
    //     // upperbb = inidcatorManager.bb.getUpper();
    //     // stochasticK = inidcatorManager.stochastic.getPercentK();
    //     // stochasticD = inidcatorManager.stochastic.getPercentD();
    //     // rsi = inidcatorManager.rsi.getValue();
    //     // adx = inidcatorManager.adx.getADX();
        
    //     // std::cout<<"date: "<<candle.date<<"\t";
    //     // std::cout<<"SMA: "<<sma<<"\t";
    //     // std::cout<<"lowerbb: "<<lowerbb<<"\t";
    //     // std::cout<<"upperbb: "<<upperbb<<"\t";
    //     // std::cout<<"stochasticK: "<<stochasticK<<"\t";
    //     // std::cout<<"stochasticD: "<<stochasticD<<"\t";
    //     // std::cout<<"rsi: "<<rsi<<"\t";
    //     // std::cout<<"adx: "<<adx<<std::endl;
        


    // }

    // backtester.finalise();
    // std::cout<<"Total Return: "<<backtester.getTotalReturnPercent()<<"%"<<std::endl;
    // std::cout<<"Number of Trades: "<< backtester.getNumberOfTrades()<<std::endl;
    // std::cout<<"Final Capital: "<<backtester.getFinalCapital()<<"\n";
    
    // std::vector<Trade> trades = backtester.getTrades();
    double total = 0;
    int win = 0;
    int lose = 0;
    // for(int i = 0; i < trades.size(); i++){
    //     total += trades[i].getExitPrice() - trades[i].getEntryPrice();
    //     // trades[i].calculateMetrics();
        
    //     // if(trades[i].getExitPrice() - trades[i].getEntryPrice() > 0){
    //     //     std::cout<<"WIN"<<"\n";
    //     //     win++;
    //     // }
    //     // if(trades[i].getExitPrice() - trades[i].getEntryPrice() < 0){
    //     //     std::cout<<"LOSS"<<"\n";
    //     //     lose++;
    //     // }    
    //     trades[i].calculateMetrics();
    //     total += trades[i].getPnL();
    //     // std::cout<<trades[i].getPnL()<<"\n";
    //     // std::cout<<trades[i].getEntryTime()<<" "<<trades[i].getEntryPrice()<<" "<<trades[i].getExitTime()<<" "<<trades[i].getExitPrice()<<"\n";
    // }
    // // std::cout<<"Win: "<<win<<" Lose: "<<lose;
    // std::cout<<"total "<<total<<"\n";

    return 0;
}