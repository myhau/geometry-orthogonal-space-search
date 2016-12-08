#ifndef HISTORY_HPP
#define HISTORY_HPP

namespace gogui {

class History {
 public:
  class State {
   private:
    std::vector<Point> points;
    std::vector<Line> lines;

   public:
    State() = default;
    State(const std::vector<Point>& statePoints, const std::vector<Line>& stateLines)
    : points(statePoints)
    , lines(stateLines)
    {}

    const std::vector<Point>& getPoints() const { return points; }
    const std::vector<Line>& getLines() const { return lines; }
  };

 private:
  History() = default;
  History(const History&) = delete;
  std::vector<State> states;

 public:
  static History& getInstance() {
    static History instance;
    return instance;
  }

  void putState() {
  	std::vector<Point> points;
    std::vector<Line> lines;

  	const auto& containers = Supervisor::getInstance().getRegisteredContainers();
    for(const AbstractContainer* abstractContainer : containers) {
      abstractContainer->forEachPoint([&points](const Point& p) {
      	points.push_back(p);
      });
      abstractContainer->forEachLine([&lines](const Line& l) {
      	lines.push_back(l);
      });
    }

    states.emplace_back(points, lines);
  }

  const std::vector<State>& getStates() const { return states; }
};

void snapshot() {
	History::getInstance().putState();
}

}

#endif // HISTORY_HPP