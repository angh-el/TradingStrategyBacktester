import React from "react";
import { BrowserRouter as Router, Routes, Route } from "react-router-dom";
import ConfigurePage from "./pages/ConfigurePage";
import ResultsPage from "./pages/ResultsPage"; // placeholder

export default function App() {
  return (
    <Router>
      <Routes>
        <Route path="/" element={<ConfigurePage />} />
        <Route path="/results" element={<ResultsPage />} />
      </Routes>
    </Router>
  );
}
