#include <vector>
#include <string>

#include <fstream>
#include <sstream>
#include <iostream>

struct Candle{
    std::string date;
    double open;
    double high;
    double low;
    double close;
    double volume;
};

void print_candles(std::vector<Candle> candles){
    for(int i = 0; i < candles.size(); ++i){
        std::cout<<"D: "<<candles.at(i).date;
        std::cout<<" O: "<<candles.at(i).open;
        std::cout<<" H: "<<candles.at(i).high;
        std::cout<<" L: "<<candles.at(i).low;
        std::cout<<" C: "<<candles.at(i).close;
        std::cout<<" V: "<<candles.at(i).volume;
        std::cout<<"\n";
    }
}

void parse_csv(const char *fp){
    std::vector<Candle> candles;
    std::ifstream file(fp);
    
    if(!file.is_open()){
        std::cerr<< "Failed to open file "<<fp<<std::endl;
        return;
    }

    std::string line;
    while(std::getline(file, line)){
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

        candles.push_back(candle);
    }

    // file.close();

    print_candles(candles);
    

    return;
}


int main(){

    const char * fp = "../data/aapl_1min.csv";
    parse_csv(fp);


    return 0;
}