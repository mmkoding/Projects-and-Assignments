import React from 'react';
import "./SidebarAdm.scss";
import DashboardIcon from "@mui/icons-material/Dashboard";
import PersonOutlineIcon from "@mui/icons-material/PersonOutline";
import ExitToAppIcon from "@mui/icons-material/ExitToApp";
import { Link } from "react-router-dom";
import { DarkModeContext } from "./context/darkModeContext";
import { useContext } from "react";
import { useHistory } from 'react-router-dom'; 


const SidebarAdm = () => {
  const history = useHistory();

  function logout() {
    window.localStorage.removeItem('isAuthenticated');
    history.push('/login');
  }
  
  const { dispatch } = useContext(DarkModeContext);
  return (
    <div className="sidebar">
      <div className="top">
        <Link to="/manage" style={{ textDecoration: "none" }}>
          <span className="logo">Manage</span>
        </Link>
      </div>
      <hr />
      <div className="center">
        <ul>
          <Link to="/manage" style={{ textDecoration: "none" }}>
          <li>
            <DashboardIcon className="icon" />
            <span>Dashboard</span>
          </li>
          </Link>
            
          <Link to="/" style={{ textDecoration: "none" }}>
          <li onClick={logout}>
            <ExitToAppIcon className="icon" />
            <span>Exit</span>
          </li>
          </Link>
        </ul>
      </div>
      <div className="bottom">
        <div
          className="colorOption"
          onClick={() => dispatch({ type: "LIGHT" })}
        ></div>
        <div
          className="colorOption"
          onClick={() => dispatch({ type: "DARK" })}
        ></div>
      </div>
    </div>
  );
};

export default SidebarAdm;