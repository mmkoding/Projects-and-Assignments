import React, { useEffect, useState } from 'react';
import Chart from 'react-apexcharts';
import axios from "axios";
import '../pages/Manage.scss';
import PolarHeatmap from "./PolarHeatmap";
import Dialog from '@mui/material/Dialog';
import DialogActions from '@mui/material/DialogActions';
import DialogContent from '@mui/material/DialogContent';
import DialogContentText from '@mui/material/DialogContentText';
import DialogTitle from '@mui/material/DialogTitle';
import styled from "styled-components";
import Button from '@mui/material/Button';
import Box from '@mui/material/Box';

function getTotalBee(beeArray) {
  let total = 0;
  for (let i = 0; i < beeArray.length; i++) {
    total += beeArray[i];
  }
  return total;
}

const BeeDataPieChart = ({ beeData }) => {
  const options = {
    labels: beeData.map(item => item.argument),
    colors: ['#C371E7', '#9371E7', '#C37195', '#FF50E7'], 
    responsive: [{
      breakpoint: 480,
      options: {
        chart: {
          width: 200
        },
        legend: {
          position: 'bottom'
        }
      }
    }],
    dataLabels: {
      enabled: true,
      formatter: function (val, opts) {
        return opts.w.config.labels[opts.seriesIndex]
      }
    },
    tooltip: {
      y: {
        formatter: function(val) {
          return val
        }
      }
    }
  };

  const series = beeData.map(item => item.value);

  return (
    <div className="row">
      <div className="mixed-chart">
        <Chart
          options={options}
          series={series}
          type="pie"
          width="700"
        />
      </div>
    </div>
  );
};

const BeeDataBarChart = ({ beeData }) => {
  const options = {
      chart: {
          id: 'basic-bar'
      },
      xaxis: {
          categories: beeData.map(item => item.argument)
      },
      colors: ['#C371E7'],
  };

  const series = [
      {
          name: 'Bees',
          data: beeData.map(item => item.value)
      }
  ];

  return (
      <div className="row">
          <div className="mixed-chart">
              <Chart
                  options={options}
                  series={series}
                  type="bar"
                  width="700"
              />
          </div>
      </div>
  );
};


const FrameAnalysis = ({ id}) => {
  const [series, setSeries] = useState([]);
  const [options, setOptions] = useState({});
  // eslint-disable-next-line
  const [graphData, setGraphData] = useState(null);
  const [beeDataForBar, setBeeDataForBar] = useState([]);

  
const description = (
  <p>
    The charts on this page provide various analyses about court bees and queen bee. Firstly, a chart that plots the movement of the queen bee around the hive is provided. Additionally, You can observe in separate charts, for every second, the court bee counts at the positions top-bottom-left-right with respect to the queen bee. Moreover in another chart, you can observe, for every second, the court bee density around the queen bee. The data that we use while creating these charts is derived by processing the outcomes of the outputs of the model we utilize. By tracking the changes in these charts over time, one can gain insight into patterns and trends regarding the relationship between queen bee and the court bees.
  </p>
);

/*
  useEffect(() => {
    const fetchQueenGraphData = async () => {
        const response = await axios.get('http://144.122.48.179:8000/create_graph_queen?bag_id=6457bc4998a51b143993c4df&hive_id=hive_0');
        setGraphData(response.data);
    };
    fetchQueenGraphData();
  }, [id]);
*/
  useEffect(() => {
    const fetchCourtGraphData = async () => {
      const response = await axios.get(`http://144.122.48.179:8000/create_graph_court?bag_id=${id}&hive_id=hive_0`);


      //console.log(response.data.data);
      
        let courtBeeData = response.data.data;
        courtBeeData = courtBeeData.replace(/'/g, '"');
        courtBeeData = JSON.parse(courtBeeData);

        const topData = courtBeeData.map(arr => arr.filter(position => position === 'top').length);
        const bottomData = courtBeeData.map(arr => arr.filter(position => position === 'bottom').length);
        const leftData = courtBeeData.map(arr => arr.filter(position => position === 'left').length);
        const rightData = courtBeeData.map(arr => arr.filter(position => position === 'right').length);
        const densityData = courtBeeData.map(arr => arr.length);
  
        setSeries([
            { name: "Court Bees on Top", data: topData },
            { name: "Court Bees on Bottom", data: bottomData },
            { name: "Court Bees on Left", data: leftData },
            { name: "Court Bees on Right", data: rightData },
            { name: "Court Bee Density", data: densityData },
        ]);
        setBeeDataForBar([
          { argument: "Court Bees on Top", value: getTotalBee(topData) },
          { argument: "Court Bees on Bottom", value: getTotalBee(bottomData) },
          { argument: "Court Bees on Left", value: getTotalBee(leftData) },
          { argument: "Court Bees on Right", value: getTotalBee(rightData) },
        ]);
        console.log('--->series:',series)
        console.log('--->bar data:',beeDataForBar)

    setOptions({
        chart: {
          type: 'line',
          background: '#000',
          foreColor: '#fff'
        },
        xaxis: {
          categories: Array.from({ length: courtBeeData.length }, (_, i) => `${i + 1}s`)
        },
        colors: ['#ff00ff', '#00ffff', '#00ff00', '#ffff00']
      });
    };
    fetchCourtGraphData();
  }, [id]);

  const [open, setOpen] = React.useState(false);

  const handleClickOpen = () => {
    setOpen(true);
  };

  const handleClose = () => {
    setOpen(false);
  };
  
    return (
      <div className="manage">
        <div className="manageContainer">
          <h1 align="center">Welcome to the Analysis Page</h1>
          <Box sx={{ height: 180 }}  padding = "50px">
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
          <QueenBeeMovement id={id} barData={beeDataForBar}/>
          {series.map((s, index) =>
            <div key={index}>
              <h2 align='center' style={{ color: '#ff00ff', textTransform: 'uppercase', letterSpacing: '2px', fontSize: '30px' }}>{s.name}</h2>
              <Chart 
                options={options} 
                series={[s]} 
                type={s.name === 'Court Bee Density' ? 'area' : 'line'}
                align = "center"
                width="95%"
                height="350"
              />
            </div>
          )}
        </div>
        
      </div>
    );
  }

  const QueenBeeMovement = ({id, barData}) => {
    // eslint-disable-next-line
    const [graphData, setGraphData] = useState({ x: [], y: [] });
    const [angleValues, setAngleValues] = useState([]);
    console.log(barData)
      useEffect(() => {
        const fetchQueenGraphData = async () => {
        const response = await axios.get(`http://144.122.48.179:8000/create_graph_queen?bag_id=${id}&hive_id=hive_0`);
    
        let angle_values = response.data.data;
        angle_values = JSON.parse(angle_values);

          // Assuming the response.data is in the same format as angle_values
          setAngleValues(angle_values);
          angle_values = angle_values.map(angle => angle * Math.PI / 180);
    
          var x = [0];
          var y = [0];
    
          for (var i = 0; i < angle_values.length; i++) {
              x.push(x[i] + Math.cos(angle_values[i]));
              y.push(y[i] + Math.sin(angle_values[i]));
          }
          setGraphData({ x, y });
          

        var canvas = document.getElementById('neonGraphCanvas');
        var ctx = canvas.getContext('2d');

        canvas.width = window.innerWidth - 200;
        canvas.height = window.innerHeight - 200;

        var neonColor = '#00FFA6';
        var neonColor_2 = '#F400A1';

        var xScale = canvas.width / (2 * Math.max(...x.map(Math.abs)));
        var yScale = canvas.height / (2 * Math.max(...y.map(Math.abs)));

        ctx.beginPath();
        ctx.moveTo(x[0] * xScale + canvas.width / 2, -y[0] * yScale + canvas.height / 2);
        ctx.lineWidth = 2;
        ctx.strokeStyle = neonColor_2;

        for (i = 1; i < x.length; i++) {
            ctx.lineTo(x[i] * xScale + canvas.width / 2, -y[i] * yScale + canvas.height / 2);
        }

        ctx.stroke();

        ctx.fillStyle = neonColor;
        ctx.beginPath();
        ctx.arc(x[0] * xScale + canvas.width / 2, -y[0] * yScale + canvas.height / 2, 5, 0, Math.PI * 2);
        ctx.fill();

        ctx.beginPath();
        ctx.arc(x[x.length - 1] * xScale + canvas.width / 2, -y[y.length - 1] * yScale + canvas.height / 2, 5, 0, Math.PI * 2);
        ctx.fill();

        ctx.fillStyle = neonColor;
        ctx.font = '18px Arial';
        ctx.fillText('Queen Bee Movement', 10, 40);

        ctx.strokeStyle = neonColor;
        ctx.lineWidth = 0.5;

        ctx.beginPath();
        for (i = 0; i < canvas.height; i += 20) {
            ctx.moveTo(0, i);
            ctx.lineTo(canvas.width, i);
        }
        ctx.stroke();

        ctx.beginPath();
        for (i = 0; i < canvas.width; i += 20) {
            ctx.moveTo(i, 0);
            ctx.lineTo(i, canvas.height);
        }
        ctx.stroke();
      };
      
      fetchQueenGraphData();
    }, [id]);
    
    const GraphContainer = styled.div`
      flex-grow: 1;
      display: flex;
      flex-direction: row;
      padding: 20px;
    `;
 
    return (
      <>
        <canvas id="neonGraphCanvas" style={{ backgroundColor: 'black'}}></canvas>
        <GraphContainer>
          <span style={{width: '50%'}}>
            <PolarHeatmap style={{width: '50%'}} angleArray={angleValues} />
          </span>
          <BeeDataPieChart beeData={barData}/>
        </GraphContainer>
      </>
    );
  }
  
  export default FrameAnalysis;
