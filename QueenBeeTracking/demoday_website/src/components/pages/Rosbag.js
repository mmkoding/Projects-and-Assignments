import React from 'react';
import '../../App.css';

export default function Rosbag() {
  return (
    <>
        <div style = {{ backgroundColor: 'black' }}>
          <div>
            <video src='/videos/rosbag.mp4' autoPlay loop muted style={{ objectFit: 'cover', width: '100%', height: '100%' }} />
          </div>
        </div>
    </>
  );
  
}
