#ifndef SUPERVISOR_HPP
#define SUPERVISOR_HPP

#include <unordered_set>
#include "point.hpp"
#include "line.hpp"

namespace gogui {

class AbstractContainer;

class Supervisor {

 private:
  Supervisor() = default;
  Supervisor(const Supervisor &) = delete;

  std::unordered_set<const AbstractContainer*> existingContainers;

 public:
  static Supervisor& getInstance() {
    static Supervisor instance;
    return instance;
  }

  void registerContainer(const AbstractContainer* containerPtr) {
    existingContainers.insert(containerPtr);
  }

  void unregisterContainer(const AbstractContainer* containerPtr) {
    existingContainers.erase(containerPtr);
  }

  const std::unordered_set<const AbstractContainer*>& getRegisteredContainers() const {
    return existingContainers;
  }
};
}

#endif // SUPERVISOR_HPP
