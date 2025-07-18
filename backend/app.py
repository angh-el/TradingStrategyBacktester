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
    try:
        equity_df = pd.read_csv("../data/indicators.csv")
        trades_df = pd.read_csv("../data/metrics.csv")
        metrics_df = pd.read_csv("../data/trades.csv")

        return {
            "equity": equity_df.to_dict(orient="records"),
            "trades": trades_df.to_dict(orient="records"),
            "metrics": metrics_df.to_dict(orient="records")
        }
    except Exception as e:
        raise RuntimeError(f"Failed to read CSV files: {e}")

@app.route("/api/run-backtest", methods=["POST"])
def handle_backtest():
    data = request.get_json()

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
