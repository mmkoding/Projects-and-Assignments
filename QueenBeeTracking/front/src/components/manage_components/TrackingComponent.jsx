import React, { useState } from 'react';
import { TextField, Button, Typography } from '@mui/material';
import styled from 'styled-components';

const ButtonInput = styled(Button)`
  height: 50px;
  margin-left: 10px;
  background-color: #7960ac;
  color: #fff;
  &:hover {
    background-color: #5754a1;
  }
`;

const TrackingComponent = () => {
  const [inputValue, setInputValue] = useState('12');
  const [gifId, setGifId] = useState('12');
  const [selectedGif, setSelectedGif] = useState('000012.gif');

  const handleInputChange = (event) => {
    setInputValue(event.target.value);
  };

  const handleSubmit = () => {
    let paddedGifId = inputValue.padStart(6, '0');
    setSelectedGif(`${paddedGifId}.gif`);
    setGifId(inputValue);
  };

  return (
    <div style={{ display: 'flex', flexDirection: 'row', justifyContent: 'center', alignItems: 'center' }}>
      <div style={{ margin: '20px' }}>
        <img src={`${process.env.PUBLIC_URL}/${selectedGif}`} alt="selected gif" />
        <Typography variant="h6" color="textPrimary">
          Trajectory {gifId}
        </Typography>
      </div>
      <div style={{ margin: '20px' }}>
        <img src={`${process.env.PUBLIC_URL}/all_trajectories.gif`} alt="all trajectories" />
        <Typography variant="h6" color="textPrimary">
          All Trajectories
        </Typography>
      </div>
      <div style={{ margin: '20px' }}>
        <TextField
          id="outlined-basic"
          label="Enter a trajectory"
          variant="outlined"
          value={inputValue}
          onChange={handleInputChange}
        />
        <ButtonInput variant="contained" color="primary" onClick={handleSubmit} style={{ marginLeft: '10px' }}>
          Get
        </ButtonInput>
      </div>
    </div>
  );
};

export default TrackingComponent;
