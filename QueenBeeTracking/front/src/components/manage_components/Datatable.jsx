import "./datatable.scss";
import { DataGrid } from "@mui/x-data-grid";
// import { userColumns } from "./detectiondatatablesource";
import { Link } from "react-router-dom";
import { useState, useEffect } from "react";

const userColumns = [
  {
    field: "bagname",
    headerName: "Bag Name",
    width: 330,
    renderCell: (params) => {
      return (
        <div className="cellWithImg">
          {params.row.bagname}
        </div>
      );
    },
  },
  {
    field: "framecount",
    headerName: "Frame Count",
    width: 200,
    renderCell: (params) => {
      return (
        <div className="cellWithStatus">
          {params.row.framecount}
        </div>
      );
    }
  },
  {
    field: "detectedframecount",
    headerName: "Detected Frame Count",
    width: 200,
    renderCell: (params) => {
      return (
        <div className="cellWithStatus">
          {params.row.detectedframecount}
        </div>
      );
    }
  },
  {
    field: "status",
    headerName: "Status",
    width: 260,
    renderCell: (params) => {
      return (
        <div className={`cellWithStatus ${params.row.status}`}>
          {params.row.status}
        </div>
      );
    },
  },
];

const Datatable = () => {
  const [data, setData] = useState([]);


  const fetchDetectedFrameCount = async (bagId, hiveId) => {
    const response = await fetch(`http://144.122.48.179:8000/detections/${bagId}/hive_0/detected_frame_count`);
    const countData = await response.json();
    return countData.data;
  }

  const fetchFrameCount = async (bagId, hiveId) => {
    const response = await fetch(`http://144.122.48.179:8000/bags/${bagId}/hive_0/frame_count`);
    const frameCountData = await response.json();
    return frameCountData.data;
  }

  const fetchBagData = async () => {
    const response = await fetch('http://144.122.48.179:8000/bags');
    const rawData = await response.json();
    
    const dataPromises = rawData.data.map(async bag => {
      const frame_count = await fetchFrameCount(bag.id, 'hive_0');  // assuming 'hive_0' as the hive id
      const detected_frameCount = await fetchDetectedFrameCount(bag.id, 'hive_0');  // assuming 'hive_0' as the hive id
      return {
        id: bag.id,
        bagname: bag.name,
        framecount: frame_count,
        detectedframecount: detected_frameCount,  
        status: "active",  // update this if your API returns a status
      };
    });

    const data = await Promise.all(dataPromises);
    console.log(data[0].bagname);
  
    setData(data);
  }

  useEffect(() => {
    fetchBagData();
  }, []);

  const handleDelete = (id) => {
    setData(data.filter((item) => item.id !== id));
  };

  const actionColumn = [
    {
      field: "action",
      headerName: "Action",
      width: 200,
      renderCell: (params) => {
        return (
          <div className="cellAction">
            <Link to={`/bag/${params.row.id}/${params.row.bagname}/outcomes/bag_info`} style={{ textDecoration: "none" }}>
              <div className="viewButton">View</div>
            </Link>
            <div
              className="deleteButton"
              onClick={() => handleDelete(params.row.id)}
            >
              Delete
            </div>
          </div>
        );
      },
    },
  ];

  return (
    <div className="datatable">
      <div className="datatableTitle">
        Add New Bag File
        <Link to="/users/new" className="link">
          Add New
        </Link>
      </div>
      <DataGrid
        className="datagrid"
        rows={data}
        columns={userColumns.concat(actionColumn)}
        pageSize={9}
        rowsPerPageOptions={[9]}
        checkboxSelection
      />
    </div>
  );
};

export default Datatable;
