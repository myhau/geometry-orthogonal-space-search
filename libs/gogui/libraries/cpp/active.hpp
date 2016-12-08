namespace gogui {
  class ActivePoint : public Point, public AbstractContainer {
   public:
  	ActivePoint(const Point& point) : Point(point)
  	{
  	  setStatus(Status::Active);
  	}

  	void forEachPoint(PointAcceptor f) const final {
      f(*this);
  	}
  	void forEachLine(LineAcceptor) const final
  	{}
  };

  class ActiveLine : public Line, public AbstractContainer {
   public:
  	ActiveLine(const Line& line) : Line(line)
  	{
  	  setStatus(Status::Active);
  	}
  	ActiveLine(const Point& p1, const Point& p2) : ActiveLine(Line(p1, p2))
  	{}

  	void forEachPoint(PointAcceptor) const final
  	{}
  	void forEachLine(LineAcceptor f) const final {
      f(*this);
  	}
  };
}