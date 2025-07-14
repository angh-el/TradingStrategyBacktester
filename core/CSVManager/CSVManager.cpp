#include "CSVManager.hpp"


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
}