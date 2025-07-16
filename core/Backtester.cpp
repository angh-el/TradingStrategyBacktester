// #include "Backtester.hpp"
// #include <stdexcept>
// #include <algorithm>

// Backtester::Backtester(std::unique_ptr<IStrategy> strat, const BacktestConfig &cfg)
//     : strategy(std::move(strat)), config(cfg), currentCapital(cfg.initialCapital), currentBarIndex(0){

//     if(!strategy){
//         throw std::runtime_error("Strategy cannot be null");
//     }

//     capitalHistory.push_back(currentCapital);
//     equityHistory.push_back(currentCapital);
// }

// void Backtester::processCandle(const Candle &candle, const IndicatorManager &indicators){
//     timestamps.push_back(candle.date);
//     lastPrice = candle.close;

//     // std::cout<<currentCapital<<" ";

//     Signal signal = strategy->generateSignal(candle, indicators);
    
//     // if(signal.isBuy()  || signal.isSell())
//     //     std::cout << "[DEBUG] Capital: " << currentCapital 
//     //       << " | Position Qty: " << currentPosition.getQuantity() 
//     //       << " | Entry: " << currentPosition.getEntryPrice() 
//     //       << " | Signal: " << (signal.isBuy() ? "BUY" : signal.isSell() ? "SELL" : "HOLD") 
//     //       << " | Date: " << candle.date << "\n";


//     // if(signal.isBuy() && currentPosition.isClosed()){
//     //     // std::cout<<"BUY\n";
//     //     executeEntry(signal);
//     // }
//     // // else if(signal.isSell() && currentPosition.isLong()){
//     // else if(signal.isSell() && currentPosition.isOpen()){
//     //     // std::cout<<"SELL\n";
//     //     executeExit(signal);
//     // }

//     if (signal.isBuy()) {
//         // std::cout<<"Buy\n";
//         // std::cout<<signal.timestamp<<"\n";
//         if (currentPosition.isClosed()) {
//             executeEntry(signal);
//         } 
//         else if (currentPosition.isLong() && config.allowPyramiding) {
//             executeAddToPosition(signal);
//         } 
//         else if (currentPosition.isShort() && config.allowPartialSells) {
//             executePartialExit(signal);
//         }
//     }
//     else if (signal.isSell()) {
//         // std::cout<<"Sell\n";
//         if (currentPosition.isLong()) {
//             if (config.allowPartialSells) {
//                 executePartialExit(signal);
//             } 
//             else {
//                 executeExit(signal);
//             }
//         } 
//         else if (currentPosition.isClosed() && config.allowShortSelling) {
//             executeShortEntry(signal);
//         } 
//         else if (currentPosition.isShort() && config.allowPyramiding) {
//             executeAddToPosition(signal);
//         }
//     }

//     // for HOLD do nothinggg

//     updateCapitalHistory(candle.close);

//     currentBarIndex++;
// }

// void Backtester::executeEntry(const Signal &signal){
//     double quantity = calculatePositionSize(signal.price);
//     // std::cout<<quantity<<"\n";
//     if(quantity <= 0){
//         // std::cout<<"yoo\n";
//         return;
//     } 

//     double tradeValue = quantity * signal.price;
//     double transactionCost = calculateTransactionCost(tradeValue);

//     if(currentCapital < tradeValue + transactionCost){
//         // std::cout<<"yoo\n";
//         return;  
//     } 

//     // std::cout<<"yoo\n";
//     try{
//         if(signal.isBuy()){
//             std::cout<<signal.timestamp<<"\n";
//             currentPosition.openLong(signal.price, quantity, signal.timestamp, currentBarIndex);
//         }

//         currentCapital -= (tradeValue + transactionCost);
//     }
//     catch(const std::exception &e){
//         std::cout<<"Error in executeEntry"<<std::endl;
//         return;
//     }
//     // std::cout<<"Open: "<<"\n";
// }

// void Backtester::executeExit(const Signal &signal){
//     if(!currentPosition.isOpen()) return;

    
//     double tradeValue = currentPosition.getQuantity() * signal.price;
//     double transactionCost = calculateTransactionCost(tradeValue);
    
//     Trade trade = Trade::createFromPosition(currentPosition, signal.price, signal.timestamp, currentBarIndex);
    
//     currentCapital += (tradeValue - transactionCost);
    
//     trades.push_back(trade);
    
//     currentPosition.close();
//     // std::cout<<"Close: "<<"\n";
// }

// void Backtester::executeShortEntry(const Signal& signal) {
//     if (!config.allowShortSelling) {
//         return;
//     }
    
//     double quantity = calculatePositionSize(signal.price);
//     if (quantity <= 0) return;
    
//     double tradeValue = quantity * signal.price;
//     double transactionCost = calculateTransactionCost(tradeValue);
    
//     if (currentCapital < transactionCost) {
//         return; 
//     }
    
//     try {
//         currentPosition.openShort(signal.price, quantity, signal.timestamp, currentBarIndex);
//         currentCapital -= transactionCost;
//     } 
//     catch (const std::exception& e) {
//         return;
//     }
// }

// void Backtester::executeAddToPosition(const Signal& signal) {
//     if (!config.allowPyramiding || !canAddToPosition()) {
//         return;
//     }
    
//     double quantity = calculatePositionSize(signal.price);
//     if (quantity <= 0) return;
    
//     double tradeValue = quantity * signal.price;
//     double transactionCost = calculateTransactionCost(tradeValue);
    
//     if (currentCapital < tradeValue + transactionCost) {
//         return; 
//     }
    
//     try {
//         if (currentPosition.isLong() && signal.isBuy()) {
//             currentPosition.addToLong(signal.price, quantity);
//             currentCapital -= (tradeValue + transactionCost);
//         } 
//         else if (currentPosition.isShort() && signal.isSell()) {
//             currentPosition.addToShort(signal.price, quantity);
//             currentCapital -= transactionCost; 
//         }
//     } 
//     catch (const std::exception& e) {
//         return;
//     }
// }

// void Backtester::executePartialExit(const Signal& signal) {
//     if (!config.allowPartialSells || !currentPosition.isOpen()) {
//         return;
//     }
    
//     double partialQuantity = currentPosition.getQuantity() / 3.0;
    
//     if (partialQuantity <= 0) return;
    
//     double tradeValue = partialQuantity * signal.price;
//     double transactionCost = calculateTransactionCost(tradeValue);
    
//     Trade trade = Trade::createPartialFromPosition(currentPosition, signal.price, signal.timestamp, currentBarIndex, partialQuantity);
    
//     if (currentPosition.isLong()) {
//         currentCapital += (tradeValue - transactionCost);
//     } 
//     else {
//         currentCapital += (currentPosition.getEntryPrice() - signal.price) * partialQuantity - transactionCost;
//     }
    
//     trades.push_back(trade);
    
//     currentPosition.reducePosition(partialQuantity);
// }

// bool Backtester::canAddToPosition() const {
//     if (!currentPosition.isOpen()) {
//         return false;
//     }
    
//     double baseSize = calculatePositionSize(currentPosition.getEntryPrice());
//     double currentSize = currentPosition.getQuantity();
//     double maxAllowedSize = baseSize * config.maxPositionSize;
    
//     return currentSize < maxAllowedSize;
// }

// void Backtester::updateCapitalHistory(double currentPrice){
//     double currentEquity = currentCapital;

//     if(currentPosition.isOpen()){
//         currentEquity += currentPosition.getUnrealisedPnL(currentPrice);
//     }

//     capitalHistory.push_back(currentCapital);
//     equityHistory.push_back(currentEquity);
// }

// double Backtester::calculatePositionSize(double price) const {
//     if(config.sizingMethod == PositionSizingMethod::FIXED_AMOUNT){
//         return Position::calculateQuantityFixedAmount(config.positionSize, price);
//     }
//     else{
//         return Position::calculateQuantity(currentCapital, price, config.positionSize);
//     }
// }

// double Backtester::calculateTransactionCost(double tradeValue) const{
//     double fixedCost =config.transactionCost;
//     double percentageCost = tradeValue * config.transactionCostRate;
//     return fixedCost + percentageCost;
// }

// void Backtester::finalise(){
//     // if(currentPosition.isOpen() && !capitalHistory.empty()){
//     //     // leave for now
//     //     return;
//     // }
//     if(currentPosition.isOpen() && !capitalHistory.empty()){
//         Signal exitSignal(SignalType::SELL, lastPrice, timestamps.empty() ? "" : timestamps.back());
        
//         executeExit(exitSignal);
        
//         if(!capitalHistory.empty()){
//             capitalHistory.back() = currentCapital;
//         }

//         return;
//     }
    
//     if(!capitalHistory.empty()){
//         capitalHistory.back() = currentCapital;
//     }
// }

// void Backtester::reset(){
//     currentCapital = config.initialCapital;
//     currentPosition.close();
//     trades.clear();
//     capitalHistory.clear();
//     equityHistory.clear();
//     timestamps.clear();
//     currentBarIndex = 0;
    
//     if(strategy) strategy -> reset();

//     capitalHistory.push_back(currentCapital);
//     equityHistory.push_back(currentCapital);
// }

// void Backtester::setStrategy(std::unique_ptr<IStrategy> newStrat){
//     if(!newStrat){
//         throw std::runtime_error("Strategy cannot be null");
//     }
//     strategy = std::move(newStrat);
// }

// void Backtester::setConfig(const BacktestConfig &newConfig){
//     config = newConfig;
// }

// double Backtester::getTotalReturn() const{
//     if(equityHistory.empty()) return 0.0;

//     double finalEquity = equityHistory.back();
//     return finalEquity - config.initialCapital;
// }

// double Backtester::getTotalReturnPercent() const{
//     if(config.initialCapital == 0.0 || equityHistory.empty()) return 0.0;

//     double finalEquity = equityHistory.back();
//     return ((finalEquity - config.initialCapital)/ config.initialCapital) * 100.0;
// }




















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
    
    // std::cout<<trade.getEntryTime()<<" "<<trade.getEntryPrice()<<" "<<trade.getExitTime()<<" "<<trade.getExitPrice()<<" "<<currentCapital<<" "<<currentPosition.getQuantity()<<"\n";

    currentCapital += (tradeValue - transactionCost);
    
    trades.push_back(trade);
    
    // std::cout<<currentPosition.

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






double Backtester::getSharpeRatio() const {
    if (equityHistory.size() < 2) return 0.0;
    
    // Calculate daily returns
    std::vector<double> returns;
    for (size_t i = 1; i < equityHistory.size(); ++i) {
        double dailyReturn = (equityHistory[i] - equityHistory[i-1]) / equityHistory[i-1];
        returns.push_back(dailyReturn);
    }
    
    if (returns.empty()) return 0.0;
    
    // Calculate mean return
    double meanReturn = 0.0;
    for (double ret : returns) {
        meanReturn += ret;
    }
    meanReturn /= returns.size();
    
    // Calculate standard deviation
    double variance = 0.0;
    for (double ret : returns) {
        variance += (ret - meanReturn) * (ret - meanReturn);
    }
    variance /= returns.size();
    double stdDev = std::sqrt(variance);
    
    // Sharpe ratio (assuming risk-free rate = 0)
    return (stdDev == 0.0) ? 0.0 : (meanReturn / stdDev) * std::sqrt(252.0 * 390.0); // 252 days * 390 minute of trading per day
}

double Backtester::getMaxDrawdown() const {
    if (equityHistory.empty()) return 0.0;
    
    double maxDrawdown = 0.0;
    double peak = equityHistory[0];
    
    for (double equity : equityHistory) {
        if (equity > peak) {
            peak = equity;
        }
        
        double drawdown = (peak - equity) / peak * 100.0; // Percentage drawdown
        if (drawdown > maxDrawdown) {
            maxDrawdown = drawdown;
        }
    }
    
    return maxDrawdown;
}

double Backtester::getWinRate() const {
    if (trades.empty()) return 0.0;
    
    int winningTrades = 0;
    for ( Trade trade : trades) {
        trade.calculateMetrics();

        if (trade.getPnL() > 0) {
            winningTrades++;
        }
    }
    
    return (static_cast<double>(winningTrades) / trades.size()) * 100.0;
}

double Backtester::getProfitFactor() const {
    if (trades.empty()) return 0.0;
    
    double totalProfit = 0.0;
    double totalLoss = 0.0;
    
    for (Trade trade : trades) {
        trade.calculateMetrics();
        double pnl = trade.getPnL();
        if (pnl > 0) {
            totalProfit += pnl;
        } else {
            totalLoss += std::abs(pnl);
        }
    }
    
    return (totalLoss == 0.0) ? (totalProfit > 0 ? std::numeric_limits<double>::infinity() : 0.0) 
                              : (totalProfit / totalLoss);
}

double Backtester::getAvgTradeDuration() const {
    if (trades.empty()) return 0.0;
    
    int totalDuration = 0;
    for (const Trade& trade : trades) {
        // totalDuration += trade.getDuration();
        totalDuration += trade.getDurationBars();
        
    }
    
    return static_cast<double>(totalDuration) / trades.size();
}

double Backtester::getAvgTradeReturn() const {
    if (trades.empty()) return 0.0;
    
    double totalReturn = 0.0;
    for (Trade trade : trades) {
        trade.calculateMetrics();
        totalReturn += trade.getPnL();
    }
    
    return totalReturn / trades.size();
}

double Backtester::getAvgTradeReturnPercent() const {
    if (trades.empty()) return 0.0;
    
    double totalReturnPercent = 0.0;
    for ( Trade trade : trades) {
        trade.calculateMetrics();
        double returnPercent = (trade.getPnL() / (trade.getEntryPrice() * trade.getQuantity())) * 100.0;
        totalReturnPercent += returnPercent;
    }
    
    return totalReturnPercent / trades.size();
}

double Backtester::getMaxConsecutiveLosses() const {
    if (trades.empty()) return 0.0;
    
    int maxLosses = 0;
    int currentLosses = 0;
    
    for (Trade trade : trades) {
        trade.calculateMetrics();
        if (trade.getPnL() < 0) {
            currentLosses++;
            maxLosses = std::max(maxLosses, currentLosses);
        } else {
            currentLosses = 0;
        }
    }
    
    return maxLosses;
}

double Backtester::getMaxConsecutiveWins() const {
    if (trades.empty()) return 0.0;
    
    int maxWins = 0;
    int currentWins = 0;
    
    for (Trade trade : trades) {
        trade.calculateMetrics();
        if (trade.getPnL() > 0) {
            currentWins++;
            maxWins = std::max(maxWins, currentWins);
        } else {
            currentWins = 0;
        }
    }
    
    return maxWins;
}

double Backtester::getLargestWin() const {
    if (trades.empty()) return 0.0;
    
    double largestWin = 0.0;
    for (Trade trade : trades) {
        trade.calculateMetrics();
        if (trade.getPnL() > largestWin) {
            largestWin = trade.getPnL();
        }
    }
    
    return largestWin;
}

double Backtester::getLargestLoss() const {
    if (trades.empty()) return 0.0;
    
    double largestLoss = 0.0;
    for (Trade trade : trades) {
        trade.calculateMetrics();
        if (trade.getPnL() < largestLoss) {
            largestLoss = trade.getPnL();
        }
    }
    
    return largestLoss;
}