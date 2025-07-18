#include "CSVManager.hpp"

bool headerWritten = false;
std::string indicatorsPath = "../data/indicators.csv";

namespace CSVManager{
    void parseCSV(MarketData *mdata, const char *fp){
        // std::vector<Candle> candles;
        std::ifstream file(fp);
        
        if(!file.is_open()){
            std::cerr<< "Failed to open file "<<fp<<std::endl;
            return;
        }

        std::string line;
        while(std::getline(file, line)){
            // std::cout<<"hi";
            std::stringstream ss(line);
            std::string item;
            
            // this just skipps the first line in the .csv
            if(line == "date,open,highest,lowest,close,volume"){continue;}

            Candle candle;
            // read each col
            std::getline(ss, candle.date, ',');
            std::getline(ss, item, ','); candle.open = std::stod(item);
            std::getline(ss, item, ','); candle.high = std::stod(item);
            std::getline(ss, item, ','); candle.low = std::stod(item);
            std::getline(ss, item, ','); candle.close = std::stod(item);
            std::getline(ss, item, ','); candle.volume = std::stod(item);

            // candles.push_back(candle);
            (*mdata).addCandle(candle);
        }

        file.close();

        // print_candles(candles);
        

        return;
    }

    


    
    void writeHeaderIfNeeded() {
        if (headerWritten) return;

        std::ofstream out(indicatorsPath, std::ios::trunc);

        // Check if file already exists and is non-empty
        std::ifstream infile(indicatorsPath);
        bool fileExistsAndNotEmpty = infile.peek() != std::ifstream::traits_type::eof();
        infile.close();

        if (!fileExistsAndNotEmpty) {
            std::ofstream out(indicatorsPath, std::ios::app);
            // out << "Date,SMA,LowerBB,UpperBB,StochasticK,StochasticD,RSI,ADX\n";
            out.close();
        }

        headerWritten = true;
    }

    // function to log indicators to indicators.csv
    void logIndicators( const std::string& date, double sma, double lowerBB, double upperBB, double stochasticK, double stochasticD, double rsi,double adx) {
        writeHeaderIfNeeded();

        std::ofstream out(indicatorsPath, std::ios::app);
        if (!out.is_open()) {
            throw std::runtime_error("Failed to open file: " + indicatorsPath);
        }

        out << date << ","
            << sma << ","
            << lowerBB << ","
            << upperBB << ","
            << stochasticK << ","
            << stochasticD << ","
            << rsi << ","
            << adx << "\n";

        out.close();
    }


    void logTrades(const std::vector<Trade> trades) {
        std::ofstream out("../data/trades.csv", std::ios::trunc);  // truncate to start fresh

        if (!out.is_open()) {
            throw std::runtime_error("Failed to open trades.csv for writing.");
        }

        // Write header
        // out << "EntryTime,ExitTime,Side,EntryPrice,ExitPrice,PNL,Duration\n";


        for ( auto trade : trades) {
            trade.calculateMetrics();
            out << trade.getEntryTime() << ","
                << trade.getExitTime() << ","
                // << trade.getSide() << ","
                << trade.getEntryPrice() << ","
                << trade.getExitPrice() << ","
                << trade.getPnL() << ","
                << trade.getDurationBars() << "\n";
        }

        out.close();
    }


    void logMetrics(const Backtester& backtester) {
        std::ofstream out("../data/metrics.csv", std::ios::trunc);  

        if (!out.is_open()) {
            throw std::runtime_error("Failed to open matrics.csv for writing.");
        }

        out <<"10000"<<","
            <<backtester.getTotalReturnPercent()<<","
            <<backtester.getSharpeRatio()<<","
            <<backtester.getMaxDrawdown() <<","
            <<backtester.getWinRate() <<","
            <<backtester.getProfitFactor() <<","
            <<backtester.getAvgTradeReturn() <<","
            <<backtester.getNumberOfTrades() <<","
            <<backtester.getAvgTradeDuration()<<","
            <<backtester.getLargestWin() <<","
            <<backtester.getLargestLoss() <<","
            <<backtester.getMaxConsecutiveWins() <<","
            <<backtester.getMaxConsecutiveLosses() <<"\n";

        out.close();
    }

}