import React from 'react';
import { Link } from 'react-router-dom';
import './Navbar.css';

function Navbar() {



  return (
    <>
      <nav className='navbar'>
        <div className='navbar-container'>
          <Link to='/' className='navbar-logo'>
            QUEEN BEE TRACKING
            {/* <i class='fab fa-typo3' /> */}
            <img src={process.env.PUBLIC_URL + '/queenbee.png'} alt='logo' style={{width: '39px', marginLeft: '3px'}}/> 
          </Link>
          
        </div>
      </nav>
    </>
  );
}

export default Navbar;
