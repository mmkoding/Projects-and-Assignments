import React, { useEffect, useState } from "react";
import axios from "axios";
import styled from "styled-components";
import Table from "@mui/material/Table";
import TableBody from "@mui/material/TableBody";
import TableCell from "@mui/material/TableCell";
import TableContainer from "@mui/material/TableContainer";
import TableHead from "@mui/material/TableHead";
import TableRow from "@mui/material/TableRow";
import Accordion from "@mui/material/Accordion";
import AccordionSummary from "@mui/material/AccordionSummary";
import AccordionDetails from "@mui/material/AccordionDetails";
import ExpandMoreIcon from "@mui/icons-material/ExpandMore";
import List from "@mui/material/List";
import ListItem from "@mui/material/ListItem";
import ListItemText from "@mui/material/ListItemText";
import Card from "@mui/material/Card";
import CardMedia from "@mui/material/CardMedia";
import Box from "@mui/material/Box";
import Button from "@mui/material/Button";
import TextField from "@mui/material/TextField";
import CircularProgress from "@mui/material/CircularProgress";

const Container = styled.div`
  flex-grow: 1;
  display: flex;
  flex-direction: column;
  padding: 20px;
`;

const Container2 = styled.div`
  display: flex;
  justify-content: flex-start;
  height: 100vh;
  align-items: stretch;
`;

const Frame = styled.img`
  width: 200px; // Adjust this as needed
  height: auto;
`;

const FrameContainerRight = styled.div`
  position: absolute;
  right: 200px; // Add some margin from the right
  top: 120px; // Position it closer to the top
  max-width: 30%;
`;

const InfoContainer = styled.div`
  margin-bottom: 20px;
`;

const FrameContainer = styled.div`
  margin-top: 20px;
  align-items: center;
`;

// const Button = styled.button`
//   margin-bottom: 20px;
//   padding: 10px 20px;
//   background-color: #7960ac;
//   color: #fff;
//   border: none;
//   border-radius: 5px;
//   cursor: pointer;

//   &:hover {
//     background-color: #5754a1;
//   }

//   &:not(:last-child) {
//     margin-right: 10px; // Add right margin to separate buttons from each other
//   }
// `;

// const ButtonInput = styled.button`
//   margin-left: 10px;
//   margin-bottom: 20px;
//   padding: 10px 20px;
//   background-color: #7960ac;
//   color: #fff;
//   border: none;
//   border-radius: 5px;
//   cursor: pointer;

//   &:hover {
//     background-color: #5754a1;
//   }

//   &:not(:last-child) {
//     margin-right: 10px; // Add right margin to separate buttons from each other
//   }
// `;

const DisabledButton = styled(Button)`
  background-color: #ccc;
  cursor: not-allowed;
`;

// const ExtractFramesButton = styled(Button)`
//   background-color: ${(props) => (props.disabled ? "#ddd" : "#7960ac")};
// `;

// const List = styled.ul`
//   margin-bottom: 20px;
//   padding-top: 10px;
// `;

// const ListItem = styled.li`
//   padding: 5px 0;
// `;

const Paragraph = styled.p`
  margin-bottom: 10px;
`;

const Header = styled.h1`
  margin-bottom: 10px;
`;

// Updated Input using Material UI TextField
const Input = styled(TextField)`
  height: 40px;
  margin-bottom: 20px;
`;

const ExtractFramesButton = styled(Button)`
  margin-top: 20px; // Added some margin-top
  background-color: ${(props) => (props.disabled ? "#ddd" : "#7960ac")};
`;

// Replaced the ButtonInput with a Material UI button
const ButtonInput = styled(Button)`
  height: 50px;
  margin-left: 10px;
  background-color: #7960ac;
  color: #fff;
  &:hover {
    background-color: #5754a1;
  }
`;

function convertDateString(timestamp) {
  const [year, month, day, hour, minute, second] = timestamp.split("-");
  const date = new Date(year, month - 1, day, hour, minute, second);
  return `${date.getDate()} ${getMonthName(
    date
  )} ${date.getFullYear()} ${date.getHours()}:${date.getMinutes()}:${date.getSeconds()}`;
}

function getMonthName(date) {
  const months = [
    "January",
    "February",
    "March",
    "April",
    "May",
    "June",
    "July",
    "August",
    "September",
    "October",
    "November",
    "December",
  ];
  return months[date.getMonth()];
}

const BagInfoComponent = ({ id }) => {
  const [bagData, setBagData] = useState(null);
  const [showTopics, setShowTopics] = useState(false);
  const [showImageTopics, setShowImageTopics] = useState(false);
  const [sampleFrames, setSampleFrames] = useState([]);
  const [enteredFrameNumber, setEnteredFrameNumber] = useState("");
  const [currentFrameUrl, setCurrentFrameUrl] = useState("");
  const [isModalOpen, setIsModalOpen] = useState(false);
  const [frameNumber, setFrameNumber] = useState(0);
  const [detectedFrameNumber, setDetectedFrameNumber] = useState(0);

  const extractFrames = async () => { // take id as a parameter to this function
    try {
      console.log("Extracting frames...");
      console.log(id); // log the id to the console
      const response = await axios.post(`http://144.122.48.179:8001/bags/${id}/extract-frames`);
      // You may handle the response data as required
    } catch (error) {
      console.error("Failed to extract frames", error);
    }
  };
  

  useEffect(() => {
    const fetchBagData = async () => {
      try {
        const response = await axios.get(
          `http://144.122.48.179:8000/bags/${id}`
        );
        setBagData(response.data);
      } catch (error) {
        console.error("Failed to fetch bag data", error);
      }
    };

    fetchBagData();
  }, [id]);

  useEffect(() => {
    const fetchFrameCount = async () => {
      try {
        const response = await axios.get(
          `http://144.122.48.179:8000/bags/${id}/hive_0/frame_count`
        );
        const frameCountData = await response.data;
        setFrameNumber(frameCountData.data);
      } catch (error) {
        console.error("Failed to fetch frame count", error);
      }
    };

    fetchFrameCount();
  }, [id]);

  // Fetch detected frame count whenever 'id' changes
  useEffect(() => {
    const fetchDetectedFrameCount = async () => {
      try {
        const response = await axios.get(
          `http://144.122.48.179:8000/detections/${id}/hive_0/detected_frame_count`
        );
        const countData = await response.data;
        setDetectedFrameNumber(countData.data);
      } catch (error) {
        console.error("Failed to fetch detected frame count", error);
      }
    };

    fetchDetectedFrameCount();
  }, [id]);

  useEffect(() => {
    const fetchFrames = async () => {
      const bagName = id;
      const hiveName = "hive_0"; // Assuming this is always the same
      const frameNumbers = Array.from({ length: 50 }, (_, i) => i + 1); // Here frameNumbers go from 1 to 45

      const framePromises = frameNumbers.map((frameNumber) =>
        axios
          .get(
            `http://144.122.48.179:8000/image/${bagName}/${hiveName}/${frameNumber}`,
            {
              responseType: "blob",
            }
          )
          .then((response) => {
            const url = window.URL.createObjectURL(new Blob([response.data]));
            return url;
          })
      );

      const frames = await Promise.all(framePromises);
      setSampleFrames(frames);
    };
    fetchFrames();
  }, [bagData]);

  if (!bagData) return <h2>Loading bag data...</h2>;

  const handleFrameNumberChange = (event) => {
    setEnteredFrameNumber(event.target.value);
  };

  const handleFrameNumberSubmit = () => {
    const index = parseInt(enteredFrameNumber) - 1;
    if (index >= 0 && index < sampleFrames.length) {
      setCurrentFrameUrl(sampleFrames[index]);
      setIsModalOpen(true); // Open the modal
    } else {
      alert("Please enter a valid frame number.");
    }
  };

  const showNextFrame = (direction) => {
    const currentIndex = sampleFrames.indexOf(currentFrameUrl);

    if (currentIndex === -1) {
      console.error("Current frame URL not found in sampleFrames");
      return;
    }

    let nextIndex;

    if (direction === "previous") {
      nextIndex =
        currentIndex === 0 ? sampleFrames.length - 1 : currentIndex - 1;
    } else {
      nextIndex = (currentIndex + 1) % sampleFrames.length;
    }

    setEnteredFrameNumber(nextIndex + 1); // Update the enteredFrameNumber state as well
    setCurrentFrameUrl(sampleFrames[nextIndex]);
  };

  const closeModal = (e) => {
    if (e) e.stopPropagation();
    setIsModalOpen(false);
  };

  const listItemsAllTopics = bagData.data.topics.map((topic, index) => (
    <ListItem key={index}>
      <ListItemText primary={topic} />
    </ListItem>
  ));

  const listItemsImageTopics = bagData.data.image_topics.map((topic, index) => (
    <ListItem key={index}>
      <ListItemText primary={topic} />
    </ListItem>
  ));

  return (
    <Container2>
      <Container>
        <InfoContainer>
          {frameNumber > 0 && !sampleFrames[0] && (
            <Box display="flex" justifyContent="center" marginBottom="20px">
              <CircularProgress />
            </Box>
          )}
          {sampleFrames[0] && (
            <Box display="flex" justifyContent="center" marginBottom="20px">
              <Card>
                <CardMedia
                  component="img"
                  style={{ maxHeight: "300px", objectFit: "contain" }}
                  image={sampleFrames[0]}
                  alt="First Frame"
                />
              </Card>
            </Box>
          )}
          <TableContainer sx={{ backgroundColor: "white" }}>
            <Table>
              <TableBody>
                <TableRow>
                  <TableCell>Name</TableCell>
                  <TableCell>{bagData.data.name}</TableCell>
                </TableRow>
                <TableRow>
                  <TableCell>Date</TableCell>
                  <TableCell>{convertDateString(bagData.data.date)}</TableCell>
                </TableRow>
                <TableRow>
                  <TableCell>Total Frame Number</TableCell>
                  <TableCell>501</TableCell> {/* Display total frame count */}
                </TableRow>
                <TableRow>
                  <TableCell>Detected Frame Number</TableCell>
                  <TableCell>501</TableCell>{" "}
                  {/* Display detected frame count */}
                </TableRow>
              </TableBody>
            </Table>
          </TableContainer>
          {/* All Topics */}
          <Accordion>
            <AccordionSummary
              expandIcon={<ExpandMoreIcon />}
              aria-controls="panel1a-content"
              id="panel1a-header"
            >
              All Topics
            </AccordionSummary>
            <AccordionDetails>
              <List>{listItemsAllTopics}</List>
            </AccordionDetails>
          </Accordion>

          {/* Image Topics */}
          <Accordion>
            <AccordionSummary
              expandIcon={<ExpandMoreIcon />}
              aria-controls="panel2a-content"
              id="panel2a-header"
            >
              Image Topics
            </AccordionSummary>
            <AccordionDetails>
              <List>{listItemsImageTopics}</List>
            </AccordionDetails>
          </Accordion>
          <div>
            <Button
              variant="contained"
              color="primary"
              style={{
                backgroundColor: frameNumber > 0 ? "gray" : "#7960ac",
                marginBottom: "20px",
                marginTop: "20px",
              }}
              onClick={() => {
                if (frameNumber > 0) {
                  if (
                    window.confirm(
                      "This action will override results. Are you sure?"
                    )
                  ) {
                    extractFrames(); // Your method to fetch frames
                  }
                } else {
                  extractFrames();
                }
              }}
            >
              Extract Frames
            </Button>
          </div>
        </InfoContainer>
        <FrameContainer>
          <Header>Frame Viewer</Header>
          <Input
            variant="outlined"
            type="number"
            min="1"
            placeholder="Enter frame number"
            value={enteredFrameNumber}
            onChange={handleFrameNumberChange}
            sx={{ backgroundColor: "white", color: "black" }}
          />
          <ButtonInput variant="contained" onClick={handleFrameNumberSubmit}>
            Get Frame
          </ButtonInput>
        </FrameContainer>
        {isModalOpen && (
          <div className="modal">
            <button
              className="arrow arrow-left"
              onClick={(e) => {
                e.stopPropagation();
                showNextFrame("previous");
              }}
            >
              &lt;
            </button>
            <button
              className="arrow arrow-right"
              onClick={(e) => {
                e.stopPropagation();
                showNextFrame("next");
              }}
            >
              &gt;
            </button>
            <button className="closeButton" onClick={closeModal}>
              Close
            </button>
            <img className="modal-image" src={currentFrameUrl} alt="Popup" />
          </div>
        )}
      </Container>
    </Container2>
  );
};

export default BagInfoComponent;
