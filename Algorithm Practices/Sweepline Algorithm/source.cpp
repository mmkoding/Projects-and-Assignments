#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <algorithm>
#include <cmath>

using namespace std;

struct Segment {
  string id;
  double x1, y1, x2, y2;
};

double sweepline_x = 0.0;

struct CompareSegments {
  bool operator() (const Segment &a, const Segment &b) const {
    double y1 = a.y1 + ((a.y2 - a.y1) / (a.x2 - a.x1)) * (sweepline_x - a.x1);
    double y2 = b.y1 + ((b.y2 - b.y1) / (b.x2 - b.x1)) * (sweepline_x - b.x1);
    return y1 < y2;
  }
};

double minDist = 1e9;
string above, below, temp_above, temp_below;

double calcY(const Segment &a, double x) {
  if(a.x1 == a.x2) return a.y1;
  return a.y1 + ((a.y2 - a.y1) / (a.x2 - a.x1)) * (x - a.x1);
}

void calc(const vector<Segment>& segments) {
  set<double> uniqueXCoordinates;
  vector<pair<double, Segment>> events;  // <x, segment>
  for (const auto& seg : segments) {
    events.push_back({seg.x1, seg});
    events.push_back({seg.x2, seg});
    uniqueXCoordinates.insert(seg.x1);
    uniqueXCoordinates.insert(seg.x2);
  }

  sort(events.begin(), events.end(), [](const pair<double, Segment> &a, const pair<double, Segment> &b) {
    return a.first < b.first;
  });

  set<Segment, CompareSegments> status;

  for (const auto& x : uniqueXCoordinates) {
    sweepline_x = x;

    auto rng = equal_range(events.begin(), events.end(), make_pair(x, Segment()), [](const pair<double, Segment> &a, const pair<double, Segment> &b) {
        return a.first < b.first;
    });

    for (auto i = rng.first; i != rng.second; ++i) {
      const auto& seg = i->second;

      if (status.count(seg)) {
        status.erase(seg);
      } else {
        auto it = status.lower_bound(seg);
        if (it != status.end()) {
          double dist = calcY(seg, sweepline_x) - calcY(*it, sweepline_x);
		  if(dist > 0){
			  temp_above = seg.id;
			  temp_below = it->id;
		  }else{
			  temp_above = it->id;
			  temp_below = seg.id;
			  dist = -dist;
		  }
          if (dist < minDist) {
            minDist = dist;
            above = temp_above;
            below = temp_below;
          }
        }
        if (it != status.begin()) {
          --it;
          double dist = calcY(seg, sweepline_x) - calcY(*it, sweepline_x);
		  if(dist > 0){
			  temp_above = seg.id;
			  temp_below = it->id;
		  }else{
			  temp_above = it->id;
			  temp_below = seg.id;
			  dist = -dist;
		  }
		  
          if (dist < minDist) {
            minDist = dist;
            above = temp_above;
            below = temp_below;
          }
        }

        status.insert(seg);
      }
    }
  }
}

int main() {
  int n;
  cin >> n;
  vector<Segment> segments(n), y_axis_symmetric_segments(n);
  for (int i = 0; i < n; i++) {
    cin >> segments[i].id >> segments[i].x1 >> segments[i].y1 >> segments[i].x2 >> segments[i].y2;
    y_axis_symmetric_segments[i].y1 = segments[i].y1;
    y_axis_symmetric_segments[i].y2 = segments[i].y2;
    y_axis_symmetric_segments[i].x1 = -segments[i].x1;
    y_axis_symmetric_segments[i].x2 = -segments[i].x2;
    y_axis_symmetric_segments[i].id = segments[i].id;
  }

  calc(segments);
  calc(y_axis_symmetric_segments);
  
  cout << above << " " << below << endl;

  return 0;
}
