# CXX = g++
# TARGET = main

# # Source files
# SRCS = main.cpp \
#        csvManager/MarketData.cpp \
#        csvManager/CSVManager.cpp \
#        indicators/IndicatorManager.cpp \
#        strategies/SMACrossover.cpp \
#        broker/Position.cpp \
#        broker/Trade.cpp \
#        Backtester.cpp

# OBJS = $(SRCS:.cpp=.o)

# all: $(TARGET)

# $(TARGET): $(OBJS)
# 	$(CXX)  -o $@ $^

# clean:
# 	del /Q $(subst /,\,$(OBJS)) $(TARGET).exe 2>NUL

# rebuild: clean all

# run:
# 	./main











# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

# Output executable
TARGET = main

# Source files
SRCS = main.cpp \
       csvManager/MarketData.cpp \
       csvManager/CSVManager.cpp \
       indicators/IndicatorManager.cpp \
       strategies/SMACrossover.cpp \
       broker/Position.cpp \
       broker/Trade.cpp \
       Backtester.cpp \
       strategies/StochasticOscillatorCrossover.cpp \
       strategies/BollingerBandMeanReversion.cpp \
       strategies/RSIMeanReversion.cpp

# Object files (replace .cpp with .o)
OBJS = $(SRCS:.cpp=.o)

# Default rule
all: $(TARGET)

# Link object files into one executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile .cpp to .o (automatic dependency rule)
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean object files and binary
clean:
	-del /Q $(subst /,\,$(OBJS)) $(TARGET).exe 2>NUL

# Force rebuild
rebuild: clean all

# Run the compiled executable
run: all
	./$(TARGET).exe
