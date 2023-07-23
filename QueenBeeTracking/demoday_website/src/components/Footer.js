import React from 'react';
import './Footer.css';

function Footer() {
  return (
    <div className='footer-container'>
      
      <div className='footer-links'>
        <div className='footer-link-wrapper'>
          <div className='footer-link-items'>
            <h2>Supervisor</h2>
            <span style={{ color: 'white' }}>Hande Alemdar</span>
            <span style={{ color: 'white' }}>Erol Şahin</span>
          </div>
          <div className='footer-link-items'>
            <h2>Team Members</h2>
            <span style={{ color: 'white' }}></span>
            <span style={{ color: 'white' }}>Atınç Utku Alparslan</span>
            <span style={{ color: 'white' }}>Mustafa Mert Köse</span>
            <span style={{ color: 'white' }}>Berke Sina Ahlatçı</span>
            <span style={{ color: 'white' }}>Murat Akkoyun</span>
            <span style={{ color: 'white' }}>Atakan Karaca</span>
          </div>
          <div className='footer-link-items'>
            <h2>Roboroyale</h2>
            <a href='https://roboroyale.eu/' target='_blank' rel='noopener noreferrer'>
              Website
            </a>
          </div>
        </div>
      </div>
      
      

    </div>
  );
}

export default Footer;
