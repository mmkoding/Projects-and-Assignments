import React, { Component } from 'react';
import Datatable from '../../manage_components/Datatable';
import NawbarAdm from '../../manage_components/NawbarAdm';
import SidebarAdm from '../../manage_components/SidebarAdm';
import '../Manage.scss';

class Detection extends Component {
  constructor(props) {
    super(props);

    // initialize state here
    this.state = {};
  }

  render() {
    return (
      <div className="manage">
        <SidebarAdm />
        <div className="manageContainer">
          <NawbarAdm />
          <Datatable />
        </div>
      </div>
    );
  }
}

export default Detection;