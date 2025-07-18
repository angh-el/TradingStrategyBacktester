import subprocess

def makeCore():
    subprocess.run(["../core/make"])

def callCore(strategy):
    strategy_map = {
        "Simple Moving Average Crossover": "sma",
        "Bollinger Band Mean Reversion": "bollinger",
        "Stochastic Oscillator Crossover": "stochastic",
        "Relative Strength Index Mean Reversion": "rsi"
    }

    arg = strategy_map.get(strategy)
    if not arg:
        print("Invalid strategy provided.")
        return False

    try:
        print("Running ",arg,"...")
        result = subprocess.run(["../core/main", arg], check=True)
        return result.returncode == 0  
    except subprocess.CalledProcessError as e:
        print(f"Subprocess failed: {e}")
        return False
    except FileNotFoundError as e:
        print(f"Executable not found: {e}")
        return False
