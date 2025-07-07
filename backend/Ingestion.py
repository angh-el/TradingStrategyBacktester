import requests
import pandas as pd
import time
from datetime import datetime

import os
from dotenv import load_dotenv

class Ingestion:
    def __init__(self, api_key: str, secret_key: str):
        self.base_url = "https://data.alpaca.markets/v2/stocks/bars"
        self.headers = {
            "accept": "application/json",
            "APCA-API-KEY-ID": api_key,
            "APCA-API-SECRET-KEY": secret_key
        }

    def fetch_data(self, symbol: str, start_date: str, limit=50000):
        all_bars = []
        page_token = None
        total_fetched = 0

        while total_fetched < limit:
            params = {
                "symbols": symbol,
                "timeframe": "1Min",
                "start": start_date,
                "limit": min(10000, limit - total_fetched),
                "adjustment": "raw",
                "feed": "sip",
                "sort": "asc"
            }
            if page_token:
                params["page_token"] = page_token

            response = requests.get(self.base_url, headers=self.headers, params=params)
            if response.status_code != 200:
                raise Exception(f"API call failed: {response.status_code} - {response.text}")

            data = response.json()
            bars = data.get("bars", {}).get(symbol, [])

            if not bars:
                print("No more data available.")
                break

            all_bars.extend(bars)
            total_fetched += len(bars)
            print(f"Fetched {total_fetched} bars so far...")

            page_token = data.get("next_page_token")
            if not page_token:
                break  

            time.sleep(0.5)  # sleep a bit

        return all_bars

    def save_to_csv(self, bars, filename: str):
        """
        Save the bars to a CSV in the format: date, open, highest, lowest, close, volume
        """
        if not bars:
            print("No data to save.")
            return

        df = pd.DataFrame([{
            "date": b["t"],
            "open": b["o"],
            "highest": b["h"],
            "lowest": b["l"],
            "close": b["c"],
            "volume": b["v"]
        } for b in bars])

        df["date"] = pd.to_datetime(df["date"])
        df.sort_values("date", inplace=True)

        df.to_csv(filename, index=False)
        print(f"Saved {len(df)} rows to {filename}")



if __name__ == "__main__":
    load_dotenv()

    API_KEY = os.getenv("ALPACA_API")
    SECRET_KEY = os.getenv("ALPACA_SECRET")

    ingestion = Ingestion(API_KEY, SECRET_KEY)
    bars = ingestion.fetch_data("AAPL", "2024-01-01")
    ingestion.save_to_csv(bars, "../data/aapl_1min.csv")
