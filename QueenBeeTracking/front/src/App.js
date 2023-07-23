import React from 'react';
import Navbar from './components/Navbar';
import './App.css';
import Home from './components/pages/Home';
import { BrowserRouter as Router, Switch, Route } from 'react-router-dom';
import Manage from './components/pages/Manage';
import Detection from "./components/pages/manage_pages/Detection";
import BagOutcomes from './components/pages/manage_pages/Bagoutcomes';
import "./components/manage_components/style/dark.scss";
import { useContext } from "react";
import { DarkModeContext } from "./components/manage_components/context/darkModeContext";

function App() {
  const { darkMode } = useContext(DarkModeContext);

  return (
		<div className={darkMode ? "app dark" : "app"}>
			<Router>
				<Navbar />
				<Switch>
					<Route path="/" exact component={Home} />
					<Route path="/manage" component={Manage} />
					<Route path="/detection" component={Detection} />
					<Route path="/bag/:id/:bagname/outcomes" component={BagOutcomes} />
				</Switch>
			</Router>
		</div>
	);
}

export default App;
