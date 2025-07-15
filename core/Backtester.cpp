#include "Backtester.hpp"
#include <stdexcept>
#include <algorithm>

Backtester::Backtester(std::unique_ptr<IStrategy> strat, const BacktestConfig &cfg)
    : strategy(std::move(strat)), config(cfg), currentCapital(cfg.initialCapital), currentBarIndex(0){

    if(!strategy){
        throw std::runtime_error("Strategy cannot be null");
    }

    capitalHistory.push_back(currentCapital);
    equityHistory.push_back(currentCapital);
}

void Backtester::processCandle(const Candle &candle, const IndicatorManager &indicators){
    timestamps.push_back(candle.date);

    Signal signal = strategy->generateSignal(candle, indicators);

    if(signal.isBuy() && currentPosition.isClosed()){
        // std::cout<<"BUY\n";
        executeEntry(signal);
    }
    // else if(signal.isSell() && currentPosition.isLong()){
    else if(signal.isSell() && currentPosition.isOpen()){
        // std::cout<<"SELL\n";
        executeExit(signal);
    }

    // for HOLD do nothinggg

    updateCapitalHistory(candle.close);

    currentBarIndex++;
}

void Backtester::executeEntry(const Signal &signal){
    double quantity = calculatePositionSize(signal.price);
    // std::cout<<quantity<<"\n";
    if(quantity <= 0){
        // std::cout<<"yoo\n";
        return;
    } 

    double tradeValue = quantity * signal.price;
    double transactionCost = calculateTransactionCost(tradeValue);

    if(currentCapital < tradeValue + transactionCost){
        // std::cout<<"yoo\n";
        return;  
    } 

    // std::cout<<"yoo\n";
    try{
        if(signal.isBuy()){
            currentPosition.openLong(signal.price, quantity, signal.timestamp, currentBarIndex);
        }

        currentCapital -= (tradeValue + transactionCost);
    }
    catch(const std::exception &e){
        std::cout<<"Error in executeEntry"<<std::endl;
        return;
    }
}

void Backtester::executeExit(const Signal &signal){
    if(!currentPosition.isOpen()) return;

    
    double tradeValue = currentPosition.getQuantity() * signal.price;
    double transactionCost = calculateTransactionCost(tradeValue);
    
    Trade trade = Trade::createFromPosition(currentPosition, signal.price, signal.timestamp, currentBarIndex);
    
    currentCapital += (tradeValue - transactionCost);
    
    trades.push_back(trade);
    
    currentPosition.close();
    // std::cout<<"Current Capital: "<<getFinalCapital()<<"\n";
}

void Backtester::updateCapitalHistory(double currentPrice){
    double currentEquity = currentCapital;

    if(currentPosition.isOpen()){
        currentEquity += currentPosition.getUnrealisedPnL(currentPrice);
    }

    capitalHistory.push_back(currentCapital);
    equityHistory.push_back(currentEquity);
}

double Backtester::calculatePositionSize(double price) const {
    if(config.sizingMethod == PositionSizingMethod::FIXED_AMOUNT){
        return Position::calculateQuantityFixedAmount(config.positionSize, price);
    }
    else{
        return Position::calculateQuantity(currentCapital, price, config.positionSize);
    }
}

double Backtester::calculateTransactionCost(double tradeValue) const{
    double fixedCost =config.transactionCost;
    double percentageCost = tradeValue * config.transactionCostRate;
    return fixedCost + percentageCost;
}

void Backtester::finalise(){
    if(currentPosition.isOpen() && !capitalHistory.empty()){
        // leave for now
        return;
    }
}

void Backtester::reset(){
    currentCapital = config.initialCapital;
    currentPosition.close();
    trades.clear();
    capitalHistory.clear();
    equityHistory.clear();
    timestamps.clear();
    currentBarIndex = 0;
    
    if(strategy) strategy -> reset();

    capitalHistory.push_back(currentCapital);
    equityHistory.push_back(currentCapital);
}

void Backtester::setStrategy(std::unique_ptr<IStrategy> newStrat){
    if(!newStrat){
        throw std::runtime_error("Strategy cannot be null");
    }
    strategy = std::move(newStrat);
}

void Backtester::setConfig(const BacktestConfig &newConfig){
    config = newConfig;
}

double Backtester::getTotalReturn() const{
    if(equityHistory.empty()) return 0.0;

    double finalEquity = equityHistory.back();
    return finalEquity - config.initialCapital;
}

double Backtester::getTotalReturnPercent() const{
    if(config.initialCapital == 0.0 || equityHistory.empty()) return 0.0;

    double finalEquity = equityHistory.back();
    return ((finalEquity - config.initialCapital)/ config.initialCapital) * 100.0;
}