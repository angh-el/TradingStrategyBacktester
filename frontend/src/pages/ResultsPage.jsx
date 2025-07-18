// import React from "react";

// export default function ResultsPage() {
//   return <div style={{ padding: "2rem" }}>Results will go here...</div>;
// }







import React, { useEffect, useState } from "react";
import { useLocation } from "react-router-dom";
import axios from "axios";

export default function ResultsPage() {
  const { state } = useLocation(); // payload passed from configure page
  const [result, setResult] = useState(null);
  const [error, setError] = useState(null);

  useEffect(() => {
    const runBacktest = async () => {
      try {
        const response = await axios.post("http://localhost:5000/api/run-backtest", state);
        if (response.data.status === "ok") {
          setResult(response.data.data);
        } else {
          setError(response.data.message || "Unknown error");
        }
      } catch (err) {
        setError(err.message);
      }
    };

    if (state) runBacktest();
  }, [state]);

  if (error) return <div>Error: {error}</div>;
  if (!result) return <div>Running backtest...</div>;

  return (
    <div style={{ padding: "2rem", fontFamily: "sans-serif" }}>
      <h2>Backtest Results</h2>

      <h3>Indicators</h3>
      <pre>{JSON.stringify(result.indicators, null, 2)}</pre>

      <h3>Metrics</h3>
      <pre>{JSON.stringify(result.metrics, null, 2)}</pre>

      <h3>Trades</h3>
      <pre>{JSON.stringify(result.trades, null, 2)}</pre>
    </div>
  );
}
