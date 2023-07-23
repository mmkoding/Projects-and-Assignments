import React from 'react';
import { Route, Switch, useParams, useRouteMatch } from 'react-router-dom';
import OutcomesNavbar from '../../manage_components/OutcomesNavbar';
import Detections from '../../manage_components/DetectionsComponent';
import FrameAnalysis from '../../manage_components/FrameAnalysis';
import BagInfoComponent from '../../manage_components/BagInfoComponent';
import Tracking from '../../manage_components/TrackingComponent';
import CourtBees from '../../manage_components/CourtBeesComponent';
import SidebarAdm from '../../manage_components/SidebarAdm';
import DownsampleComponent from '../../manage_components/DownsampleComponent';

import './Bagoutcomes.scss';


function Outcomes() {
  const { id } = useParams();
  const { path } = useRouteMatch();

  return (
    <div className="manage">
      <SidebarAdm />
      <div className="manageContainer">
        <OutcomesNavbar />
        <Switch>
          <Route path={`${path}`} exact render={() => <BagInfoComponent id={id} />} />
          <Route path={`${path}/bag_info`} exact render={() => <BagInfoComponent id={id} />} />
          <Route path={`${path}/downsampler`} exact render={() => <DownsampleComponent id={id} />} />
          <Route path={`${path}/detections`} render={() => <Detections id={id} />} />
          <Route path={`${path}/frameanalysis`} render={() => <FrameAnalysis id={id} />} />
          <Route path={`${path}/tracking`} render={() => <Tracking />} />
          <Route path={`${path}/courtbees`} render={() => <CourtBees />} />
        </Switch>
      </div>
    </div>
  );
}

export default Outcomes;
