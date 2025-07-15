#ifndef TRADE_HPP
#define TRADE_HPP

#include "Position.hpp"
#include <string>
#include <stdexcept>
#include <iostream>

class Trade {
private:
    std::string entryTime;
    std::string exitTime;
    double entryPrice;
    double exitPrice;
    double quantity;
    PositionType tradeType;
    int entryBarIndex;
    int exitBarIndex;

    double pnl;
    double returnPercent;
    bool isWinning;

    

public:
    Trade(const std::string &entryT, const std::string &exitT, double entryP, double exitP, double qty,  PositionType type, int entryBar, int exitBar);

    std::string getEntryTime() const{return entryTime;}
    std::string getExitTime() const {return exitTime;}
    double getEntryPrice() const {return entryPrice;}
    double getExitPrice() const {return exitPrice;}
    double getQuantity() const {return quantity;}
    PositionType getTradeType() const {return tradeType;}
    int getEntryBarIndex() const {return entryBarIndex;}
    int getExitBarIndex() const {return exitBarIndex;}

    void calculateMetrics();
    double getPnL() const {return pnl;}
    double getReturnPercent() const {return returnPercent;}
    int getDurationBars() const {return exitBarIndex - entryBarIndex;}
    bool isWin() const {return isWinning;}
    bool isLoss() const {return !isWinning;}

    bool isLongTrade() const {return tradeType == PositionType::LONG;}
    bool isShortTrade() const {return tradeType == PositionType::SHORT;}

    static Trade createFromPosition(const Position &position, double exitPrice, const std::string &exitTime, int exitBarIndex);
    static Trade createPartialFromPosition(const Position &position, double exitPrice, const std::string &exitTime, int exitBarIndex, double partialQuantity);
};

#endif
