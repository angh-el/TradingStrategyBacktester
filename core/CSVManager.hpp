#ifndef CSVMANAGER_HPP
#define CSVMANAGER_HPP

#include "MarketData.hpp"

#include <vector>
#include <string>

#include <fstream>
#include <sstream>
#include <iostream>

namespace CSVManager{
    void parseCSV(MarketData *mdata, const char *fp);
}






#endif