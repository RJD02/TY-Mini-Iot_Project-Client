import "./App.css";
import {
  BrowserRouter as Router,
  Navigate,
  Routes,
  Route,
} from "react-router-dom";
import Navbar from "./components/Navbar/Navbar";
import Pets from "./pages/Pets/Pets";

function App() {
  return (
    <div className="container-fluid">
      <Navbar />
      <Router>
        <Routes>
          <Route path="/pets" element={<Pets />} />
        </Routes>
      </Router>
    </div>
  );
}

export default App;
