import React from 'react';
import '../../App.css';
import Cards from '../Cards';
import HeroSection from '../HeroSection';
import Footer from '../Footer';
import TechPortfolio from '../Techs';

function Home() {
  return (
    <>
      <HeroSection />
      <Cards />
      <TechPortfolio />
      <Footer />
    </>
  );
}

export default Home;
