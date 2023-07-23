import React from 'react';
import "./chart.scss";
import {
  BarChart,
  Bar,
  XAxis,
  YAxis,
  CartesianGrid,
  Tooltip,
  ResponsiveContainer,
} from "recharts";

const Chart = ({ aspect, title, bags }) => {
  // Construct the data array from bags data

  let arrayWithUniqueDates = [...new Set(bags.map(bag => bag.date.substring(0,10)))];

  let data = arrayWithUniqueDates.map(date => {
    let count = bags.filter(bag => bag.date.substring(0,10) === date).length;
    return {
      name: date,
      Total: count,
    }
  });

  return (
    <div className="chart">
      <div className="title">{title}</div>
      <ResponsiveContainer
        debounce={300}
        width="100%"
        aspect={aspect}
        onError={(err) => err.preventDefault()}
      >
        <BarChart
          width={730}
          height={250}
          data={data}
          margin={{ top: 10, right: 30, left: 0, bottom: 0 }}
        >
          <CartesianGrid strokeDasharray="3 3" className="chartGrid" />
          <XAxis dataKey="name" stroke="gray" />
          <YAxis />
          <Tooltip />
          <Bar
            dataKey="Total"
            fill="#8884d8"
          />
        </BarChart>
      </ResponsiveContainer>
    </div>
  );
};

export default Chart;
