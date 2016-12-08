#ifndef COMMON_HPP
#define COMMON_HPP

#include <cmath>

namespace gogui {

class GeoObject
{
 public:
  enum class Status {
  	Normal, Active, Processed
  };
  Status getStatus() const { return status; }
  void setStatus(Status s) { status = s; }

 private:
  Status status = Status::Normal;
};

bool compareDouble (const double a, const double b) {
  return fabs(a - b) < 1e-10;
}

}

#endif // COMMON_HPP
