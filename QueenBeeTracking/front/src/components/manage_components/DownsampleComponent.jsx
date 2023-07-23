import React, { useState, useEffect } from 'react';
import axios from 'axios';
import Slider from '@mui/material/Slider';
import Button from '@mui/material/Button';
import CardMedia from '@mui/material/CardMedia';
import Container from '@mui/material/Container';
import Typography from '@mui/material/Typography';
import styled from 'styled-components';
import CircularProgress from "@mui/material/CircularProgress";
import Box from "@mui/material/Box";
import Dialog from '@mui/material/Dialog';
import DialogActions from '@mui/material/DialogActions';
import DialogContent from '@mui/material/DialogContent';
import DialogContentText from '@mui/material/DialogContentText';
import DialogTitle from '@mui/material/DialogTitle';

const ButtonStyled = styled(Button)`
  background-color: #7960ac;
  color: #fff;
  &:hover {
    background-color: #5754a1;
  }
`;

const SliderStyled = styled(Slider)`

color: #7960ac;

`;

const description = (
  <p>
    You can adjust the downsampling amount by comparing the bee size in your frames with the bee size of the sample frame
  </p>
);

const DownsampleComponent = ({ id}) => {
  const [downsampleAmount, setDownsampleAmount] = useState(1);
  const [firstFrameUrl, setFirstFrameUrl] = useState(null);
  const [loading, setLoading] = useState(true); // Added loading state


  const sample_image = process.env.PUBLIC_URL + '/sample_bee.png';
  
  useEffect(() => {
    const fetchFirstFrame = async () => {
      const bagName = id;
      const hiveName = "hive_0"; // Assuming this is always the same
      const frameNumber = 1; // Assuming this is always the same

      const response = await axios.get(`http://144.122.48.179:8000/image/${bagName}/${hiveName}/${frameNumber}`, {
        responseType: "blob",
      });

      const url = window.URL.createObjectURL(new Blob([response.data]));
      setFirstFrameUrl(url);
      setLoading(false); // Set loading to false when image is fetched
    };
    fetchFirstFrame();
  }, [id]);


  const handleChange = (event, newValue) => {
    setDownsampleAmount(newValue);
  };

  const handleDownsample = async () => {
    try {
      const response = await axios.post(`http://144.122.48.179:8000/downsample?bag_id=${id}&hive_id=hive_0&downsample_amount=${downsampleAmount}`)
      if (response.status === 200){
        alert("Downsampled frames successfully");
      }
    }
    catch (error) {
      console.log(error)
      alert("An error occurred while downsampling frames")
    }
  };

  const [open, setOpen] = React.useState(false);

  const handleClickOpen = () => {
    setOpen(true);
  };

  const handleClose = () => {
    setOpen(false);
  };


  return (
    <Container>
        <Box sx={{ height: 20 }}  padding = "50px 25px">
          <Button variant="outlined" onClick={handleClickOpen}  sx={{backgroundColor: '#7960ac', color: '#fff', '&:hover': { backgroundColor: '#5754a1',} }}>
            Show Instructions
          </Button>
          <Dialog open={open} onClose={handleClose}>
            <DialogTitle>{"Instructions"}</DialogTitle>
            <DialogContent>
              <DialogContentText>
                {description}
              </DialogContentText>
            </DialogContent>
            <DialogActions>
              <Button onClick={handleClose} sx={{backgroundColor: '#7960ac', color: '#fff', '&:hover': { backgroundColor: '#5754a1',} }}>
                Close
              </Button>
            </DialogActions>
          </Dialog>
        </Box>
        <Box width={700} height={700}>
        <Typography variant="h6">Sample Image</Typography>
        <CardMedia component="img" image={sample_image} alt="Original Image" sx={{ 
        maxWidth: '50%', // constrain maximum width to its container
        height: 'auto',   // auto height based on aspect ratio
        objectFit: 'contain' // ensure it keeps its aspect ratio while fitting within its box 
        }} />
        <Typography variant="h6">Downsampled Image</Typography>
        {
            loading 
            ? <CircularProgress /> // Display loading spinner if image is not yet fetched
            : <CardMedia 
                component="img" 
                image={firstFrameUrl} 
                alt="Downsampled Image" 
                sx={{ 
                width: `${100/downsampleAmount}%`, 
                height: `${100/downsampleAmount}%`,
                objectFit: 'contain' // keep the aspect ratio while resizing
                }} 
            />
        }
        <SliderStyled
            defaultValue={downsampleAmount}
            aria-labelledby="discrete-slider"
            valueLabelDisplay="auto"
            step={1}
            marks
            min={1}
            max={10}
            onChange={handleChange}
            sx={{color: '#7960ac', '&:hover': { backgroundColor: '#c9b0ec',} }}
        />
        <ButtonStyled variant="contained" color="primary" onClick={handleDownsample} sx={{backgroundColor: '#7960ac', color: '#fff', '&:hover': { backgroundColor: '#5754a1',} }}>
            Downsample
        </ButtonStyled>
        </Box>
    </Container>
  );
};

export default DownsampleComponent;
