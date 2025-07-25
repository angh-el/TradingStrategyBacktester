from flask import Flask, request, jsonify
from flask_cors import CORS
from CoreWrapper import callCore
from Ingestion import Ingestion
import os
from dotenv import load_dotenv
import pandas as pd

app = Flask(__name__)
CORS(app)  # Allow requests from React frontend

def build_response():
    indicators = ["date", "sma", "lowerbb", "upperbb", "stochasticK", "stochasticD", "rsi", "adx"]
    trades = ["Entry time", "Exit time", "Entry price","Exit price","PnL","Duration"]
    metrics = ["Start capital", "Total return", "Sharpe ratio", "Max drawdown", "Win rate", "Profit factor", "Avg trade return", "Number of trades", "Avg trade duration", "Greatest win", "Greatest loss", "Max continous wins", "Max continous losses"]
    ohlcv = ["Open", "Highest", "Lowest", "Close", "Volume"]

    try:
        indicators_df = pd.read_csv("../data/indicators.csv", header=None, names=indicators)
        metrics_df = pd.read_csv("../data/metrics.csv", header=None, names=metrics)
        trades_df = pd.read_csv("../data/trades.csv", header=None, names=trades)
        ohlvc_df = pd.read_csv("../data/aapl_1min.csv")

        # print(ohlvc_df)

        return {
            "ohlcv":ohlvc_df.to_dict(orient="records"),
            "indicators": indicators_df.to_dict(orient="records"),
            "metrics": metrics_df.to_dict(orient="records"),
            "trades": trades_df.to_dict(orient="records")
        }
    except Exception as e:
        raise RuntimeError(f"Failed to read CSV files: {e}")

@app.route("/api/run-backtest", methods=["POST"])
def handle_backtest():
    data = request.get_json()
    print("REQUEST")

    # print("data: ",data["strategy"])
    try:
        # # get api key & secret
        # load_dotenv()
        # API_KEY = os.getenv("ALPACA_API")
        # SECRET_KEY = os.getenv("ALPACA_SECRET")
        # # instanciate the Ingestion class
        # ingestion = Ingestion(API_KEY, SECRET_KEY)
        # # call api function
        # bars = ingestion.fetch_data("AAPL", "2024-01-01")
        # # save everything to the .csv
        # ingestion.save_to_csv(bars, "../data/aapl_1min.csv")
        
        result = callCore(data["strategy"])
        if result:
            json_response = build_response()
            return jsonify({"status" : "ok", "data" : json_response})
        else:
            return jsonify({"status" : "error", "message": "Failure in the backtester"})
        
        # return jsonify({"status": "ok"}) 
        # return jsonify({"status": "ok", "result": result})
    except Exception as e:
        print("Error:", e)
        return jsonify({"status": "error", "message": str(e)}), 500

if __name__ == "__main__":
    app.run(debug=True)
