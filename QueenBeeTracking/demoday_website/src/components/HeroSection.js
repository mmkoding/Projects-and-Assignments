import React from 'react';
import '../App.css';
import './HeroSection.css';

function HeroSection() {
  return (
    <div className='hero-container'>
      <video src={process.env.PUBLIC_URL + '/video/HeroVideo.mp4'} autoPlay loop muted />
      <h1 className='header'>QueenBee Tracker</h1>
      <p className='description'>We provide tools and analyses about queen bee and court bees to help researchers on their studies. </p>
      <div className='hero-btns'>
      </div>
    </div>
  );
}

export default HeroSection;
