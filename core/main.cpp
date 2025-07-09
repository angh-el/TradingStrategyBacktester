#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "MarketData.hpp"
#include "CSVManager.hpp"


int main(){
    const char * fp = "../data/aapl_1min.csv";
    MarketData mdata;

    CSVManager::parseCSV(&mdata, fp);
    // mdata.printCandles();

    return 0;
}