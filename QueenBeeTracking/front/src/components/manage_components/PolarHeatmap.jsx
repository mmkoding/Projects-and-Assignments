import React, { useEffect, useRef } from "react";
import { Chart, PolarAreaController, ArcElement, CategoryScale, Tooltip, Title, Legend, RadialLinearScale } from 'chart.js';

Chart.register(PolarAreaController, ArcElement, CategoryScale, Tooltip, Title, Legend, RadialLinearScale);

const PolarHeatmap = ({ angleArray }) => {
    const canvasRef = useRef(null);
    const chartRef = useRef(null); 

    useEffect(() => {
        const canvas = canvasRef.current;
        const ctx = canvas.getContext("2d");

        // Calculate the frequency of each angle direction
        var angleFrequencies = {};
        angleArray.forEach(function(angle) {
            angle = (angle + 90) % 360; // Shift angles by 90 degrees and normalize to 0-359 range
            if (angleFrequencies[angle]) {
                angleFrequencies[angle]++;
            } else {
                angleFrequencies[angle] = 1;
            }
        });

        // Convert angle frequencies to an array of objects
        var angleData = [];
        for (var angle in angleFrequencies) {
            if (angleFrequencies.hasOwnProperty(angle)) {
                angleData.push({ angle: parseInt(angle), frequency: angleFrequencies[angle] });
            }
        }

        // Sort angle data by angle value
        angleData.sort(function(a, b) {
            return a.angle - b.angle;
        });
        
        // Create arrays for the angle values and frequencies
        var angles = angleData.map(function(data) { return data.angle; });
        var frequencies = angleData.map(function(data) { return data.frequency; });
        
        const data = {
            datasets: [
                {
                    label: 'Angle Frequencies',
                    data: frequencies,
                    backgroundColor: 'rgb(148, 0, 211, 0.6)', // Neon purple color
                    borderColor: 'rgb(148, 0, 211)', // Neon purple color
                    borderWidth: 1,
                },
            ],
        };

        if (chartRef.current) {
            chartRef.current.destroy();
        }

        chartRef.current = new Chart(ctx, {
            type: 'polarArea',
            data: data,
            options: {
                responsive: true,
                plugins: {
                    legend: {
                        display: false,
                    },
                    title: {
                        display: true,
                        text: 'QUEEN BEE ORIENTATION HEATMAP',
                        color: '#FF00FF',
                        font: {  
                            size: 30,
                            family: 'Arial',
                        }
                    }
                },
                scales: {
                    r: {
                        ticks: {
                            beginAtZero: true,
                            max: Math.max(...frequencies),
                            stepSize: 1
                        },
                        reverse: false 
                    }
                },
                startAngle: -Math.PI / 2,
                animation: {
                    animateRotate: true 
                },
            },
        });
    }, [angleArray]);

    return <canvas ref={canvasRef} />;
};

export default PolarHeatmap;
