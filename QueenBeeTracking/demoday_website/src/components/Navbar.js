import React, { useState, useEffect } from 'react';
import { Link } from 'react-router-dom';
import './Navbar.css';

function Navbar() {
  const [click, setClick] = useState(false);
  // eslint-disable-next-line
  const [button, setButton] = useState(true);

  const handleClick = () => setClick(!click);
  const closeMobileMenu = () => setClick(false);

  function scrollToNextPage() {
    window.scrollTo({
      top: window.innerHeight,
      behavior: 'smooth',
    });
  }

  function scrollToTop() {
    window.scrollTo({
      top: 0,
      behavior: 'smooth',
    });
  }

  function scrollToBottom() {
    window.scrollTo({
      top: document.body.scrollHeight,
      behavior: 'smooth',
    });
  }

  const showButton = () => {
    if (window.innerWidth <= 960) {
      setButton(false);
    } else {
      setButton(true);
    }
  };

  useEffect(() => {
    showButton();
  }, []);

  window.addEventListener('resize', showButton);

  return (
    <>
      <nav className='navbar'>
        <div className='navbar-container'>
          <Link to='/' className='navbar-logo' onClick={() => {closeMobileMenu(); scrollToTop();}}>
            QUEEN BEE TRACKING
            {/* <i class='fab fa-typo3' /> */}
            <img src={process.env.PUBLIC_URL + '/queenbee.png'} alt='logo' style={{width: '39px', marginLeft: '3px'}}/> 
          </Link>
          <div className='menu-icon' onClick={handleClick}>
            <i className={click ? 'fas fa-times' : 'fas fa-bars'} />
          </div>
          <ul className={click ? 'nav-menu active' : 'nav-menu'}>
            <li className='nav-item'>
              <Link to='/' className='nav-links' onClick={() => {closeMobileMenu(); scrollToTop();}}>
                Home
              </Link>
            </li>
            <li className='nav-item'>
              <Link
                to='/'
                className='nav-links'
                onClick={() => {
                  closeMobileMenu();
                  scrollToNextPage();
                }}
              >
                Our Work
              </Link>
            </li>

            <li className='nav-item'>
              <Link
                to='/'
                className='nav-links'
                onClick={() => {
                  closeMobileMenu();
                  scrollToBottom();
                }}
              >
                Team
              </Link>
            </li>

          </ul>
        </div>
      </nav>
    </>
  );
}

export default Navbar;
