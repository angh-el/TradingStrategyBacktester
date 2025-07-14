#include "Position.hpp"
#include <stdexcept>

Position::Position(): type(PositionType::NONE), entryPrice(0.0), quantity(0.0), entryTime(""), entryBarIndex(0){}

Position::Position(PositionType posType, double price, double qty, const std::string &time, int barIndex)
    : type(posType), entryPrice(price), quantity(qty), entryTime(time), entryBarIndex(barIndex){}

bool Position::isOpen() const{ return type!=PositionType::NONE;}

bool Position::isLong() const{ return type == PositionType::LONG;}

bool Position::isShort() const{ return type == PositionType::SHORT;}

bool Position::isClosed() const{ return type == PositionType::NONE;}

void Position::openLong(double price, double qty, const std::string &time, int barIndex){
    if(isOpen){
        throw std::runtime_error("Cannot open long positon: position already open");
    }

    type = PositionType::LONG;
    entryPrice = price;
    quantity = qty;
    entryTime = time;
    entryBarIndex = barIndex;
}

void Position::openShort(double price, double qty, const std::string &time, int barIndex){
    if(isOpen()){
        throw std::runtime_error("Cannot open short position: positon already open");
    }

    type = PositionType::SHORT;
    entryPrice = price;
    quantity = qty;
    entryTime = time;
    entryBarIndex = barIndex;
}

void Position::close(){
    type = PositionType::NONE;
    entryPrice = 0.0;
    quantity = 0.0;
    entryTime = "";
    entryBarIndex = 0;
}

double Position::getUnrealisedPnL(double currentPrice) const{
    if(!isOpen()) return 0.0;

    if(isLong()){
        // long pos - profit when price goes up
        return quantity * (currentPrice - entryPrice);
    }
    else{
        // short pos - profit when pricegoes dowsn
        return quantity * (entryPrice - currentPrice);
    }
}

double Position::getPositionValue(double currentPrice) const{
    if(!isOpen()) return 0.0;

    return quantity * currentPrice;
}

double Position::calculateQuantity(double capital, double price, double positionSizePercent){
    if(capital <= 0.0 || price <= 0.0 || positionSizePercent <= 0) return 0.0;

    double allocatedCapital = capital * positionSizePercent;
    return allocatedCapital / price;
}

double Position::calculateQuantityFixedAmount(double fixedAmount, double price){
    if(fixedAmount <= 0.0 || price <= 0.0) return 0.0;

    return fixedAmount / price;
}