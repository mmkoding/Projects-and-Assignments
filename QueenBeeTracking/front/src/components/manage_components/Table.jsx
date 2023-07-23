import "./table.scss";
import Table from "@mui/material/Table";
import TableBody from "@mui/material/TableBody";
import TableCell from "@mui/material/TableCell";
import TableContainer from "@mui/material/TableContainer";
import TableHead from "@mui/material/TableHead";
import TableRow from "@mui/material/TableRow";
import Paper from "@mui/material/Paper";
import { Link } from "react-router-dom";
import { useState } from "react";
import { useEffect } from "react";
import IconButton from '@mui/material/IconButton';
import CheckIcon from '@mui/icons-material/Check';

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

function getStatusColor(status) {
  switch (status) {
    case "Done":
      return "green";
    case "Inactive":
      return "red";
    default:
      return "orange";
  }
}

const List = (Bags) => {
  const [bags, setBags] = useState(Object.keys(Bags).map((id) => Bags[id])[0]);

  const updateStatus = (bagId, newStatus) => {
    const updatedBags = bags.map((bag) =>
      bag.id === bagId ? { ...bag, status: newStatus } : bag
    );
    setBags(updatedBags);
  };

  const DropdownComponent = ({ row }) => {
    const [selectedValue, setSelectedValue] = useState(0.5);
    const values = Array.from({ length: 19 }, (_, i) => 0.5 + i * 0.25);

    const handleChange = (event) => {
      setSelectedValue(event.target.value);
    };

    const handleDone = () => {
      updateStatus(row.id, "Pending");
    };

    return (
      <div>
        <select onChange={handleChange}>
          {values.map((value, index) => (
            <option key={index} value={value}>
              {value}
            </option>
          ))}
        </select>
        <IconButton color="primary" onClick={handleDone}>
          <CheckIcon />
        </IconButton>
      </div>
    );
  };

  return (
    <TableContainer component={Paper} className="table">
      <Table sx={{ minWidth: 650 }} aria-label="simple table">
        <TableHead>
          <TableRow>
            <TableCell className="tableCell">Action</TableCell>
            <TableCell className="tableCell">Tracking ID</TableCell>
            <TableCell className="tableCell">Bag Name</TableCell>
            <TableCell className="tableCell">Created Date</TableCell>
            <TableCell className="tableCell">Uploaded Date</TableCell>
            <TableCell className="tableCell">Approval Status</TableCell>
          </TableRow>
        </TableHead>
        <TableBody>
          {bags.map((row) => (
            <TableRow key={row.id}>
              <TableCell className="tableCell">
                <Link
                  to={`/bag/${row.id}/${row.name}/outcomes/`}
                  style={{ textDecoration: "none" }}
                >
                  <div className="viewButton">See Details</div>
                </Link>
              </TableCell>
              <TableCell className="tableCell">{row.id}</TableCell>
              <TableCell className="tableCell">{row.name}</TableCell>
              <TableCell className="tableCell">
                {convertDateString(row.name)}
              </TableCell>
              <TableCell className="tableCell">
                {convertDateString(row.date)}
              </TableCell>
              <TableCell className="tableCell">
                <span style={{ color: getStatusColor(row.status) }}>
                  {row.status}
                </span>
              </TableCell>
            </TableRow>
          ))}
        </TableBody>
      </Table>
    </TableContainer>
  );
};

export default List;
