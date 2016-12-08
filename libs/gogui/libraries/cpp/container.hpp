#ifndef CONTAINER_HPP
#define CONTAINER_HPP

#include "supervisor.hpp"
#include <vector>
#include <functional>

namespace gogui{

class Point;
class Line;
    
class AbstractContainer
{
 public:
  AbstractContainer() {
    Supervisor::getInstance().registerContainer(this);
  }
  ~AbstractContainer() {
    Supervisor::getInstance().unregisterContainer(this);
  }
 
  typedef std::function<void(const Line&)> LineAcceptor;
  typedef std::function<void(const Point&)> PointAcceptor;

  virtual void forEachLine(LineAcceptor) const = 0;
  virtual void forEachPoint(PointAcceptor) const = 0;
};

}
#endif // CONTAINER_HPP
