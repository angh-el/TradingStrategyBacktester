#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include <memory>
#include <iomanip>
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

    // std::unique_ptr<SimpleMovingAverage>sma = std::make_unique<SimpleMovingAverage>();
    // std::unique_ptr<BollingerBands>bb = std::make_unique<BollingerBands>();
    // std::unique_ptr<Stochastic>stochastic = std::make_unique<Stochastic>();
    // std::unique_ptr<RelativeStrengthIndex> rsi = std::make_unique<RelativeStrengthIndex>();
    // std::unique_ptr<AverageDirectionalIndex> adx = std::make_unique<AverageDirectionalIndex>();    
    

    BacktestConfig config(10000.00);
    config.sizingMethod = PositionSizingMethod::PERCENTAGE_CAPITAL;
    config.positionSize = 0.1;
    config.allowPyramiding = false;
    config.allowShortSelling = true;
    config.transactionCost = 0.0;

    // auto strategy =  std::make_unique<SimpleMovingAverageCrossover>(75,175);
    auto strategy =  std::make_unique<StochasticOscillatorCrossover>(20, 2);
    // auto strategy =  std::make_unique<BollingerBandMeanReversion>();
    // auto strategy =  std::make_unique<RSIMeanReversion>();

    Backtester backtester(std::move(strategy), config);

    while(true){
        Candle candle = mdata.getNextCandle();
        
        if(candle.date.empty()) break;
    

        inidcatorManager.update(candle);

        backtester.processCandle(candle, inidcatorManager);
        
        // sma->update(candle);
        // std::cout<<sma->getSMA()<<" ";

        sma = inidcatorManager.sma->getSMA();
        lowerbb = inidcatorManager.bb->getLower();
        upperbb = inidcatorManager.bb->getUpper();
        stochasticK = inidcatorManager.stochastic->getPercentK();
        stochasticD = inidcatorManager.stochastic->getPercentD();
        rsi = inidcatorManager.rsi->getValue();
        adx = inidcatorManager.adx->getADX();
        
        // std::cout<<"date: "<<candle.date<<"\t";
        // std::cout<<"SMA: "<<sma->getSMA()<<"\t";
        // std::cout<<"lowerbb: "<<bb->getLower()<<"\t";
        // std::cout<<"upperbb: "<<bb->getUpper()<<"\t";
        // std::cout<<"stochasticK: "<<stochastic->getPercentK()<<"\t";
        // std::cout<<"stochasticD: "<<stochastic->getPercentD()<<"\t";
        // std::cout<<"rsi: "<<rsi->getValue()<<"\t";
        // std::cout<<"adx: "<<adx->getADX()<<std::endl;

        std::cout<<"date: "<<candle.date<<"\t";
        std::cout<<"SMA: "<<sma<<"\t";
        std::cout<<"lowerbb: "<<lowerbb<<"\t";
        std::cout<<"upperbb: "<<upperbb<<"\t";
        std::cout<<"stochasticK: "<<stochasticK<<"\t";
        std::cout<<"stochasticD: "<<stochasticD<<"\t";
        std::cout<<"rsi: "<<rsi<<"\t";
        std::cout<<"adx: "<<adx<<std::endl;
        


    }

    // backtester.finalise();
    std::cout<<"Total Return: "<<backtester.getTotalReturnPercent()<<"%"<<std::endl;
    std::cout<<"Number of Trades: "<< backtester.getNumberOfTrades()<<std::endl;
    std::cout<<"Final Capital: "<<backtester.getFinalCapital()<<"\n";
    
    return 0;
}