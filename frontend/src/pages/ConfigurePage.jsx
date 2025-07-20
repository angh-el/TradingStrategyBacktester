// import React, { useState } from "react";
// import { useNavigate } from "react-router-dom";
// import DatePicker from "react-datepicker";
// import "react-datepicker/dist/react-datepicker.css";
// import axios from "axios";

// const strategies = [
//   "Simple Moving Average Crossover",
//   "Bollinger Band Mean Reversion",
//   "Stochastic Oscillator Crossover",
//   "Relative Strength Index Mean Reversion",
//   "My Strategy",
// ];

// export default function ConfigurePage() {
//   const [ticker, setTicker] = useState("");
//   const [startDate, setStartDate] = useState(null);
//   const [endDate, setEndDate] = useState(null);
//   const [strategy, setStrategy] = useState(strategies[0]);
//   const navigate = useNavigate();

//   const handleSubmit = async () => {
//     const payload = {
//       ticker,
//       start_date: startDate?.toISOString().split("T")[0],
//       end_date: endDate?.toISOString().split("T")[0],
//       strategy,
//     };

//     // try {
//     //   await axios.post("http://localhost:5000/api/run-backtest", payload);
//     //   navigate("/results");
//     // } catch (err) {
//     //   alert("Error running backtest");
//     // }

//     try {
//         navigate("/results", { state: payload }); 
//     } catch (err) {
//         alert("Error running backtest");
//     }

//   };

//   return (
//     <div style={{ maxWidth: 500, margin: "2rem auto", fontFamily: "sans-serif" }}>
//       <h1>Trading Strategy Backtester</h1>
//       <h2>Configure</h2>

//       <label>Ticker</label>
//       <input
//         type="text"
//         value={ticker}
//         onChange={(e) => setTicker(e.target.value)}
//         style={{ width: "100%", marginBottom: "1rem" }}
//       />

//       <label>Start Date</label>
//       <DatePicker
//         selected={startDate}
//         onChange={(date) => setStartDate(date)}
//         dateFormat="yyyy-MM-dd"
//         className="date-picker"
//       />

//       <label>End Date</label>
//       <DatePicker
//         selected={endDate}
//         onChange={(date) => setEndDate(date)}
//         dateFormat="yyyy-MM-dd"
//         className="date-picker"
//       />

//       <label>Strategy</label>
//       <select
//         value={strategy}
//         onChange={(e) => setStrategy(e.target.value)}
//         style={{ width: "100%", marginBottom: "1rem" }}
//       >
//         {strategies.map((s) => (
//           <option key={s}>{s}</option>
//         ))}
//       </select>

//       <button onClick={handleSubmit}>Run Backtest</button>
//     </div>
//   );
// }

















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
  "My Strategy",
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
      navigate("/results", { state: payload });
    } catch (err) {
      alert("Error running backtest");
    }
  };

  return (
    <div style={{
      maxWidth: 500,
      margin: "2rem auto",
      padding: "2rem",
      fontFamily: "Arial, sans-serif",
      border: "1px solid #ddd",
      borderRadius: "8px",
      backgroundColor: "#fafafa"
    }}>
      <h1 style={{ textAlign: "center", marginBottom: "1.5rem" }}>Trading Strategy Backtester</h1>
      <h2 style={{ marginBottom: "1rem" }}>Configure</h2>

      <div style={{ marginBottom: "1rem" }}>
        <label style={{ display: "block", marginBottom: "0.3rem" }}>Ticker</label>
        <input
          type="text"
          value={ticker}
          onChange={(e) => setTicker(e.target.value)}
          style={{ width: "100%", padding: "0.5rem", borderRadius: "4px", border: "1px solid #ccc" }}
        />
      </div>

      <div style={{ marginBottom: "1rem" }}>
        <label style={{ display: "block", marginBottom: "0.3rem" }}>Start Date</label>
        <DatePicker
          selected={startDate}
          onChange={(date) => setStartDate(date)}
          dateFormat="yyyy-MM-dd"
          className="date-picker"
          wrapperClassName="date-picker-wrapper"
          popperPlacement="bottom-start"
          style={{ width: "100%" }}
        />
      </div>

      <div style={{ marginBottom: "1rem" }}>
        <label style={{ display: "block", marginBottom: "0.3rem" }}>End Date</label>
        <DatePicker
          selected={endDate}
          onChange={(date) => setEndDate(date)}
          dateFormat="yyyy-MM-dd"
          className="date-picker"
          wrapperClassName="date-picker-wrapper"
          popperPlacement="bottom-start"
          style={{ width: "100%" }}
        />
      </div>

      <div style={{ marginBottom: "1rem" }}>
        <label style={{ display: "block", marginBottom: "0.3rem" }}>Strategy</label>
        <select
          value={strategy}
          onChange={(e) => setStrategy(e.target.value)}
          style={{ width: "100%", padding: "0.5rem", borderRadius: "4px", border: "1px solid #ccc" }}
        >
          {strategies.map((s) => (
            <option key={s}>{s}</option>
          ))}
        </select>
      </div>

      <button
        onClick={handleSubmit}
        style={{
          width: "100%",
          padding: "0.75rem",
          backgroundColor: "#007bff",
          color: "#fff",
          border: "none",
          borderRadius: "4px",
          cursor: "pointer",
          fontSize: "1rem"
        }}
      >
        Run Backtest
      </button>
    </div>
  );
}

