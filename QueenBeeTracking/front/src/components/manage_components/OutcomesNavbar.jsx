import React from 'react';
import { Link, useParams } from 'react-router-dom';
import './OutcomesNavbar.scss'

const OutcomesNavbar = () => {
  const { id, bagname } = useParams();

  return (
    <div className="navbar">
      <div className="logo"> Bag: {bagname}</div>
      <ul>
        <li>
          <Link to={`/bag/${id}/${bagname}/outcomes/bag_info`}>Bag Info</Link>
        </li>
        <li>
          <Link to={`/bag/${id}/${bagname}/outcomes/downsampler`}>Downsampler</Link>
        </li>
        <li>
          <Link to={`/bag/${id}/${bagname}/outcomes/detections`}>Detections</Link>
        </li>
        <li>
          <Link to={`/bag/${id}/${bagname}/outcomes/frameanalysis`}>Analysis</Link>
        </li>
        <li>
          <Link to={`/bag/${id}/${bagname}/outcomes/tracking`}>Tracking</Link>
        </li>
        <li>
          <Link to={`/bag/${id}/${bagname}/outcomes/courtbees`}>Court Bees</Link>
        </li>
      </ul>
    </div>
  );
};

export default OutcomesNavbar;
