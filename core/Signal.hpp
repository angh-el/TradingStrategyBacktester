#ifndef SIGNAL_HPP
#define SIGNAL_HPP

enum class SignalType {BUY, SELL, HOLD};

struct Signal{
    SignalType type = SignalType::HOLD;
};
#endif




