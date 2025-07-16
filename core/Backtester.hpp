#ifndef BACKTESTER_HPP
#define BACKTESTER_HPP


#include "strategies/IStrategy.hpp"
#include "broker/Position.hpp"
#include "broker/Trade.hpp"
#include "indicators/IIndicator.hpp"

#include <vector>
#include <memory>

enum class PositionSizingMethod{
    FIXED_AMOUNT,       // fixed amount per trade
    PERCENTAGE_CAPITAL  // percentage of current capital
};

struct BacktestConfig{
    double initialCapital;
    PositionSizingMethod sizingMethod;
    double positionSize;                // either fixed amount or percentage
    double transactionCost;             // per trade cost
    double transactionCostRate;
    bool allowShortSelling;
    bool allowPyramiding;
    bool allowPartialSells;
    double maxPositionSize;

    BacktestConfig(double capital = 10000.0)
        : initialCapital(capital), sizingMethod(PositionSizingMethod::PERCENTAGE_CAPITAL), positionSize(1.0), transactionCost(0.0), transactionCostRate(0.0), allowShortSelling(false), allowPyramiding(true), allowPartialSells(true), maxPositionSize(3.0){}
};

class Backtester{
private:
    std::unique_ptr<IStrategy> strategy;
    BacktestConfig config;
    Position currentPosition;
    std::vector<Trade> trades;

    double currentCapital;
    double lastPrice;
    std::vector<double> capitalHistory; 
    std::vector<double> equityHistory;      // capital + unrealised pnl

    int currentBarIndex;
    std::vector<std::string> timestamps;

    double calculateTransactionCost(double tradeValue) const;
    double calculatePositionSize(double price) const;
    bool canAddToPosition() const;
    void executeEntry(const Signal &signal);
    void executeShortEntry(const Signal &signal);
    void executeAddToPosition(const Signal &signal);
    void executeExit(const Signal &signal);
    void executePartialExit(const Signal &signal);
    void updateCapitalHistory(double currentPrice);

public:
    Backtester(std::unique_ptr<IStrategy> strat, const BacktestConfig &cfg = BacktestConfig());

    void processCandle(const Candle &candle, const IndicatorManager &indicators);
    void finalise();

    void reset();
    void setStrategy(std::unique_ptr<IStrategy> newStrategy);

    const std::vector<Trade> &getTrades() const {return trades;}
    const std::vector<double> &getCapitalHistory() const {return capitalHistory;}
    const std::vector<double> &getEquityHistory() const {return equityHistory;}
    const Position &getCurrentPosition() const {return currentPosition;}

    // metrics getters
    double getFinalCapital() const {return currentCapital;}
    double getTotalReturn() const;
    double getTotalReturnPercent() const;
    int getNumberOfTrades() const {return trades.size();}
    double getSharpeRatio() const;
    double getMaxDrawdown() const;
    double getWinRate() const;
    double getProfitFactor() const;
    double getAvgTradeDuration() const;
    double getAvgTradeReturn() const;
    double getAvgTradeReturnPercent() const;
    double getMaxConsecutiveLosses() const;
    double getMaxConsecutiveWins() const;
    double getLargestWin() const;
    double getLargestLoss() const;

    std::string getStrategyName() const {return strategy->getName();}

    const BacktestConfig &getConfig() const {return config;}
    void setConfig (const BacktestConfig &newConfig);
};

#endif