import React, { useEffect, useState } from "react";
import axios from "axios";
import styled from "styled-components";
import Button from '@mui/material/Button';

const Paragraph = styled.p`
  margin-bottom: 10px;
`;

const Header = styled.h1`
  margin-bottom: 10px;
`;

const Container = styled.div`
  flex-grow: 1;
  display: flex;
  flex-direction: column;
  padding: 20px;
`;

const InfoContainer = styled.div`
  margin-bottom: 20px;
`;

const ButtonStyled = styled(Button)`
  background-color: #7960ac;
  color: #fff;
  &:hover {
    background-color: #5754a1;
  }
`;

const ButtonInput = styled.button`
  margin-left: 10px;
  margin-bottom: 20px;
  padding: 10px 20px;
  background-color: #7960ac;
  color: #fff;
  border: none;
  border-radius: 5px;
  cursor: pointer;

  &:hover {
    background-color: #5754a1;
  }

  &:not(:last-child) {
    margin-right: 10px; // Add right margin to separate buttons from each other
  }
`;

const DisabledButton = styled(Button)`
  background-color: #ccc;
  cursor: not-allowed;
`;

const FetchFramesButton = styled(Button)`
  background-color: ${(props) => (props.disabled ? "#ddd" : "#7960ac")};
`;

const DetectFramesButton = styled(Button)`
  background-color: ${(props) => (props.disabled ? "#ddd" : "#7960ac")};
`;

const Input = styled.input`
  margin-bottom: 20px;
  padding: 10px 20px;
  background-color: #ffffff; // white background
  color: #000000; // black text
  border: 1px solid #000000; // black border
  border-radius: 5px;
`;

function Detections({ id }) {
  const [enteredFrameNumber, setEnteredFrameNumber] = useState("");
  const [sampleFrames, setSampleFrames] = useState([]);
  const [isModalOpen, setIsModalOpen] = useState(false);
  const [currentFrameUrl, setCurrentFrameUrl] = useState("");
  const [detectedFrameNumber, setDetectedFrameNumber] = useState(0);

  const detectFrames = async () => {
    try {
      const response = await axios.post(
        `http://144.122.48.179:8000/detect?bag_id=${id}&hive_id=hive_0`,
      );
    } catch (error) {
      console.error("Failed to run detection", error);
    }
  };

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

  const fetchFrames = async () => {
    const bagName = id;
    const hiveName = "hive_0"; // Assuming this is always the same
    const frameNumbers = Array.from({ length: 50 }, (_, i) => i + 1); // Here frameNumbers go from 1 to 45

    const framePromises = frameNumbers.map((frameNumber) =>
      axios
        .get(
          `http://144.122.48.179:8000/detect_frame?bag_id=${bagName}&hive_id=${hiveName}&frame_id=${frameNumber}`,
          {
            responseType: "arraybuffer",
          }
        )
        .then((response) => {
          const base64 = btoa(
            new Uint8Array(response.data).reduce(
              (data, byte) => data + String.fromCharCode(byte),
              ""
            )
          );
          return `data:image/png;base64,${base64}`;
        })
    );

    const frames = await Promise.all(framePromises);
    setSampleFrames(frames);
  };

  useEffect(() => {
    fetchFrames();
  }, []);

  const handleFrameNumberChange = (event) => {
    setEnteredFrameNumber(event.target.value);
  };

  const handleFrameNumberSubmit = () => {
    const index = parseInt(enteredFrameNumber) - 1;
    if (index >= 0 && index < sampleFrames.length) {
      setCurrentFrameUrl(sampleFrames[index]);
      setIsModalOpen(true);
    } else {
      alert("Please enter a valid frame number.");
    }
  };

  const closeModal = (e) => {
    if (e) e.stopPropagation();
    setIsModalOpen(false);
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

  const getFrameNumber = () => {
    const currentIndex = sampleFrames.indexOf(currentFrameUrl);
    return currentIndex + 1; // Because frameNumbers are from 1 to 45
  };

  return (
    <Container>
      <Header>Detection Interface</Header>
      <Paragraph>Detected Frame Number: 501</Paragraph>
      <div>
        <ButtonStyled 
        variant="contained" color="primary"
          // style={{
          //   backgroundColor: detectedFrameNumber > 0 ? "gray" : "#7960ac",
          // }}
          onClick={() => {
            if (detectedFrameNumber > 0) {
              if (
                window.confirm(
                  "This action will override results. Are you sure?"
                )
              ) {
                detectFrames(); // Your method to detect frames
              }
            } else {
              detectFrames();
            }
          }}
        >
          Create Analysis
        </ButtonStyled>
      </div>

      <div className="frameNumberInputContainer">
        <Input
          className="frameNumberInput"
          type="number"
          placeholder="Enter frame number"
          value={enteredFrameNumber}
          onChange={handleFrameNumberChange}
        />
        <ButtonInput className="getButton" onClick={handleFrameNumberSubmit}>
          Get
        </ButtonInput>
      </div>
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
          <span className="frameNumber">Frame {getFrameNumber()}</span>
          <img className="modal-image" src={currentFrameUrl} alt="Popup" />
        </div>
      )}
    </Container>
  );
}

export default Detections;
