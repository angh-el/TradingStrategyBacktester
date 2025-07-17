import React, { useState } from "react";
import { useNavigate } from "react-router-dom";
import DatePicker from "react-datepicker";
import "react-datepicker/dist/react-datepicker.css";
import axios from "axios";

const strategies = [
  "Simple Moving Average Crossover",
  "Bollinger Band Mean Reversion",
  "Stochastic Oscillator Crossover",
  "Relative Strength Index Mean Reversion",
];

export default function ConfigurePage() {
  const [ticker, setTicker] = useState("");
  const [startDate, setStartDate] = useState(null);
  const [endDate, setEndDate] = useState(null);
  const [strategy, setStrategy] = useState(strategies[0]);
  const navigate = useNavigate();

  const handleSubmit = async () => {
    const payload = {
      ticker,
      start_date: startDate?.toISOString().split("T")[0],
      end_date: endDate?.toISOString().split("T")[0],
      strategy,
    };

    try {
      await axios.post("http://localhost:5000/api/run-backtest", payload);
      navigate("/results");
    } catch (err) {
      alert("Error running backtest");
    }
  };

  return (
    <div style={{ maxWidth: 500, margin: "2rem auto", fontFamily: "sans-serif" }}>
      <h1>Trading Strategy Backtester</h1>
      <h2>Configure</h2>

      <label>Ticker</label>
      <input
        type="text"
        value={ticker}
        onChange={(e) => setTicker(e.target.value)}
        style={{ width: "100%", marginBottom: "1rem" }}
      />

      <label>Start Date</label>
      <DatePicker
        selected={startDate}
        onChange={(date) => setStartDate(date)}
        dateFormat="yyyy-MM-dd"
        className="date-picker"
      />

      <label>End Date</label>
      <DatePicker
        selected={endDate}
        onChange={(date) => setEndDate(date)}
        dateFormat="yyyy-MM-dd"
        className="date-picker"
      />

      <label>Strategy</label>
      <select
        value={strategy}
        onChange={(e) => setStrategy(e.target.value)}
        style={{ width: "100%", marginBottom: "1rem" }}
      >
        {strategies.map((s) => (
          <option key={s}>{s}</option>
        ))}
      </select>

      <button onClick={handleSubmit}>Run Backtest</button>
    </div>
  );
}
