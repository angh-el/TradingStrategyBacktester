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


int main(int argc, char* argv[]){
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
   

    BacktestConfig config(10000.00);
    config.sizingMethod = PositionSizingMethod::PERCENTAGE_CAPITAL;
    config.positionSize = 0.1;
    config.allowPyramiding = false;
    config.allowShortSelling = true;
    config.transactionCost = 0.0;

    if(argc < 2){
        std::cerr<<"Error";
    }

    std::string strategyName = argv[1];
    std::unique_ptr<IStrategy> strategy;

    if (strategyName == "sma") {
        strategy = std::make_unique<SimpleMovingAverageCrossover>(75, 175);
    } 
    else if (strategyName == "stochastic") {
        strategy = std::make_unique<StochasticOscillatorCrossover>(20, 2);
    } 
    else if (strategyName == "bollinger") {
        strategy = std::make_unique<BollingerBandMeanReversion>();
    } 
    else if (strategyName == "rsi") {
        strategy = std::make_unique<RSIMeanReversion>();
    }
    else if (strategyName == "my-strat") {
        strategy = std::make_unique<MyStrategy>(10,20, 1.5,7,14, 3,14, 10.0,45.0, 50.0, 50.0);
    } 
    else {
        std::cerr << "Unknown strategy: " << strategyName << "\n";
        return 1;
    }

    // auto strategy =  std::make_unique<SimpleMovingAverageCrossover>(75,175);
    // auto strategy =  std::make_unique<StochasticOscillatorCrossover>(20, 2);
    // auto strategy =  std::make_unique<BollingerBandMeanReversion>();
    // auto strategy =  std::make_unique<RSIMeanReversion>();

    Backtester backtester(std::move(strategy), config);

    while(true){
        Candle candle = mdata.getNextCandle();
        
        if(candle.date.empty()) break;
        

        inidcatorManager.update(candle);

        backtester.processCandle(candle, inidcatorManager);
        

        sma = inidcatorManager.sma->getSMA();
        lowerbb = inidcatorManager.bb->getLower();
        upperbb = inidcatorManager.bb->getUpper();
        stochasticK = inidcatorManager.stochastic->getPercentK();
        stochasticD = inidcatorManager.stochastic->getPercentD();
        rsi = inidcatorManager.rsi->getValue();
        adx = inidcatorManager.adx->getADX();
        
        CSVManager::logIndicators(
            candle.date,
            sma,
            lowerbb,
            upperbb,
            stochasticK,
            stochasticD,
            rsi,
            adx
        );
        
        // std::cout<<"date: "<<candle.date<<"\t";
        // std::cout<<"SMA: "<<sma<<"\t";
        // std::cout<<"lowerbb: "<<lowerbb<<"\t";
        // std::cout<<"upperbb: "<<upperbb<<"\t";
        // std::cout<<"stochasticK: "<<stochasticK<<"\t";
        // std::cout<<"stochasticD: "<<stochasticD<<"\t";
        // std::cout<<"rsi: "<<rsi<<"\t";
        // std::cout<<"adx: "<<adx<<std::endl;
        


    }

    CSVManager::logTrades(backtester.getTrades());
    CSVManager::logMetrics(backtester);

    
    
    // backtester.finalise();

    // std::cout<<"Total Return: "<<backtester.getTotalReturnPercent()<<"%"<<std::endl;
    // std::cout<<"Number of Trades: "<< backtester.getNumberOfTrades()<<std::endl;
    // std::cout<<"Final Capital: "<<backtester.getFinalCapital()<<"\n";
    // std::cout<<"Sharpe Ratio: "<<backtester.getSharpeRatio() <<"\n";
    // std::cout<<"Max Drawdown: "<<backtester.getMaxDrawdown() <<"\n";
    // std::cout<<"Win Rate: "<<backtester.getWinRate() <<"\n";
    // std::cout<<"Profit Factor: "<<backtester.getProfitFactor() <<"\n";
    // std::cout<<"Avg Trade Duration: "<<backtester.getAvgTradeDuration() <<"\n";
    // std::cout<<"Avg Trade Return: "<<backtester.getAvgTradeReturn() <<"\n";
    // std::cout<< <<"\n";

    // backtester.getTrades();

    std::cout<<"Backtester run successfully"<<std::endl;

    return 0;
}