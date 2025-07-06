import requests

import os
from dotenv import load_dotenv

load_dotenv()

def get_data(ticker, start_date, end_date):
    API = os.getenv('ALPHA_VANTAGE_KEY')    
    url = 'https://www.alphavantage.co/query?function=TIME_SERIES_INTRADAY&symbol=AAPL&interval=1min&apikey=${API}'
    r = requests.get(url)
    data = r.json()

    print(data)
    
    return

get_data(1, 1, 1)