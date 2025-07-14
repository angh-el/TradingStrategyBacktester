#include "Broker.hpp"

Broker::Broker(double initialCapital) : capital(initialCapital) {}

double Broker::calculateQty(double price) const {
    double availableCapital = capital * allocationPct;
    if (price <= 0) return 0;
    return (availableCapital - commission) / price;
}

void Broker::executeTrade(const Signal& signal, const Candle& candle, Metrics& metrics) {
    if (signal.type == SignalType::BUY && !inPosition) {
        double qty = calculateQty(candle.close);
        if (qty <= 0) return;

        entryPrice = candle.close;
        position = qty;
        inPosition = true;
        capital -= (qty * entryPrice + commission);

        metrics.logEntry(candle.date, entryPrice, qty);

    } else if (signal.type == SignalType::SELL && inPosition) {
        double exitPrice = candle.close;
        capital += (position * exitPrice - commission);

        metrics.logExit(candle.date, exitPrice, position);

        position = 0;
        inPosition = false;
    }
}

bool Broker::isInPosition() const {
    return inPosition;
}

double Broker::getCapital() const {
    return capital;
}


void Metrics::logEntry(const std::string& time, double price, double qty) {
    current = {};
    current.entryTime = time;
    current.entryPrice = price;
    current.quantity = qty;
    current.duration = 0;
}

void Metrics::logExit(const std::string& time, double price, double qty) {
    current.exitTime = time;
    current.exitPrice = price;
    current.pnl = (price - current.entryPrice) * qty;
    trades.push_back(current);
}

void Metrics::computeMetrics() {
    double totalPnl = 0.0;
    int wins = 0, total = 0;
    double maxDrawdown = 0.0;
    double equity = 0.0, peak = 0.0;

    for (const auto& t : trades) {
        totalPnl += t.pnl;
        if (t.pnl > 0) wins++;
        equity += t.pnl;
        peak = std::max(peak, equity);
        maxDrawdown = std::max(maxDrawdown, peak - equity);
    }

    std::cout << "Total Return: " << totalPnl << "\n";
    std::cout << "Win Rate: " << (total > 0 ? (double)wins / trades.size() : 0) << "\n";
    std::cout << "Max Drawdown: " << maxDrawdown << "\n";
    std::cout << "Number of Trades: " << trades.size() << "\n";
}
