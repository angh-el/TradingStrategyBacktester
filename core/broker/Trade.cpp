#include "Trade.hpp"
#include <cmath>

Trade::Trade(const std::string &entryT, const std::string &exitT, double entryP, double exitP, double qty,  PositionType type, int entryBar, int exitBar)
    : entryTime(entryT), exitTime(exitT), entryPrice(entryP), exitPrice(exitP), quantity(qty), tradeType(type), entryBarIndex(entryBar), exitBarIndex(exitBar){}

void Trade::calculateMetrics(){
    if(tradeType == PositionType::LONG){
        // long trade :. profit when exit price > entry price
        pnl = quantity * (exitPrice - entryPrice);
        returnPercent = ((exitPrice - entryPrice) / entryPrice) * 100.0;
    }
    else if(tradeType == PositionType::SHORT){
        // short trade :. profit when exit price < entry price
        pnl = quantity * (entryPrice - exitPrice);
        returnPercent = ((entryPrice - exitPrice) / entryPrice) * 100.0;
    }
    else{
        pnl = 0.0;
        returnPercent = 0.0;
    }

    // std::cout<<""
    isWinning = pnl > 0.0;
}

Trade Trade::createFromPosition(const Position &position, double exitPrice, const std::string &exitTime, int exitBarIndex){
    if(!position.isOpen()){
        throw std::runtime_error("Cannot create trade from closed position");
    }

    return Trade(position.getEntryTime(), exitTime, position.getEntryPrice(), exitPrice, position.getQuantity(), position.getType(), position.getEntryBarIndex(), exitBarIndex);
}

Trade Trade::createPartialFromPosition(const Position& position, double exitPrice, const std::string& exitTime, int exitBarIndex, double partialQuantity){
    if (!position.isOpen()) {
        throw std::runtime_error("Cannot create trade from closed position");
    }
    
    if (partialQuantity <= 0 || partialQuantity > position.getQuantity()) {
        throw std::runtime_error("Invalid partial quantity");
    }
    
    return Trade( position.getEntryTime(), exitTime, position.getEntryPrice(), exitPrice, partialQuantity, position.getType(),position.getEntryBarIndex(),exitBarIndex);
}