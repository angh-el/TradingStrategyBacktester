// import React from "react";

// export default function ResultsPage() {
//   return <div style={{ padding: "2rem" }}>Results will go here...</div>;
// }







// import React, { useEffect, useState } from "react";
// import { useLocation } from "react-router-dom";
// import axios from "axios";

// export default function ResultsPage() {
//   const { state } = useLocation(); // payload passed from configure page
//   const [result, setResult] = useState(null);
//   const [error, setError] = useState(null);

//   useEffect(() => {
//     const runBacktest = async () => {
//       try {
//         const response = await axios.post("http://localhost:5000/api/run-backtest", state);
//         if (response.data.status === "ok") {
//           setResult(response.data.data);
//         } else {
//           setError(response.data.message || "Unknown error");
//         }
//       } catch (err) {
//         setError(err.message);
//       }
//     };

//     if (state) runBacktest();
//   }, [state]);

//   if (error) return <div>Error: {error}</div>;
//   if (!result) return <div>Running backtest...</div>;

//   return (
//     <div style={{ padding: "2rem", fontFamily: "sans-serif" }}>
//       <h2>Backtest Results</h2>

//       <h3>OHLCV Data</h3>
//       <pre>{JSON.stringify(result.ohlcv, null, 2)}</pre>

//       {/* <h3>Indicators</h3>
//       <pre>{JSON.stringify(result.indicators, null, 2)}</pre>

//       <h3>Metrics</h3>
//       <pre>{JSON.stringify(result.metrics, null, 2)}</pre>

//       <h3>Trades</h3>
//       <pre>{JSON.stringify(result.trades, null, 2)}</pre> */}
//     </div>
//   );
// }
















// import React from "react";
// import { useLocation } from "react-router-dom";

// export default function ResultsPage() {
//   const { state } = useLocation(); // user inputs

//   return (
//     <div style={{ display: "flex", flexDirection: "column", padding: "1rem", fontFamily: "sans-serif" }}>
//       {/* Main Candlestick + SMA + Bollinger Chart */}
//       <div style={{ width: "100%", height: "500px", marginBottom: "2rem" }}>
//         <div style={{
//           border: "1px solid #ccc",
//           height: "100%",
//           display: "flex",
//           alignItems: "center",
//           justifyContent: "center"
//         }}>
//           <span>Main Price Chart (Candlestick + SMA + Bollinger)</span>
//         </div>
//       </div>

//       {/* Indicators Graph */}
//       <div style={{ width: "100%", height: "200px", marginBottom: "2rem" }}>
//         <div style={{
//           border: "1px solid #ccc",
//           height: "100%",
//           display: "flex",
//           alignItems: "center",
//           justifyContent: "center"
//         }}>
//           <span>Indicators Chart (RSI, %D, %K, ADX)</span>
//         </div>
//       </div>

//       {/* Bottom Panel: Metrics + Trades */}
//       <div style={{ display: "flex", gap: "1rem" }}>
//         {/* Metrics Panel */}
//         <div style={{ flex: 1, border: "1px solid #ccc", padding: "1rem" }}>
//           <h3>Final Metrics</h3>
//           <ul>
//             {[...Array(5)].map((_, i) => (
//               <li key={i}>Metric {i + 1}</li>
//             ))}
//           </ul>
//         </div>

//         {/* Trades Panel */}
//         <div style={{ flex: 2, border: "1px solid #ccc", padding: "1rem" }}>
//           <h3>Trades</h3>
//           <table style={{ width: "100%", borderCollapse: "collapse" }}>
//             <thead>
//               <tr>
//                 <th>Entry</th>
//                 <th>Exit</th>
//                 <th>Entry Price</th>
//                 <th>Exit Price</th>
//                 <th>PnL</th>
//                 <th>Duration</th>
//               </tr>
//             </thead>
//             <tbody>
//               {[...Array(3)].map((_, i) => (
//                 <tr key={i}>
//                   <td>2024-01-01</td>
//                   <td>2024-01-05</td>
//                   <td>100.0</td>
//                   <td>105.0</td>
//                   <td>5.0</td>
//                   <td>4d</td>
//                 </tr>
//               ))}
//             </tbody>
//           </table>
//         </div>
//       </div>
//     </div>
//   );
// }













import React, { useEffect, useState } from "react";
import { useLocation } from "react-router-dom";
import axios from "axios";
import Plot from "react-plotly.js";

export default function ResultsPage() {
  const { state } = useLocation();
  const [data, setData] = useState(null);
  const [error, setError] = useState(null);

  useEffect(() => {
    const runBacktest = async () => {
      try {
        const response = await axios.post("http://localhost:5000/api/run-backtest", state);
        if (response.data.status === "ok") {
          setData(response.data.data);
        } else {
          setError(response.data.message);
        }
      } catch (err) {
        setError(err.message);
      }
    };

    if (state) runBacktest();
  }, [state]);

  if (error) return <div>Error: {error}</div>;
  if (!data) return <div>Loading backtest...</div>;

  const { ohlcv, indicators, metrics } = data;
  // const {metrics} = data;
  const metricsMap = Array.isArray(metrics)
  ? metrics.reduce((acc, cur) => ({ ...acc, ...cur }), {})
  : metrics;




  const x = indicators.map((_,i) =>i);
  const dates = indicators.map((d) => d.date);
  const open = ohlcv.map((d) => d.open);
  const high = ohlcv.map((d) => d.highest);
  const low = ohlcv.map((d) => d.lowest);
  const close = ohlcv.map((d) => d.close);

  const sma = indicators.map((d) => d.sma);
  const upperbb = indicators.map((d) => d.upperbb);
  const lowerbb = indicators.map((d) => d.lowerbb);

  // Build lookup from date to index
  const dateToIndex = {};
  // ohlcv.forEach((d, i) => {
  //   dateToIndex[d.date] = i;
  // });

  // Trade markers
  const entryPoints = data.trades.map((trade) => ({
    x: trade["Entry time"],
    y: trade["Entry price"],
  }));

  const exitPoints = data.trades.map((trade) => ({
    x: trade["Exit time"],
    y: trade["Exit price"],
  }));

  const rsi = indicators.map((d) => d.rsi);
  const stochasticK = indicators.map((d) => d.stochasticK);
  const stochasticD = indicators.map((d) => d.stochasticD);
  const adx = indicators.map((d) => d.adx);


  return (
    <div style={{ padding: "1rem", fontFamily: "sans-serif" }}>
      <h2>Backtest Results</h2>

      {/* Main Price Chart */}
      <Plot
        data={[
          {
            x: dates,
            // x,
            open: open,
            high: high,
            low: low,
            close: close,
            type: "candlestick",
            name: "Price",
            // xaxis: "x",
            yaxis: "y",
          },
          {
            x: dates,
            // x,
            y: sma,
            type: "scatter",
            mode: "lines",
            name: "SMA",
            line: { color: "black", width: 1.5 },
            // xaxis: "x",
            yaxis: "y",
          },
          {
            x: dates,
            // x,
            y: upperbb,
            type: "scatter",
            mode: "lines",
            name: "Upper BB",
            line: { color: "green", width: 0.3, shape:"spline", smoothing:1.6},
            // xaxis: "x",
            yaxis: "y",
          },
          {
            x: dates,
            // x,
            y: lowerbb,
            type: "scatter",
            mode: "lines",
            name: "Lower BB",
            line: { color: "green", width: 0.3, shape:"spline", smoothing:1.6},
            // xaxis: "x",
            yaxis: "y",
          },
          {
            x: entryPoints.map((pt) => pt.x),
            y: entryPoints.map((pt) => pt.y - 1.5),
            type: "scatter",
            mode: "markers",
            name: "Entry",
            marker: {
              symbol: "triangle-up",
              size: 10,
              color: "green",
            },
          },
          {
            x: exitPoints.map((pt) => pt.x),
            y: exitPoints.map((pt) => pt.y + 1.5),
            type: "scatter",
            mode: "markers",
            name: "Exit",
            marker: {
              symbol: "triangle-down",
              size: 10,
              color: "red",
            },
          },

        ]}
        layout={{
          title: "Price with SMA and Bollinger Bands",
          height: 500,
          width: 1200,
          xaxis: { rangeslider: { visible: false } },
          yaxis: {  title: "Price", 
                    range:[178, 195],
          },
        }}
        // style={{ width: "100%" }}
      />

      {/* Indicators Chart */}
      <Plot
        data={[
          {
            x: dates,
            // x,
            y: rsi,
            type: "scatter",
            mode: "lines",
            name: "RSI",
            line: { color: "blue", width: 1 },
          },
          {
            x: dates,
            // x,
            y: stochasticK,
            type: "scatter",
            mode: "lines",
            name: "%K",
            line: { color: "red"},
          },
          {
            x: dates,
            // x,
            y: stochasticD,
            type: "scatter",
            mode: "lines",
            name: "%D",
            line: { color: "purple" },
          },
          {
            x: dates,
            // x,
            y: adx,
            type: "scatter",
            mode: "lines",
            name: "ADX",
            line: { color: "black" },
          },
        ]}
        layout={{
          title: "Technical Indicators",
          height: 250,
          width: 1200,
          xaxis: { title: "Date" },
          yaxis: { title: "Value" },
        }}
        style={{ width: "100%" }}
      />


      <div style={{ display: "flex", gap: "20px", marginTop: "20px" }}>
  {/* Trades Table */}
  <div style={{ flex: 2 }}>
    <h3>Trades</h3>
    <table style={{ width: "100%", borderCollapse: "collapse" }}>
      <thead>
        <tr>
          <th>Entry Time</th>
          <th>Exit Time</th>
          <th>Entry Price</th>
          <th>Exit Price</th>
          <th>PnL</th>
          <th>Duration</th>
        </tr>
      </thead>
      <tbody>
        {data.trades.map((trade, index) => (
          <tr key={index}>
            <td>{trade["Entry time"]}</td>
            <td>{trade["Exit time"]}</td>
            <td>{trade["Entry price"]}</td>
            <td>{trade["Exit price"]}</td>
            <td>
              <span style={{
              color: trade["PnL"] > 0 ? "green" :
                     trade["PnL"] < 0 ? "red" : "gray",
              fontWeight: "bold"
            }}>
              {trade["PnL"]}</span>
            </td>
            <td>{trade["Duration"]}</td>
          </tr>
        ))}
      </tbody>
    </table>
  </div>


{/* Metrics Panel */}
<div style={{
  flex: 1,
  border: "1px solid #ccc",
  padding: "10px",
  borderRadius: "8px",
  backgroundColor: "#f9f9f9",
  maxHeight: "400px",
  overflowY: "auto"
}}>
  <h3>Final Metrics</h3>
  {/* metrics = ["Start capital", "Total return", "Sharpe ratio", "Max drawdown", "Win rate", "Profit factor", "Avg trade return", "Number of trades", "Avg trade duration", "Greatest win", "Greatest loss", "Max continous wins", "Max continous losses"] */}
  <ul style={{ listStyle: "none", padding: 0 }}>
    <li><strong>Start capital: </strong>{metricsMap["Start capital"]}</li>
    <li><strong>Total return: </strong> {metricsMap["Total return"]}  <>%</></li>
    <li><strong>Sharpe Ratio: </strong> {metricsMap["Sharpe ratio"]}</li>
    <li><strong>Max drawdown: </strong> {metricsMap["Max drawdown"]}</li>
    <li><strong>Win Rate: </strong> {metricsMap["Win rate"]} <>%</></li>
    <li><strong>Profit factor: </strong> {metricsMap["Profit factor"]}</li>
    <li><strong>Avg trade return: </strong> {metricsMap["Avg trade return"]}</li>
    <li><strong>Number of trades: </strong> {metricsMap["Number of trades"]}</li>
    <li><strong>Avg trade duration:</strong> {metricsMap["Avg trade duration"]}</li>
    <li><strong>Greatest win:</strong> {metricsMap["Greatest win"]}</li>
    <li><strong>Greatest loss:</strong> {metricsMap["Greatest loss"]}</li>
    <li><strong>Max continous wins:</strong> {metricsMap["Max continous wins"]}</li>
    <li><strong>Max continous losses:</strong> {metricsMap["Max continous losses"]}</li>
    
  </ul>
</div>

</div>




    </div>

    




  );
}
