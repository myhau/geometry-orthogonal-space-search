#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cstdio>
#include <vector>

#include "container.hpp"
#include "line.hpp"

namespace gogui
{
template<typename T>
class vector :
  public std::vector<T>,
  public AbstractContainer
{
  static_assert(std::is_base_of<GeoObject, T>::value, "gogui containers can hold only gogui geo objects");

 public:
  vector()
  : std::vector<T>()
  {}

  vector(const std::vector<T>& that)
  : std::vector<T>(that)
  {}

  vector(std::vector<T>&& that)
  : std::vector<T>(std::forward(that))
  {}

  enum class VisualizationMethod {
    CLOUD, PATH, CLOSED_PATH
  };

  void visualizeAs(VisualizationMethod vis) { visualizationMethod = vis; }
  
  void forEachLine(LineAcceptor) const final;
  void forEachPoint(PointAcceptor) const final;

 private:
  VisualizationMethod visualizationMethod = VisualizationMethod::CLOUD;
};

template<>
void vector<Point>::forEachLine(LineAcceptor f) const {
  if(visualizationMethod == VisualizationMethod::PATH || visualizationMethod == VisualizationMethod::CLOSED_PATH) {
    const Point* previousPoint = nullptr;
    for(const Point& point : *this) {
      if(previousPoint != nullptr) {
        const Line line(*previousPoint, point);
        f(line);
      }
      previousPoint = &point;
    }
    if(visualizationMethod == VisualizationMethod::CLOSED_PATH && size()>2) {
      const Line closingLine(front(), back());
      f(closingLine);
    }
  }
}

template<>
void vector<Point>::forEachPoint(PointAcceptor f) const {
  for(const Point& point : *this) {
    f(point);
  }
}

template<>
void vector<Line>::forEachLine(LineAcceptor f) const {
  for(const Line& line : *this) {
    f(line);
  }
}

template<>
void vector<Line>::forEachPoint(PointAcceptor f) const {}

}

#endif // VECTOR_HPP