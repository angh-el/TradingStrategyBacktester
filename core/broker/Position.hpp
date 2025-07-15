#ifndef POSITION_HPP
#define POSITION_HPP

#include <string>

enum class PositionType{NONE, LONG, SHORT};

class Position{
private:
    PositionType type;
    double entryPrice;
    double quantity;
    std::string entryTime;
    int entryBarIndex;

public:
    Position();
    Position(PositionType posType, double price, double qty, const std::string &time, int barIndex);

    bool isOpen() const;
    bool isLong() const;
    bool isShort() const;
    bool isClosed() const;

    PositionType getType() const {return type;}
    double getEntryPrice() const {return entryPrice;}
    double getQuantity() const {return quantity;}
    std::string getEntryTime() const {return entryTime;}
    int getEntryBarIndex() const {return entryBarIndex;}

    void openLong(double price, double qty, const std::string &time, int barIndex);
    void openShort(double price, double qty, const std::string &time, int barIndex);
    void addToLong(double price, double qty);
    void addToShort(double price, double qty);
    void reducePosition(double qty);
    void close();

    double getUnrealisedPnL(double currentPrice) const;
    double getPositionValue(double currentPrice) const;
    double getAverageEntryPrice() const {return entryPrice;}

    static double calculateQuantity(double capital, double price, double positionSizePercent = 1.0);
    static double calculateQuantityFixedAmount(double fixedAmount, double price);

};

#endif