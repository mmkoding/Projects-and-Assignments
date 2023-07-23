//import React, { useState, useEffect } from 'react';
//import '../../App.css';
import './Manage.scss';
import SidebarAdm from '../manage_components/SidebarAdm';
import NawbarAdm from '../manage_components/NawbarAdm';
import Chart from "../manage_components/Chart";
import Table from "../manage_components/Table";
import { useState, useEffect } from 'react';
import axios from 'axios';
import { useHistory } from 'react-router-dom'; 

export default function Manage() {
  const [bags, setBags] = useState([]);
  const history = useHistory();

  const isAuthenticated = window.localStorage.getItem('isAuthenticated');
  if (!isAuthenticated || isAuthenticated !== 'true') {
    history.push("/");
  }

  useEffect(() => {
    axios.get('http://144.122.48.179:8000/bags')
    .then(response => setBags(response.data.data))
    .catch(error => console.error(error));
  }, []);

//  useEffect(() => {
//    axios.get('http://localhost:8000/detections')
//      .then(response => setDetections(response.data.data))
//      .catch(error => console.error(error));
//  }, []);


  return (
      <div className="manage">
        <SidebarAdm />
        <div className="manageContainer">
          <NawbarAdm />
          <div className="listContainer">
          <div className="listTitle">Bag Files <span className="listTitleBagCount">Total: {bags.length}</span> </div>
          {bags.length > 0 && <Table Bags = {bags} />}
        </div>
        <div className="charts">
        <Chart title="Created Bags" aspect={16 / 3} bags={bags} />
        </div>
        
        </div>
      </div>
  );
}
