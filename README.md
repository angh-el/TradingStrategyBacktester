# TradingStrategyBacktester
A high-performance trading strategy backtester designed for rapid analysis of algorithmic trading strategies on large data sets. The backtesting engine is capable of processing up to 50,000 1-minute OHLCV data points with minimal latency. The system combines the ease of web-based interaction with the raw computational power needed for quantitive analysis.

## ✨ Key Features
- Core engine built in C++ optimised specifically for high-frequency data processing: supports up to 50,000 1-minute OHLCV bars seamlessly 
- Wide range of technical indicators(Simple Moving Average, Bollinger Bands, Relative Strength Index, Stochastic %K and %D, Average Directional Index) computed for offering a comprehensive overview of the price movement of the asset
- Several sample trading strategies (SMA mean crossover, Bollinger Band mean reversion, Stochastic Oscillator crossover, RSI mean reversion) to exemplify the
- Interactive frontend that allows users to choose the ticker symbol, start and end date for backtesting and strategy of choice before plotting highly detailed graphs showcasing the different indicators and providing metrics to quantify the successfulness of the strategy

## 🚀 Project Highlights
**Configuration settings**

<img width="825" height="707" alt="image" src="https://github.com/user-attachments/assets/f66246eb-94e9-407a-acec-ce9dcc557dda" />

---
**Graphical output**

<img width="1477" height="711" alt="image" src="https://github.com/user-attachments/assets/9fad8faf-6da1-4c2d-9851-6cf84679e255" />

---
**Metrics and trades logs**

<img width="1858" height="542" alt="image" src="https://github.com/user-attachments/assets/a0000830-1493-477a-a846-867a1129e64f" />

---

## 🛠️ Stack used
- **React and Plotly.js** - Frontend UI
- **Flask** - Python backend
- **Alpha Vantage and Alapaca** - API requests
- **C++** - Core backtesting logic
- **Docker** - Containerisation



## 💻 Usage 
```bash
docker-compose up --build
```
Then open your browser at http://localhost:3000


