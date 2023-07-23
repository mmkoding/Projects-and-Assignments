export const userColumns = [
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
