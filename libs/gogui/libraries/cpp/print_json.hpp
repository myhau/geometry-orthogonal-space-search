#ifndef PRINT_JSON_HPP
#define PRINT_JSON_HPP

#include "history.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

#include "libraries/cppjson/include/cppjson.h"

namespace gogui {

namespace {
class JSONPrinter {
 private:
  const std::vector<History::State>& history;

  typedef unsigned int PointID;
  typedef unsigned int LineID;

  class InternalLine {
   public:
    PointID point1id, point2id;
    bool operator == (const InternalLine& that) const {
      return point1id == that.point1id && point2id == that.point2id;
    }
    bool operator < (const InternalLine& that) const {
      return point1id == that.point1id ? point2id < that.point2id : point1id < that.point1id;
    }
    void normalize() {
      if (point1id > point2id)
        std::swap(point1id, point2id);
    }
  };

  std::vector<Point> points;
  std::vector<InternalLine> lines;

 public:
  explicit JSONPrinter(const std::vector<History::State>& _history)
  : history(_history)
  {}

  std::string getJSON() {
    getPoints();
    getLines();

    std::stringstream buf;    
    json::object_map_t output;
    
    output["points"] = getJSONPointsDefinition();
    output["lines"] = getJSONLinesDefinition();
    output["history"] = getJSONAllStates();

    json::Value(output).write(buf);
    return buf.str();
  }
  
 protected:
  
  json::Value getJSONPointsDefinition() {
    std::vector<json::Value> json_points;
    for (Point& point : this->points) {
      json::object_map_t json_point;
      json_point["x"] = point.x;
      json_point["y"] = point.y;
      json_points.push_back(json_point);
    }
    return json::Value(json_points);
  }
  
  json::Value getJSONLinesDefinition() {
    std::vector<json::Value> json_lines;
    for (InternalLine& line : this->lines) {
        json::object_map_t json_line;
        json_line["p1"] = (int) line.point1id;
        json_line["p2"] = (int) line.point2id;
        json_lines.push_back(json_line);
    }
    return json::Value(json_lines);
  }
    
  json::Value getJSONAllStates() {
    std::vector<json::Value> json_states;
    for (const History::State& state : history) {      
      json_states.push_back(getJSONState(state));
    }
    return json::Value(json_states);
  }

  static GeoObject::Status mergeStatus(GeoObject::Status s1, GeoObject::Status s2) {
    if(s1==GeoObject::Status::Active || s2==GeoObject::Status::Active) {
      return GeoObject::Status::Active;
    }
    else if(s1==GeoObject::Status::Normal || s2==GeoObject::Status::Normal) {
      return GeoObject::Status::Normal;
    }
    return GeoObject::Status::Processed;
  }

  static std::string statusToString(GeoObject::Status s) {
    switch(s) {
      case GeoObject::Status::Normal:
        return "normal";
      case GeoObject::Status::Active:
        return "active";
      case GeoObject::Status::Processed:
        return "processed";
    }
  }
  
  json::Value getJSONState(const History::State& state) {
    json::object_map_t json_state;

    std::map<Point, GeoObject::Status> displayPoints;    
    for(const Point& point : state.getPoints()) {
      if(displayPoints.count(point))
        displayPoints[point] = mergeStatus(displayPoints[point], point.getStatus());
      else
        displayPoints[point] = point.getStatus();
    }

    std::vector<json::Value> json_points;
    for(const auto pair : displayPoints) {
      const Point& point = pair.first;
      const GeoObject::Status status = pair.second;
      const int pointId = getPointID(point);

      json::object_map_t json_point;
      json_point["pointID"] = pointId;
      json_point["style"] = statusToString(point.getStatus());
      json_points.push_back(json_point);
    }
    json_state["points"] = json_points;
    
    std::vector<json::Value> json_lines;
    for(const Line& line : state.getLines()) {
      json::object_map_t json_line;
      json_line["lineID"] = (int) getLineID(line);
      json_line["style"] = statusToString(line.getStatus());
      json_lines.push_back(json_line);
    }
    json_state["lines"] = json_lines;
    return json::Value(json_state);
  }

 private:
 
  void getPoints() {
    points.clear();

    // Collect all points
    for(const History::State& state : history) {
      for(const Point& point : state.getPoints()) {
        points.push_back(point);
      }
      for(const Line& line : state.getLines()) {
        const Point& point1 = line.point1;
        const Point& point2 = line.point2;
        points.push_back(point1);
        points.push_back(point2);
      }
    }

    // Order points and remove duplicates
    sort(points.begin(), points.end());
    points.erase(std::unique(points.begin(), points.end()), points.end());
  }

  PointID getPointID(const Point& point) {
    std::vector<Point>::iterator it = std::lower_bound(points.begin(), points.end(), point);
    assert(it != points.end());
    return std::distance(points.begin(), it);
  }

  void getLines() {
    lines.clear();

    // Collect all lines
    for(const History::State& state : history) {
      for(const Line& line : state.getLines()) {
        const Point& point1 = line.point1;
        const Point& point2 = line.point2;

        InternalLine iline;
        iline.point1id = getPointID(point1);
        iline.point2id = getPointID(point2);
        iline.normalize();
        lines.push_back(iline);
      }
    }

    // Order lines and remove duplicates
    sort(lines.begin(), lines.end());
    std::vector<InternalLine>::iterator it = std::unique(lines.begin(), lines.end());
    lines.resize(std::distance(lines.begin(), it));
  }

  LineID getLineID(const InternalLine& iline) {
    std::vector<InternalLine>::iterator it = std::lower_bound(
      lines.begin(), lines.end(), iline);
    assert(it != lines.end());
    return std::distance(lines.begin(), it);
  }

  LineID getLineID(Line line) {
    InternalLine iline;
    iline.point1id = getPointID(line.point1);
    iline.point2id = getPointID(line.point2);
    iline.normalize();
    return getLineID(iline);
  }
};

}  // namespace

void printJSON() {
  JSONPrinter jsp(History::getInstance().getStates());
  std::cout << jsp.getJSON();
}

}  // namespace gogui

#endif
